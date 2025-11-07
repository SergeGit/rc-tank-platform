// ========================
// File: bms.cpp
// ========================
#include "bms.h"
#include "config.h"
#include "main.h"

float batteryVoltage = 0.0f;
bool isRelayEnabled = false;

static uint16_t battSamples[10];
static uint8_t battSampleIdx = 0;
static bool battSamplesFull = false;

void setupBatteryManagement() {
  pinMode(PIN_BATT_RELAY, OUTPUT);
  digitalWrite(PIN_BATT_RELAY, LOW);
}

float updateBatteryVoltage() {
  uint16_t raw = analogRead(PIN_BATT_VOLTAGE);
  battSamples[battSampleIdx++] = raw;
  if (battSampleIdx >= 10) { battSampleIdx = 0; battSamplesFull = true; }

  uint32_t sum = 0;
  uint8_t count = battSamplesFull ? 10 : battSampleIdx;
  if (count == 0) count = 1;
  for (uint8_t i=0;i<count;i++) sum += battSamples[i];
  float avg = (float)sum / (float)count;
  float voltage = (avg / 1023.0f) * ADC_REF_V * ((R1 + R2) / R2);
  batteryVoltage = voltage;
  return voltage;
}

void updateBatteryManagement() {
  updateBatteryVoltage();
  // simple safeguard
  if (batteryVoltage < 6.0f) {
    emergencyStop();
  }
}

void prepareBatteryResponse() {
  uint16_t v100 = (uint16_t)(batteryVoltage * 100.0f);
  responseBuffer[0] = v100 & 0xFF;
  responseBuffer[1] = (v100 >> 8) & 0xFF;
  responseBuffer[2] = 0; // battery type placeholder
  responseBuffer[3] = isRelayEnabled ? 1 : 0;
  responseBuffer[4] = 0; // reserved
  responseLength = 5;
}

String getBmsDiagnosticString() {
  String s = "Batt=" + String(batteryVoltage,2) + "V";
  s += isRelayEnabled ? " R=ON" : " R=OFF";
  return s;
}

void printBmsDiagnostics() {
  Serial.println(F("--- BMS ---"));
  Serial.print(F("Battery Voltage: ")); Serial.println(batteryVoltage);
  Serial.print(F("Relay Enabled: ")); Serial.println(isRelayEnabled ? "YES" : "NO");
}

// ========================================
// End of file: bms.cpp