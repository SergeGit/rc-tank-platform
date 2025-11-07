// ========================
// File: turret.cpp
// ========================
#include "turret.h"
#include "motor_control.h"
#include "config.h"

static byte motorTurretRotate = 0xFF;
static byte motorTurretElev = 0xFF;

void setupTurret() {
  motorTurretRotate = registerL293DMotor(PIN_TURRET_ROT_EN, PIN_TURRET_ROT_IN1, PIN_TURRET_ROT_IN2, "TurretRot", TURRET_MIN_THRESHOLD);
  motorTurretElev = registerL293DMotor(PIN_TURRET_ELEV_EN, PIN_TURRET_ELEV_IN1, PIN_TURRET_ELEV_IN2, "TurretElev", TURRET_MIN_THRESHOLD);
}

static int mapDataToSigned(uint8_t data) { return (int)data - 100; }

void handleTurretRotate(uint8_t data) {
  int s = mapDataToSigned(data);
  if (motorTurretRotate != 0xFF) setMotorSpeed(motorTurretRotate, s);
}

void handleTurretElevate(uint8_t data) {
  int s = mapDataToSigned(data);
  if (motorTurretElev != 0xFF) setMotorSpeed(motorTurretElev, s);
}

void stopTurret() {
  if (motorTurretRotate != 0xFF) stopMotor(motorTurretRotate);
  if (motorTurretElev != 0xFF) stopMotor(motorTurretElev);
}

String getTurretDiagnosticString() {
  String s = "Turret:";
  if (motorTurretRotate != 0xFF) s += " Rot=" + String(getMotorSpeed(motorTurretRotate));
  if (motorTurretElev != 0xFF) s += " Elev=" + String(getMotorSpeed(motorTurretElev));
  return s;
}

void printTurretDiagnostics() {
  Serial.println(F("--- Turret ---"));
  Serial.println(getTurretDiagnosticString());
}

// ========================================
// End of file: turret.cpp