# Braille-to-Text and Speech Conversion System

## Overview

This project is a real-time assistive system that reads physical Braille text and converts it into audible speech. An ESP32-CAM captures images of Braille characters, which are sent over WiFi to a Flask-based server for image processing. The server uses computer vision techniques to detect Braille dots, interpret the characters, and output the recognized text as speech using a text-to-speech engine.

The system is designed to be low-cost, modular, and accessible — providing an affordable assistive solution for visually impaired individuals.

## Features

- Real-time Braille image capture using ESP32-CAM
- CNC plotter-based mechanical scanning for precise, systematic camera positioning
- OpenCV-based image processing pipeline (grayscale, Gaussian blur, adaptive thresholding, morphological operations)
- Contour detection and filtering for accurate Braille dot identification
- 6-dot grid mapping for Braille character recognition (A–Z)
- Temporal filtering (multi-frame validation + debouncing) for stable detection
- Word-level buffering with timeout-based word boundary detection
- Text-to-speech output using pyttsx3 (supports English and Spanish)
- Multithreaded speech queue for non-blocking audio output

## System Architecture

The system is divided into 11 independent modules:

1. **Mechanical Scanning Module** — ESP32-S3 + stepper motors control X/Y axis movement using the AccelStepper library. Push buttons allow manual positioning of the camera over each Braille cell.

2. **Image Acquisition Module** — ESP32-CAM captures images at VGA resolution in JPEG format with sensor-level corrections (vertical flip, horizontal mirror).

3. **Communication Module** — ESP32-CAM connects to WiFi and sends captured images to the Flask server via HTTP POST requests (Content-Type: image/jpeg).

4. **Server Interface Module** — Flask API endpoint (`/upload`) receives the image byte stream, converts it to a NumPy array, and decodes it into an OpenCV-compatible image.

5. **Image Processing Pipeline** — Extracts a Region of Interest (ROI), applies grayscale conversion, Gaussian blur, adaptive thresholding, and morphological opening to isolate Braille dots.

6. **Feature Extraction Module** — Detects contours in the binary image, filters them by area and radius, and computes centroids of valid Braille dots.

7. **Braille Interpretation Module** — Maps detected dot positions onto a 2-column × 3-row grid to form a 6-bit binary vector, which is matched against a lookup table to identify the corresponding character.

8. **Temporal Filtering Module** — Accepts a character only after it is detected consistently for a minimum number of frames (default: 5), with debouncing to prevent duplicate entries.

9. **Word Formation Module** — Buffers recognized characters sequentially; uses a timeout mechanism to detect word boundaries and finalize complete words.

10. **Speech Synthesis Module** — Uses pyttsx3 for offline TTS with selectable voice (English or Spanish). A speech queue with a dedicated worker thread ensures non-blocking speech generation.

11. **Output Module** — Plays synthesized speech through a speaker, providing real-time audio feedback of the interpreted Braille text.

## Hardware Components

- ESP32-CAM (AI Thinker module)
- ESP32-S3 microcontroller
- Stepper motors (X and Y axes)
- Stepper motor driver circuitry
- CNC plotter frame / mechanical scanning structure
- Push buttons (X forward, X backward, Y forward, Y backward)
- Speaker / audio output device
- Power supply
- Laptop/PC running the Flask server

## Software Components

- **Arduino / ESP-IDF:** ESP32-CAM firmware (camera init, WiFi, HTTP POST), ESP32-S3 firmware (AccelStepper motor control)
- **Python / Flask:** Server-side API for receiving and processing images
- **OpenCV:** Image preprocessing and contour detection
- **NumPy:** Image array handling
- **pyttsx3:** Offline text-to-speech engine
- **Python threading / queue:** Multithreaded speech worker

## Working Principle

1. The user positions the camera over a Braille cell using the CNC plotter (via push buttons).
2. The ESP32-CAM captures an image and transmits it to the Flask server via HTTP POST over WiFi.
3. The Flask server preprocesses the image (ROI extraction → grayscale → blur → thresholding → morphological ops).
4. Contour detection identifies Braille dots; their centroids are mapped onto a 6-dot grid.
5. The resulting binary pattern is matched against a Braille lookup table to identify the character.
6. Temporal filtering validates the character across multiple frames to ensure stability.
7. Validated characters are accumulated in a word buffer; a timeout finalizes each complete word.
8. The finalized word is passed to pyttsx3, which speaks it aloud through the speaker.

## Advantages

- Low-cost implementation using open-source hardware and software
- Robust detection under varying lighting using adaptive thresholding
- Reduced false detections via temporal filtering and debouncing
- Word-level speech output for better usability
- Multilingual support (English and Spanish)
- Modular design — easy to debug, extend, and maintain

## Limitations

- Dependent on WiFi availability for image transmission
- Manual button-based scanning (no automatic line/paragraph traversal)
- Supports only basic Braille alphabet (A–Z); no numbers, punctuation, or contractions
- Processing pipeline introduces some latency (not fully instantaneous)
- Camera alignment with Braille dots is sensitive; misalignment reduces accuracy
- Not fully portable due to CNC mechanism and server dependency

## Future Improvements

- Automatic line and paragraph scanning
- Support for Grade 2 Braille (contractions, numbers, punctuation)
- Standalone offline mode (remove WiFi/server dependency)
- Additional language support beyond English and Spanish
- Miniaturized and portable form factor

## Contributors

- Sayandeep Kundu (24BEC0205)
- Trishan Talukdar (24BEC0450)
- Aditya Chakrabarti (24BEC0454)

School of Electronics Engineering, Vellore Institute of Technology
Course: Microprocessors and Microcontrollers (BECE204L)
Faculty: Dr. S. Sundar | Winter Semester 2025–2026

## Demo

[View Demo Video](https://drive.google.com/drive/folders/1y0GSsZr4Jz3g7TpYAep5FCvrYLmXPWHp)

## License

This project is released for educational and research purposes.
