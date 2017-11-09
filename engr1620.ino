/* vim: set ft=cpp ts=2 sw=2 : */
#include <Wire.h>
#include <LIDARLite.h>
#include <stdlib.h>
LIDARLite lidar;

const uint8_t LED_PIN = 3;

const float TRIGGER_DISTANCE = 650; // in centimeters

const uint8_t HISTORY_SIZE = 200; // cycles to save
float hist[HISTORY_SIZE] = {0}; // {0} zeros the array

const uint8_t CYCLES_PER_BIAS_CORRECTION = 100;
bool bias_correction;

using cycle_type = uint16_t; // limit for cycles is 2^16 - 1; slower than uint8_t but less error prone
cycle_type cycles;

constexpr cycle_type gcd(const cycle_type a, const cycle_type b) {
    return b ? gcd(b, a%b) : a;
}
constexpr cycle_type lcm(const cycle_type a, const cycle_type b) {
    return abs(a * b) / gcd(abs(a), abs(b));
}
constexpr cycle_type CYCLE_RESET = lcm(CYCLES_PER_BIAS_CORRECTION, HISTORY_SIZE);
// cycle number at which to reset the cycle count
// note: must be less than limit of cycle_type

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
  // TODO: Do not get average every cycle

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
