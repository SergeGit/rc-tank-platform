// ========================
// File: diagnostics.cpp
// ========================
#include "diagnostics.h"
#include "bms.h"
#include "motor_control.h"
#include "tracks.h"
#include "turret.h"
#include "weapons.h"
#include "lights.h"
#include <Arduino.h>

void setupDiagnostics() {
  // placeholder for any initialization
}

void printSystemDiagnostics() {
  Serial.println(F("===== SYSTEM DIAGNOSTICS ====="));
  printBmsDiagnostics();
  printAllMotorDiagnostics();
  printTracksDiagnostics();
  printTurretDiagnostics();
  printWeaponsDiagnostics();
  printLightsDiagnostics();
  Serial.println(F("=============================="));
}

String getSystemDiagnosticsSummary() {
  // produce a short, single-line summary suitable for I2C (<=32 bytes)
  String s = getBmsDiagnosticString();
  s += " ";
  s += getTracksDiagnosticString();
  s += " ";
  s += getWeaponsDiagnosticString();

  // truncate to fit
  if (s.length() >= I2C_MAX_RESPONSE) s = s.substring(0, I2C_MAX_RESPONSE - 1);
  return s;
}

// ========================================
// End of file: diagnostics.cpp