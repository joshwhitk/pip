/*
 * bobross — gently nod a Bob Ross bobblehead with a hobby servo.
 *
 * The head rocks ±AMPLITUDE_DEG around CENTER_DEG following a sine wave,
 * one full cycle (nod down, up, back to center) per PERIOD_MS. The motion
 * is small, smooth, and endless — a happy little bobble.
 *
 * Board:   ESP32-S3 (over USB)
 * Servo:   signal wire on GPIO 13, powered from 5V with a common ground
 * Library: ESP32Servo  (Arduino IDE: Tools > Manage Libraries… > "ESP32Servo")
 */

#include <ESP32Servo.h>
#include <math.h>

// ---- Tunables ---------------------------------------------------------------
const int   SERVO_PIN     = 13;      // signal pin the servo is wired to
const float CENTER_DEG    = 90.0f;   // resting / mid position of the head
const float AMPLITUDE_DEG = 5.0f;    // swing each way from center (±5°).
                                     //   Set to 2.5 for a 5° peak-to-peak nod.
const float PERIOD_MS     = 1000.0f; // one full bob per second (1 Hz)
const int   UPDATE_MS     = 20;      // control loop / servo refresh (50 Hz)

// Pulse-width endpoints for a typical hobby servo (µs at 0° and 180°).
// Widen/narrow these if your servo under- or over-travels.
const int   PULSE_MIN_US  = 500;
const int   PULSE_MAX_US  = 2500;
// -----------------------------------------------------------------------------

Servo headServo;

// Map an angle in degrees to a servo pulse width in microseconds. Driving the
// servo by pulse width (rather than integer degrees) keeps the tiny ±5° motion
// smooth instead of stair-stepping through only a handful of whole degrees.
int angleToMicros(float deg) {
  const float span = PULSE_MAX_US - PULSE_MIN_US;
  return (int)(PULSE_MIN_US + (deg / 180.0f) * span + 0.5f);
}

void setup() {
  // ESP32Servo needs a hardware timer allocated before attach().
  ESP32PWM::allocateTimer(0);
  headServo.setPeriodHertz(50);                         // standard 50 Hz frame
  headServo.attach(SERVO_PIN, PULSE_MIN_US, PULSE_MAX_US);
  headServo.write(CENTER_DEG);                          // start centered
}

void loop() {
  // Derive the phase (0..1) from the wall clock so the 1 s cycle stays accurate
  // regardless of how long the loop body takes.
  const float phase = (millis() % (unsigned long)PERIOD_MS) / PERIOD_MS;
  const float angle = CENTER_DEG + AMPLITUDE_DEG * sinf(TWO_PI * phase);
  headServo.writeMicroseconds(angleToMicros(angle));
  delay(UPDATE_MS);
}
