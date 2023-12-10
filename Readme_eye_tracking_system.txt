## Overview

This Python script uses OpenCV to perform real-time face and eye detection using Haar cascade classifiers. It monitors the eyes for signs of closure or if the eyes are not on the road and triggers an alert if closed eyes are detected continuously for a specified duration.

## Dependencies

- OpenCV: Open Source Computer Vision Library
- NumPy: Fundamental package for scientific computing with Python
- time: Provides various time-related functions
- playsound: Library for playing sound files

##Functionality

The code utilizes the OpenCV library to:
1-Read frames from the infrared camera.
2-Convert frames to grayscale for face and eye detection.
3-Detect faces using the pre-trained Haar cascade for frontal face detection.
4-Extract the region of interest (ROI) for eyes within the detected face.
5-Detect eyes within the ROI using the pre-trained Haar cascade for eye detection.
6-Check if the eye is closed based on the height of the eye region (example).
7-Track the duration without eye capture.
8-Play an alert sound if the user's eyes are closed for a specified threshold (adjustable).

##Code breakdown

Global variables:
-face_cascade: Pre-trained Haar cascade for face detection
-eye_cascade: Pre-trained Haar cascade for eye detection
-cap: OpenCV object for capturing video from the camera
-start_time: Time when faces were last detected
-eye_not_captured_duration: Duration without eye capture
-threshold_seconds: Time threshold for triggering eye closed alert
-alert_sent: Flag to prevent multiple alerts
-sound_alarm function:
   -Plays the alert sound (customizable)
Main loop:
-Reads a frame from the camera
-Converts the frame to grayscale
-Detects faces in the frame
-Resets timer and alert status if faces are detected
-Draws rectangles around detected faces
-Extracts ROI for eyes within each face
-Detects eyes within each ROI
-Draws rectangles around detected eyes
-Checks if the eye is closed based on the height (example)
-Plays an alert sound if the eye is closed and the threshold is reached
-Calculates the duration without eye capture
-Displays counter information (optional)
-Displays the resulting frame
-Breaks the loop if 'q' is pressed or the threshold is reached
Release camera and close windows:
  -Releases the camera capture object
  -Destroys all OpenCV windows

##Note
-This is a basic implementation and may require further adjustments based on your specific needs and camera setup.

