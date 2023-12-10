import cv2
import numpy as np
import time
import playsound

# Load the pre-trained Haar cascade for face and eyes
face_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')
eye_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_eye.xml')

# Open the infrared camera (you may need to adjust the index based on your camera)
cap = cv2.VideoCapture(0)

# Initialize variables for the counter and alert
start_time = time.time()
eye_not_captured_duration = 0
threshold_seconds = 3  # Adjust this threshold as needed
alert_sent = False

def sound_alarm():
    # Add your alert sound or notification here
    # For example, using playsound library to play an alert sound
    print("Wake up!")
    playsound.playsound(r"C:\alert_sound.mp3")

while True:
    # Read a frame from the camera
    ret, frame = cap.read()

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
            print(eh)
            if eh < eye_closed_threshold:
        
                if not alert_sent:
                    sound_alarm()
                    alert_sent = True

    # Calculate the duration without eye capture
    current_time = time.time()
    eye_not_captured_duration = current_time - start_time

    # Display the counter information
   # print("Seconds without eye capture: {:.2f}".format(eye_not_captured_duration))
    

    # Display the resulting frame
    cv2.imshow('Eye Movement Detection', frame)

    # Break the loop if 'q' is pressed or if the threshold is reached
    if cv2.waitKey(1) & 0xFF == ord('q') or eye_not_captured_duration > threshold_seconds:
        sound_alarm()
        break

# Release the camera and close all OpenCV windows
cap.release()
cv2.destroyAllWindows()
