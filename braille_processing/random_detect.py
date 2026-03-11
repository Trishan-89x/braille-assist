import cv2
import numpy as np
import os
import random
import time

# ===== MODE SELECTION (ONLY ONCE) =====
mode = input("Select mode: (L)etter or (N)umber: ").strip().upper()

if mode not in ["L", "N"]:
    print("Invalid selection. Restart program.")
    exit()

print("\nRunning in", "Letter Mode" if mode == "L" else "Number Mode")
print("Press Ctrl+C to stop.\n")
print("--------------------------------------------------")

# ===== BRAILLE MAP =====
base_map = {
    (1,0,0,0,0,0): ('A', '1'),
    (1,1,0,0,0,0): ('B', '2'),
    (1,0,0,1,0,0): ('C', '3'),
    (1,0,0,1,1,0): ('D', '4'),
    (1,0,0,0,1,0): ('E', '5'),
    (1,1,0,1,0,0): ('F', '6'),
    (1,1,0,1,1,0): ('G', '7'),
    (1,1,0,0,1,0): ('H', '8'),
    (0,1,0,1,0,0): ('I', '9'),
    (0,1,0,1,1,0): ('J', '0'),

    (1,0,1,0,0,0): ('K', None),
    (1,1,1,0,0,0): ('L', None),
    (1,0,1,1,0,0): ('M', None),
    (1,0,1,1,1,0): ('N', None),
    (1,0,1,0,1,0): ('O', None),
    (1,1,1,1,0,0): ('P', None),
    (1,1,1,1,1,0): ('Q', None),
    (1,1,1,0,1,0): ('R', None),
    (0,1,1,1,0,0): ('S', None),
    (0,1,1,1,1,0): ('T', None),
    (1,0,1,0,0,1): ('U', None),
    (1,1,1,0,0,1): ('V', None),
    (0,1,0,1,1,1): ('W', None),
    (1,0,1,1,0,1): ('X', None),
    (1,0,1,1,1,1): ('Y', None),
    (1,0,1,0,1,1): ('Z', None),
}

files = os.listdir("braille_dataset")

# ===== CONTINUOUS LOOP =====
try:
    while True:

        random_file = random.choice(files)
        img = cv2.imread(f"braille_dataset/{random_file}")

        gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
        _, thresh = cv2.threshold(gray, 120, 255, cv2.THRESH_BINARY_INV)

        regions = [
            thresh[30:90, 30:90],
            thresh[90:150, 30:90],
            thresh[150:210, 30:90],
            thresh[30:90, 110:170],
            thresh[90:150, 110:170],
            thresh[150:210, 110:170],
        ]

        pattern = []

        for r in regions:
            if cv2.countNonZero(r) > 50:
                pattern.append(1)
            else:
                pattern.append(0)

        pattern_tuple = tuple(pattern)
        detected = base_map.get(pattern_tuple, (None, None))

        if detected[0] is None:
            result = "Unknown"
        else:
            if mode == "L":
                result = detected[0]
            else:
                result = detected[1] if detected[1] is not None else "Not a Number"

        print("Image:", random_file,
              "| Pattern:", pattern,
              "| Output:", result)

        time.sleep(2)

except KeyboardInterrupt:
    print("\nProgram stopped.")
