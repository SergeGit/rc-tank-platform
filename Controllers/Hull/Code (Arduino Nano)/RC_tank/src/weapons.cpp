// ========================
// File: weapons.cpp
// ========================
#include "weapons.h"
#include "config.h"
#include "main.h"

static bool laserOn = false;
static bool irOn = false;
static bool cannonReady = true;
static unsigned long cannonLastFired = 0;
static unsigned long cannonDuration = 0;
static bool cannonActive = false;

void setupWeapons() {
  pinMode(PIN_LASER, OUTPUT);
  pinMode(PIN_IR, OUTPUT);
  pinMode(PIN_CANNON_FIRE, OUTPUT);
  digitalWrite(PIN_LASER, LOW);
  digitalWrite(PIN_IR, LOW);
  digitalWrite(PIN_CANNON_FIRE, LOW);
}

void updateWeapons() {
  unsigned long now = millis();
  if (cannonActive) {
    if (now - cannonLastFired >= cannonDuration) {
      digitalWrite(PIN_CANNON_FIRE, LOW);
      cannonActive = false;
      cannonLastFired = now;
      cannonReady = false;
    }
  } else {
    if (!cannonReady && (now - cannonLastFired >= CANNON_COOLDOWN)) {
      cannonReady = true;
    }
  }
}

void handleLaserToggle(uint8_t data) {
  laserOn = (data != 0);
  digitalWrite(PIN_LASER, laserOn ? HIGH : LOW);
}

void handleIrToggle(uint8_t data) {
  irOn = (data != 0);
  digitalWrite(PIN_IR, irOn ? HIGH : LOW);
}

void handleCannonFire(uint8_t duration) {
  if (!cannonReady) return;
  cannonDuration = (uint32_t)duration; // ms
  digitalWrite(PIN_CANNON_FIRE, HIGH);
  cannonActive = true;
  cannonLastFired = millis();
}

bool getIsCannonReady() { return cannonReady; }

void disableWeapons() {
  handleLaserToggle(0);
  handleIrToggle(0);
  digitalWrite(PIN_CANNON_FIRE, LOW);
  cannonActive = false;
}

String getWeaponsDiagnosticString() {
  String s = "Weapons:";
  s += laserOn ? "L=1" : "L=0";
  s += irOn ? " I=1" : " I=0";
  s += cannonReady ? " C=READY" : " C=COOL";
  return s;
}

void printWeaponsDiagnostics() {
  Serial.println(F("--- Weapons ---"));
  Serial.println(getWeaponsDiagnosticString());
}

// ========================================
// End of file: weapons.cpp
