# ENGR 1620 Arduino Code
This is the code that will be used for controlling the LIDAR Lite v3 sensor.
It is intended to turn on an LED if a car is detected by the sensor.

## Uploading the Code
To upload the code to an Arduino, you need to first import Garmin's LIDAR Lite V3 library. Garmin distributes a ZIP file that you need to add to the Arduino IDE library manager.
Now, plug in the Arduino to your computer.
In the IDE, select the board you are targeting (Arduino Pro Mini), and select the programmer that is being used (this should be automatic).
Now you can upload the code, and you should see lights flash on the programmer while it is being uploaded.

## Editing the Code
There are several variables that can be easily changed to suit different needs.
`TRIGGER_DISTANCE` is the distance (in cm) at which the LED will be triggered.
`HISTORY_SIZE` is the number of distances it saves, and the average of these cycles is used to turn on and off the LED. If this is higher, the LED will take longer to turn on and longer to turn off.
`LED_PIN` is the pin the LED is connected to.
`LIDAR_CONFIGURATION` is a Garmin variable that can be changed according to the comments in the code.
