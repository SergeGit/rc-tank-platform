// ========================
// File: lights.cpp
// ========================
#include "lights.h"
#include "config.h"

static uint8_t frontState = 0; // 0 off,1 on,2 blink
static uint8_t rearState = 0;
static unsigned long lastBlinkToggle = 0;
static bool blinkOn = false;

void setupLights() {
  pinMode(PIN_FRONT_LIGHT, OUTPUT);
  pinMode(PIN_REAR_LIGHT, OUTPUT);
  digitalWrite(PIN_FRONT_LIGHT, LOW);
  digitalWrite(PIN_REAR_LIGHT, LOW);
}

void updateLights() {
  unsigned long now = millis();
  if (frontState == 2 || rearState == 2) {
    if (now - lastBlinkToggle >= LIGHT_BLINK_INTERVAL) {
      blinkOn = !blinkOn;
      lastBlinkToggle = now;
      if (frontState == 2) digitalWrite(PIN_FRONT_LIGHT, blinkOn ? HIGH : LOW);
      if (rearState == 2) digitalWrite(PIN_REAR_LIGHT, blinkOn ? HIGH : LOW);
    }
  }
}

void handleFrontLights(uint8_t data) {
  frontState = data;
  if (frontState == 0) digitalWrite(PIN_FRONT_LIGHT, LOW);
  else if (frontState == 1) digitalWrite(PIN_FRONT_LIGHT, HIGH);
}

void handleRearLights(uint8_t data) {
  rearState = data;
  if (rearState == 0) digitalWrite(PIN_REAR_LIGHT, LOW);
  else if (rearState == 1) digitalWrite(PIN_REAR_LIGHT, HIGH);
}

uint8_t getFrontLightState() { return frontState; }
uint8_t getRearLightState() { return rearState; }

String getLightsDiagnosticString() {
  String s = "Lights: F=" + String(frontState) + " R=" + String(rearState);
  return s;
}

void printLightsDiagnostics() {
  Serial.println(F("--- Lights ---"));
  Serial.println(getLightsDiagnosticString());
}

// ========================================
// End of file: lights.cpp