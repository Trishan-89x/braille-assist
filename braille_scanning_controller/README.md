# ESP32 Braille Scanning Controller

## Overview
This module controls the mechanical scanning system used in the Smart Braille Assistive System.  
The ESP32 moves a scanning mechanism across a Braille surface using two stepper motors arranged in an X-Y configuration.

During scanning, characters are transmitted to a remote web server over WiFi.  
The server processes the text and provides multilingual text-to-speech feedback.

The system supports both English and Spanish sentence modes.

---

## System Architecture

The controller performs three main tasks:

1. **Motion Control**
   - Controls two stepper motors (X and Y axes)
   - Moves across the scanning surface in a grid pattern

2. **WiFi Communication**
   - Connects to a local network
   - Sends detected characters to a Flask web server

3. **Language Mode Selection**
   - English mode
   - Spanish mode

---

## Hardware Components

- ESP32 microcontroller
- Two stepper motors (X and Y axis)
- Stepper motor drivers
- Start button
- Reset button
- Language selection switches
- Status LEDs
- WiFi connection

---

## Operating Modes

### English Mode
When the English switch is enabled, the system scans and sends the sentence:
Good afternoon

### Spanish Mode
When the Spanish switch is enabled, the system scans and sends the sentence:
Buenas tardes

---

## Scanning Algorithm

1. User selects language (English or Spanish)
2. Start button begins scanning
3. The scanner moves horizontally across the surface
4. Every scan segment:
   - Waits briefly
   - Sends one character to the server
5. When the end of the sentence is reached:
   - Sends the command `END`
6. Scanner moves to the next row
7. Continues until the full area is scanned

---

## Communication with Server

The ESP32 sends HTTP requests to a Flask server:
http://<server-ip>:5000/update?text=<character>


Special commands:

| Command | Function |
|------|------|
| START_EN | Start English session |
| START_ES | Start Spanish session |
| END | End of sentence |
| CLEAR | Reset system |

---

## Pin Configuration

### Language Selection

| Function | GPIO |
|--------|------|
| English Switch | 4 |
| Spanish Switch | 5 |

### Control Buttons

| Function | GPIO |
|--------|------|
| Start Button | 14 |
| Reset Button | 15 |

### Status LEDs

| Function | GPIO |
|--------|------|
| English LED | 12 |
| Spanish LED | 13 |

### Stepper Motor Pins

#### X Axis
| Pin | GPIO |
|----|----|
| X1 | 6 |
| X2 | 7 |
| X3 | 8 |
| X4 | 9 |

#### Y Axis
| Pin | GPIO |
|----|----|
| Y1 | 16 |
| Y2 | 17 |
| Y3 | 18 |
| Y4 | 21 |

---

## Libraries Used

- WiFi.h
- HTTPClient.h
- AccelStepper

---

## Purpose in the Full System

This controller forms the **mechanical scanning component** of the Smart Braille Assistive System.

It integrates with:

- Braille input detection module
- Web server for text processing
- Multilingual text-to-speech system
