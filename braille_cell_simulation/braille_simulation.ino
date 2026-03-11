#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ----------- INPUT PINS -----------
#define D1 13
#define D2 12
#define D3 14
#define D4 27
#define D5 26
#define D6 25

// ----------- DOT LEDs -----------
#define L1 18
#define L2 19
#define L3 21
#define L4 22
#define L5 23
#define L6 32

// ----------- OUTPUT LED -----------
#define OUTPUT_LED 2

// ----------- OLED SETTINGS -----------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_SDA 4
#define OLED_SCL 15

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

String lastPattern = "";
bool systemReady = false;

void setup() {

  Serial.begin(115200);

  // Initialize I2C on custom pins
  Wire.begin(OLED_SDA, OLED_SCL);

  // Inputs
  pinMode(D1, INPUT_PULLUP);
  pinMode(D2, INPUT_PULLUP);
  pinMode(D3, INPUT_PULLUP);
  pinMode(D4, INPUT_PULLUP);
  pinMode(D5, INPUT_PULLUP);
  pinMode(D6, INPUT_PULLUP);

  // Dot LEDs
  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
  pinMode(L3, OUTPUT);
  pinMode(L4, OUTPUT);
  pinMode(L5, OUTPUT);
  pinMode(L6, OUTPUT);

  // Output LED
  pinMode(OUTPUT_LED, OUTPUT);

  // OLED Init
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while(true);
  }

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 10);
  display.println("Braille");
  display.setCursor(0, 35);
  display.println("Ready");
  display.display();

  delay(1000);

  // Capture initial pattern
  int b1 = !digitalRead(D1);
  int b2 = !digitalRead(D2);
  int b3 = !digitalRead(D3);
  int b4 = !digitalRead(D4);
  int b5 = !digitalRead(D5);
  int b6 = !digitalRead(D6);

  lastPattern = String(b1) + String(b2) + String(b3) +
                String(b4) + String(b5) + String(b6);

  systemReady = true;
}

void loop() {

  int b1 = !digitalRead(D1);
  int b2 = !digitalRead(D2);
  int b3 = !digitalRead(D3);
  int b4 = !digitalRead(D4);
  int b5 = !digitalRead(D5);
  int b6 = !digitalRead(D6);

  // Update dot LEDs
  digitalWrite(L1, b1);
  digitalWrite(L2, b2);
  digitalWrite(L3, b3);
  digitalWrite(L4, b4);
  digitalWrite(L5, b5);
  digitalWrite(L6, b6);

  String pattern = String(b1) + String(b2) + String(b3) +
                   String(b4) + String(b5) + String(b6);

  if(systemReady && pattern != lastPattern) {

    char letter = detectBraille(pattern);

    if(letter != 0) {

      digitalWrite(OUTPUT_LED, HIGH);

      Serial.print("BT_TX: ");
      Serial.println(letter);

      display.clearDisplay();
      display.setTextSize(3);
      display.setCursor(40, 20);
      display.print(letter);
      display.display();

    } else {

      digitalWrite(OUTPUT_LED, LOW);

      display.clearDisplay();
      display.setTextSize(1);
      display.setCursor(10, 25);
      display.println("Invalid Braille");
      display.display();
    }

    lastPattern = pattern;
  }

  delay(150);
}

// ----------- BRAILLE MAPPING -----------

char detectBraille(String p) {

  // A–J
  if (p == "100000") return 'A';
  if (p == "110000") return 'B';
  if (p == "100100") return 'C';
  if (p == "100110") return 'D';
  if (p == "100010") return 'E';
  if (p == "110100") return 'F';
  if (p == "110110") return 'G';
  if (p == "110010") return 'H';
  if (p == "010100") return 'I';
  if (p == "010110") return 'J';

  // K–T
  if (p == "101000") return 'K';
  if (p == "111000") return 'L';
  if (p == "101100") return 'M';
  if (p == "101110") return 'N';
  if (p == "101010") return 'O';
  if (p == "111100") return 'P';
  if (p == "111110") return 'Q';
  if (p == "111010") return 'R';
  if (p == "011100") return 'S';
  if (p == "011110") return 'T';

  // U–Z
  if (p == "101001") return 'U';
  if (p == "111001") return 'V';
  if (p == "010111") return 'W';
  if (p == "101101") return 'X';
  if (p == "101111") return 'Y';
  if (p == "101011") return 'Z';

  return 0;
}
