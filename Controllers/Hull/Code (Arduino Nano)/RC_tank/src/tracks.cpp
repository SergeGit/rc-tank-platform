// ========================
// File: tracks.cpp
// ========================
#include "tracks.h"
#include "motor_control.h"
#include "config.h"

static byte motorLeft = 0xFF;
static byte motorRight = 0xFF;

void setupTracks() {
  // assume motor_control already setup in main
  motorLeft = registerL293DMotor(PIN_LEFT_PWM1, PIN_LEFT_PWM2, PIN_LEFT_PWM1, "LeftTrack", TRACK_MIN_THRESHOLD);
  motorRight = registerL293DMotor(PIN_RIGHT_PWM1, PIN_RIGHT_PWM2, PIN_RIGHT_PWM1, "RightTrack", TRACK_MIN_THRESHOLD);
}

static int mapDataToSpeed(uint8_t data) {
  int s = (int)data - 100; // map 0-200 -> -100..100
  return s;
}

void handleTracksMove(uint8_t data) {
  int fb = mapDataToSpeed(data);
  setArcadeDriveInput(fb, 0);
}

void handleTracksTurn(uint8_t data) {
  int lr = mapDataToSpeed(data);
  setArcadeDriveInput(0, lr);
}

void setArcadeDriveInput(int forwardBackward, int leftRight) {
  int left = forwardBackward + leftRight;
  int right = forwardBackward - leftRight;
  trackdrive(left, right);
}

void trackdrive(int leftSpeed, int rightSpeed) {
  leftSpeed = constrain(leftSpeed, -100, 100);
  rightSpeed = constrain(rightSpeed, -100, 100);
  if (motorLeft != 0xFF) setMotorSpeed(motorLeft, leftSpeed);
  if (motorRight != 0xFF) setMotorSpeed(motorRight, rightSpeed);
}

void emergencyStopTracks() {
  if (motorLeft != 0xFF) stopMotor(motorLeft);
  if (motorRight != 0xFF) stopMotor(motorRight);
}

String getTracksDiagnosticString() {
  String s = "Tracks:";
  if (motorLeft != 0xFF) s += " L=" + String(getMotorSpeed(motorLeft));
  if (motorRight != 0xFF) s += " R=" + String(getMotorSpeed(motorRight));
  return s;
}

void printTracksDiagnostics() {
  Serial.println(F("--- Tracks ---"));
  Serial.println(getTracksDiagnosticString());
}

// ========================================
// End of file: tracks.cpp