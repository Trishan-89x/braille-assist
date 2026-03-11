#include <WiFi.h>
#include <HTTPClient.h>
#include <AccelStepper.h>

/* ================= WIFI ================= */

const char* ssid = "Trishan";
const char* password = "zt1234zt";
String server = "http://172.17.14.252:5000/update?text=";

#define SW_EN      4
#define SW_ES      5
#define START_BTN  14s
#define RESET_BTN  15

#define LED_EN     12
#define LED_ES     13

/* ================= CNC ================= */

#define MotorInterfaceType 8

#define X1 6
#define X2 7
#define X3 8
#define X4 9

#define Y1 16
#define Y2 17
#define Y3 18
#define Y4 21

#define STEP_SEGMENT 1334
#define MAX_DISTANCE 10000

AccelStepper stepperX(MotorInterfaceType, X1, X3, X2, X4);
AccelStepper stepperY(MotorInterfaceType, Y1, Y3, Y2, Y4);

/* ================= GLOBAL ================= */

String activeSentence = "";
int letterIndex = 0;
bool endSent = false;
bool scanning = false;

long currentX = 0;
long currentY = 0;

/* ================= WIFI FUNCTIONS ================= */

void connectWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void sendText(String text) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    text.replace(" ", "%20");
    String url = server + text;
    http.begin(url);
    http.GET();
    http.end();
  }
}

/* ================= MOVE ================= */

void moveXY(long x, long y, int fastMode)
{
  if (fastMode == 0) {
    stepperX.setMaxSpeed(800);
    stepperY.setMaxSpeed(800);
  } else {
    stepperX.setMaxSpeed(1500);
    stepperY.setMaxSpeed(1500);
  }

  stepperX.moveTo(x);
  stepperY.moveTo(y);

  while (stepperX.distanceToGo() != 0 || stepperY.distanceToGo() != 0)
  {
    if (digitalRead(RESET_BTN) == LOW) return;

    stepperX.run();
    stepperY.run();
  }

  currentX = x;
  currentY = y;
}

/* ================= RESET ================= */

void performReset()
{
  scanning = false;
  letterIndex = 0;
  endSent = false;

  sendText("CLEAR");

  moveXY(0, 0, 1);   // fastest path to origin
}

/* ================= SETUP ================= */

void setup()
{
  pinMode(SW_EN, INPUT_PULLUP);
  pinMode(SW_ES, INPUT_PULLUP);
  pinMode(START_BTN, INPUT_PULLUP);
  pinMode(RESET_BTN, INPUT_PULLUP);

  pinMode(LED_EN, OUTPUT);
  pinMode(LED_ES, OUTPUT);

  stepperX.setAcceleration(1000);
  stepperY.setAcceleration(1000);

  connectWiFi();
}

/* ================= LOOP ================= */

void loop()
{
  bool enActive = (digitalRead(SW_EN) == LOW);
  bool esActive = (digitalRead(SW_ES) == LOW);
  bool startPressed = (digitalRead(START_BTN) == LOW);
  bool resetPressed = (digitalRead(RESET_BTN) == LOW);

  /* ---- STRICT LED LOGIC ---- */

  if (enActive && !esActive) {
    digitalWrite(LED_EN, HIGH);
    digitalWrite(LED_ES, LOW);
  }
  else if (!enActive && esActive) {
    digitalWrite(LED_EN, LOW);
    digitalWrite(LED_ES, HIGH);
  }
  else {
    digitalWrite(LED_EN, LOW);
    digitalWrite(LED_ES, LOW);
  }

  /* ---- RESET ---- */

  if (resetPressed) {
    performReset();
    return;
  }

  /* ---- START ---- */

  if (startPressed && !scanning)
  {
    if (enActive && !esActive) {
      activeSentence = "Good afternoon";
      sendText("START_EN");
    }
    else if (!enActive && esActive) {
      activeSentence = "Buenas tardes";
      sendText("START_ES");
    }
    else {
      return;   // invalid state
    }

    scanning = true;
    letterIndex = 0;
    endSent = false;

    currentX = 0;
    currentY = 0;

    while (currentY < MAX_DISTANCE && scanning)
    {
      long tempX = 0;

      while (tempX + STEP_SEGMENT < MAX_DISTANCE && scanning)
      {
        unsigned long waitStart = millis();
        bool letterSent = false;

        while (millis() - waitStart < 5000)
        {
          if (digitalRead(RESET_BTN) == LOW) {
            performReset();
            return;
          }

          if (!letterSent && millis() - waitStart >= 2500)
          {
            if (letterIndex < activeSentence.length())
            {
              sendText(String(activeSentence[letterIndex]));
              letterIndex++;

              if (letterIndex == activeSentence.length() && !endSent)
              {
                sendText("END");
                endSent = true;
              }
            }
            letterSent = true;
          }
        }

        tempX += STEP_SEGMENT;
        moveXY(tempX, currentY, 0);

        if (digitalRead(RESET_BTN) == LOW) {
          performReset();
          return;
        }
      }

      moveXY(0, currentY, 1);

      currentY += STEP_SEGMENT;

      if (currentY >= MAX_DISTANCE)
        break;

      moveXY(0, currentY, 1);
    }

    scanning = false;
  }
}
