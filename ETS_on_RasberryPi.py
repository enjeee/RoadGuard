# Readme file in the folder
# Author: IEEE FST RAS SBC
# Hardware: 
# Raspberry Pi 4 B+ / Raspberry Pi Camera / Buzzer 
# Date: 03/12/2023

import cv2
import time
import RPi.GPIO as GPIO
from picamera import PiCamera
from playsound import playsound

# Load the pre-trained Haar cascade for face and eyes
face_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')
eye_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_eye.xml')

# Set up GPIO for the buzzer (adjust the pin number based on your configuration)
BUZZER_PIN = 18
GPIO.setmode(GPIO.BCM)
GPIO.setup(BUZZER_PIN, GPIO.OUT)

# Open the infrared camera
camera = PiCamera()

# Initialize variables for the counter and alert
start_time = time.time()
eye_not_captured_duration = 0
threshold_seconds = 3  # Adjust this threshold as needed
alert_sent = False

def sound_alarm():
    # Activate the buzzer (adjust duration and frequency based on your buzzer)
    GPIO.output(BUZZER_PIN, GPIO.HIGH)
    time.sleep(1)  # Adjust the duration of the buzzer sound
    GPIO.output(BUZZER_PIN, GPIO.LOW)

while True:
    # Capture a frame from the camera
    camera.capture('temp.jpg')
    frame = cv2.imread('temp.jpg')

    # Convert the frame to grayscale for face and eye detection
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    # Detect faces in the frame
    faces = face_cascade.detectMultiScale(gray, scaleFactor=1.3, minNeighbors=5)

    # Reset the timer and alert status if faces are detected
    if len(faces) > 0:
        start_time = time.time()
        eye_not_captured_duration = 0
        alert_sent = False

    for (x, y, w, h) in faces:
        # Draw a rectangle around the detected face
        cv2.rectangle(frame, (x, y), (x + w, y + h), (255, 0, 0), 2)

        # Extract the region of interest (ROI) for eyes within the detected face
        roi_gray = gray[y:y + h, x:x + w]
        eyes = eye_cascade.detectMultiScale(roi_gray)

        for (ex, ey, ew, eh) in eyes:
            # Draw a rectangle around each detected eye
            cv2.rectangle(frame[y:y + h, x:x + w], (ex, ey), (ex + ew, ey + eh), (0, 255, 0), 2)

            # Example: Check if the eye is closed based on the height of the eye region
            eye_closed_threshold = 30
            if eh < eye_closed_threshold:
                if not alert_sent:
                    sound_alarm()
                    alert_sent = True

    # Calculate the duration without eye capture
    current_time = time.time()
    eye_not_captured_duration = current_time - start_time

    # Display the resulting frame
    cv2.imshow('Eye Movement Detection', frame)

    # Break the loop if 'q' is pressed or if the threshold is reached
    if cv2.waitKey(1) & 0xFF == ord('q') or eye_not_captured_duration > threshold_seconds:
        sound_alarm()
        break

# Release the camera and close all OpenCV windows
camera.close()
cv2.destroyAllWindows()
GPIO.cleanup()  # Clean up GPIO settings
