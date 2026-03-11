# Braille Input Simulation (ESP32)

## Overview
This module contains the ESP32 simulation used to test the Braille input interface of the Smart Braille Communication System.  
The simulation replicates a single Braille cell using six input switches representing the six Braille dots.

The system reads the state of the six inputs, converts the dot pattern into a corresponding alphabet character, and displays the detected letter on an OLED display.

This simulation was developed and tested using the Wokwi ESP32 simulator.

---

## Hardware Components (Simulated)

- ESP32 microcontroller
- 6 push buttons representing Braille dots
- 6 LEDs representing active Braille dots
- SSD1306 OLED display (I2C)
- Status LED for valid Braille detection

---

## Braille Cell Representation

A Braille character consists of **six dots** arranged in a 3×2 matrix.

Dot numbering used in the simulation:
1 4
2 5
3 6

Each button corresponds to one dot in the Braille cell.

---

## Working Principle

1. User presses switches corresponding to a Braille pattern.
2. ESP32 reads the state of the six input pins.
3. The six-bit pattern is generated (example: `100000`).
4. The pattern is matched with a Braille mapping table.
5. If valid:
   - The corresponding alphabet letter is displayed on the OLED.
   - The letter is transmitted over Serial (`BT_TX:`).
   - The output LED turns ON.
6. If invalid:
   - OLED displays **Invalid Braille**.

---

## Pin Configuration

### Input Pins (Braille dots)

| Dot | GPIO |
|----|----|
| D1 | 13 |
| D2 | 12 |
| D3 | 14 |
| D4 | 27 |
| D5 | 26 |
| D6 | 25 |

### LED Indicators

| Dot LED | GPIO |
|------|------|
| L1 | 18 |
| L2 | 19 |
| L3 | 21 |
| L4 | 22 |
| L5 | 23 |
| L6 | 32 |

### Other Pins

| Component | GPIO |
|-----------|------|
| Status LED | 2 |
| OLED SDA | 4 |
| OLED SCL | 15 |

---

## Braille Mapping

The system supports detection of **alphabet characters A–Z** using standard 6-dot Braille encoding.

Example:

| Pattern | Letter |
|--------|--------|
| 100000 | A |
| 110000 | B |
| 100100 | C |
| 100110 | D |
| 100010 | E |

---

## Simulation Platform

The simulation was created using:

- **Wokwi ESP32 Simulator**
- Arduino framework
- Adafruit SSD1306 library

---

## Purpose

This simulation validates the **Braille input detection logic** before deploying the system on physical hardware for the Smart Braille Communication System.
