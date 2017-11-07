#include <Wire.h>
#include <LIDARLite.h>

LIDARLite lidar;

const uint8_t LED_PIN = 6;

const float TRIGGER_DISTANCE = 50; // in centimeters

const uint8_t CYCLES_PER_BIAS_CORRECTION = 100; // every 100 trials, run a bias correction
uint8_t cycles;
bool bias_correction;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); // Initialize serial connection to display distance readings

  // LIDAR over I2C interface

  lidar.begin(0, true); // Set configuration to default and I2C to 400 kHz


  /* From LIDAR Lite v3 GitHub examples
    configure(int configuration, char lidarliteAddress)
    Selects one of several preset configurations.
    Parameters
    ----------------------------------------------------------------------------
    configuration:  Default 0.
      0: Default mode, balanced performance.
      1: Short range, high speed. Uses 0x1d maximum acquisition count.
      2: Default range, higher speed short range. Turns on quick termination
          detection for faster measurements at short range (with decreased
          accuracy)
      3: Maximum range. Uses 0xff maximum acquisition count.
      4: High sensitivity detection. Overrides default valid measurement detection
          algorithm, and uses a threshold value for high sensitivity and noise.
      5: Low sensitivity detection. Overrides default valid measurement detection
          algorithm, and uses a threshold value for low sensitivity and noise.
    lidarliteAddress: Default 0x62. Fill in new address here if changed. See
      operating manual for instructions.
  */

  lidar.configure(0); // Change this number to try out alternate configurations

  pinMode(LED_PIN, OUTPUT);

  digitalWrite(LED_PIN, LOW);

  cycles = 0;
  bias_correction = true; // always start with a corrected result

}

void lightLED(bool on) {
  digitalWrite(LED_PIN, on ? HIGH : LOW);
}

void loop() {
  // put your main code here, to run repeatedly:

  float dist = lidar.distance(bias_correction);
  lightLED(dist > TRIGGER_DISTANCE);
  if(++cycles > CYCLES_PER_BIAS_CORRECTION) {
    cycles = 0;
    bias_correction = true;
  } else {
    bias_correction = false;
  }

}
