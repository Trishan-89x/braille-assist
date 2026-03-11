# Braille Assistive Web Server

## Overview

This module implements the web server and text-to-speech interface for the Smart Braille Assistive System.

The server receives characters from the ESP32 Braille scanning controller through HTTP requests, reconstructs the detected sentence in real time, displays the text on a web interface, and converts the completed sentence into speech output.

The system supports **multilingual operation (English and Spanish)** and allows monitoring of the detected Braille input through a browser dashboard.

---

## System Architecture

The web server performs three core functions:

1. **Receive text from ESP32 hardware**
2. **Display detected characters in a live web dashboard**
3. **Convert completed sentences into speech using text-to-speech**

Communication between the ESP32 and this server is done through HTTP GET requests.

---

## Features

- Real-time Braille text display
- Multilingual text-to-speech (English / Spanish)
- Browser-based monitoring interface
- Sentence history logging
- REST API communication with ESP32 hardware

---

## Web Interface

The dashboard contains two sections.

### Live Detected Text

Displays characters received from the ESP32 scanner in real time.

### Recent Sentences

Shows previously detected sentences with timestamps after they are spoken.

The interface automatically refreshes every **500 ms** using JavaScript.

---

## API Endpoints

### `/`

Loads the browser interface dashboard.

---

### `/get_data`

Returns the current detected text and sentence history.

Example response:

```json
{
  "live_text": "Hello",
  "recent": [
    {
      "time": "14:22:18",
      "text": "Hello"
    }
  ]
}

/update
Used by the ESP32 controller to send characters and control commands.
Example request:
http://server-ip:5000/update?text=A

Supported Commands
Command	      Description
START_EN	    Start English detection mode
START_ES	    Start Spanish detection mode
START	Reset   current text
CLEAR	Clear   live text
END	Trigger   text-to-speech
A-Z	Append    detected letter

Text-to-Speech
Speech synthesis is implemented using pyttsx3.
Supported voices:
Language	  Voice
English	  Microsoft David
Spanish	  Microsoft Helena
Speech is triggered when the server receives the command:
END
The complete detected sentence is then spoken aloud.

Technologies Used
Python
Flask
pyttsx3
HTML / CSS
JavaScript (AJAX updates)

Purpose in the Full System
This module acts as the central interface between the Braille hardware and the user.
It connects:
ESP32 Braille scanning controller
Real-time web dashboard
Multilingual text-to-speech engine
The server converts detected Braille characters into spoken sentences, enabling accessible interaction for visually impaired users.
