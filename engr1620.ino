#include <Wire.h>
#include <LIDARLite.h>
#include <stdlib.h>

LIDARLite lidar;

const uint8_t LED_PIN = 3;

const float TRIGGER_DISTANCE = 650; // in centimeters

const uint8_t HISTORY_SIZE = 200; // cycles to save
float hist[HISTORY_SIZE] = {0}; // {0} zeros the array

const uint8_t CYCLES_PER_BIAS_CORRECTION = 100;
uint8_t cycles;
bool bias_correction;

constexpr uint8_t CYCLE_RESET = 200; // should be the least common multiple of HISTORY_SIZE and CYCLES_PER_BIAS_CORRECTION

const uint8_t LIDAR_CONFIGURATION = 3;
/*
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
*/

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); // Initialize serial connection to display distance readings

  // LIDAR over I2C interface

  lidar.begin(0, true); // Set configuration to default and I2C to 400 kHz

  lidar.configure(LIDAR_CONFIGURATION);

  pinMode(LED_PIN, OUTPUT);

  digitalWrite(LED_PIN, LOW);

  cycles = 0;
  bias_correction = true; // always start with a corrected result

}

void lightLED(bool on) {
  digitalWrite(LED_PIN, on ? HIGH : LOW);
}

float getAverage() {
  register float sum = 0.0;
  register uint8_t i;
  for(i = 0; i < HISTORY_SIZE; ++i)
    sum += hist[i];
  return sum/HISTORY_SIZE;
}

void loop() {
  // put your main code here, to run repeatedly:

  // TODO: Limit cycles to save power

  float dist = lidar.distance(bias_correction);
  hist[cycles % HISTORY_SIZE] = dist; // must wait for HISTORY_SIZE cycles to populate history for good averages -- not a problem
  Serial.println(dist);

  float avg = getAverage();

  lightLED(avg < TRIGGER_DISTANCE);
  if(++cycles % CYCLES_PER_BIAS_CORRECTION == 0) {
    bias_correction = true;
  } else {
    bias_correction = false;
  }

  // Reset cycles to speed up the modulus operation
  if(cycles >= CYCLE_RESET)
    cycles = 0;

}
