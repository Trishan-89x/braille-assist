import cv2
import numpy as np
import os

# Create folder to store images
if not os.path.exists("braille_dataset"):
    os.makedirs("braille_dataset")

# Standard Braille mapping (A–Z and 0–9)
braille_map = {
    'A': (1,0,0,0,0,0),
    'B': (1,1,0,0,0,0),
    'C': (1,0,0,1,0,0),
    'D': (1,0,0,1,1,0),
    'E': (1,0,0,0,1,0),
    'F': (1,1,0,1,0,0),
    'G': (1,1,0,1,1,0),
    'H': (1,1,0,0,1,0),
    'I': (0,1,0,1,0,0),
    'J': (0,1,0,1,1,0),
    'K': (1,0,1,0,0,0),
    'L': (1,1,1,0,0,0),
    'M': (1,0,1,1,0,0),
    'N': (1,0,1,1,1,0),
    'O': (1,0,1,0,1,0),
    'P': (1,1,1,1,0,0),
    'Q': (1,1,1,1,1,0),
    'R': (1,1,1,0,1,0),
    'S': (0,1,1,1,0,0),
    'T': (0,1,1,1,1,0),
    'U': (1,0,1,0,0,1),
    'V': (1,1,1,0,0,1),
    'W': (0,1,0,1,1,1),
    'X': (1,0,1,1,0,1),
    'Y': (1,0,1,1,1,1),
    'Z': (1,0,1,0,1,1),

    # Numbers 1–9 and 0 use same as A–J
    '1': (1,0,0,0,0,0),
    '2': (1,1,0,0,0,0),
    '3': (1,0,0,1,0,0),
    '4': (1,0,0,1,1,0),
    '5': (1,0,0,0,1,0),
    '6': (1,1,0,1,0,0),
    '7': (1,1,0,1,1,0),
    '8': (1,1,0,0,1,0),
    '9': (0,1,0,1,0,0),
    '0': (0,1,0,1,1,0),
}

positions = [
    (60,50),
    (60,120),
    (60,190),
    (140,50),
    (140,120),
    (140,190)
]

for char, pattern in braille_map.items():

    img = np.ones((300, 200), dtype=np.uint8) * 255

    for i in range(6):
        if pattern[i] == 1:
            cv2.circle(img, positions[i], 20, (0,0,0), -1)

    cv2.imwrite(f"braille_dataset/{char}.jpg", img)

print("All Braille images generated successfully.")
