// ========================
// File: main.cpp
// ========================
#include <Arduino.h>
#include <Wire.h>
#include "main.h"
#include "bms.h"
#include "motor_control.h"
#include "tracks.h"
#include "turret.h"
#include "weapons.h"
#include "lights.h"
#include "diagnostics.h"

uint8_t responseBuffer[I2C_MAX_RESPONSE];
uint8_t responseLength = 0;
volatile uint8_t systemState = STATE_NORMAL;

unsigned long lastMotorUpdate = 0;
unsigned long lastBatteryCheck = 0;

void receiveEvent(int howMany) {
  if (howMany >= 2) {
    uint8_t cmd = Wire.read();
    uint8_t data = Wire.read();
    processCommand(cmd, data);
  }
}

void requestEvent() {
  Wire.write(responseBuffer, responseLength);
}

void setup() {
  Serial.begin(115200);
  delay(200);
  Wire.begin(I2C_ADDRESS);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);

  // Subsystems
  setupBatteryManagement();
  setupMotorControl();
  setupTracks();
  setupTurret();
  setupWeapons();
  setupLights();
  setupDiagnostics();

  // basic pin modes
  pinMode(PIN_CANNON_FEEDBACK, INPUT_PULLUP);

  // init response
  responseLength = 0;
}

void loop() {
  unsigned long now = millis();

  if (systemState == STATE_NORMAL) {
    if (now - lastBatteryCheck >= BATTERY_CHECK_INTERVAL) {
      updateBatteryManagement();
      lastBatteryCheck = now;
    }

    if (now - lastMotorUpdate >= MOTOR_UPDATE_INTERVAL) {
      updateMotors();
      lastMotorUpdate = now;
    }

    updateWeapons();
    updateLights();
  }

  // optionally print periodic diagnostics for debug
  // if (now % 10000 < MOTOR_UPDATE_INTERVAL) printAllMotorDiagnostics();
}

void processCommand(uint8_t command, uint8_t data) {
  // Allow resume command while in E-STOP
  if (systemState == STATE_EMERGENCY_STOP && !(command == CMD_EMERGENCY_STOP && data == 1)) {
    return;
  }

  switch (command) {
    case CMD_TRACKS_MOVE: handleTracksMove(data); break;
    case CMD_TRACKS_TURN: handleTracksTurn(data); break;
    case CMD_TURRET_ROTATE: handleTurretRotate(data); break;
    case CMD_TURRET_ELEVATE: handleTurretElevate(data); break;
    case CMD_LASER_TOGGLE: handleLaserToggle(data); break;
    case CMD_IR_TOGGLE: handleIrToggle(data); break;
    case CMD_CANNON_FIRE: handleCannonFire(data); break;
    case CMD_FRONT_LIGHTS: handleFrontLights(data); break;
    case CMD_REAR_LIGHTS: handleRearLights(data); break;
    case CMD_GET_BATTERY: prepareBatteryResponse(); break;
    case CMD_GET_STATUS: prepareStatusResponse(); break;
    case CMD_GET_DIAGNOSTICS: prepareDiagnosticsResponse(); break;
    case CMD_EMERGENCY_STOP:
      if (data == 0) emergencyStop(); else resumeFromEmergencyStop();
      break;
    default:
      setSystemError();
      break;
  }
}

void prepareResponseData(uint8_t command) {
  // Not used - keep for extensibility
  responseLength = 0;
}

void prepareDiagnosticsResponse() {
  String diag = getSystemDiagnosticsSummary();
  if (diag.length() == 0) { responseLength = 0; return; }
  if (diag.length() >= I2C_MAX_RESPONSE) {
    // truncate but ensure null-terminated when converted
    diag = diag.substring(0, I2C_MAX_RESPONSE - 1);
  }
  responseLength = diag.length();
  diag.toCharArray((char*)responseBuffer, responseLength + 1);
}

void emergencyStop() {
  stopAllMotors();
  disableWeapons();
  handleFrontLights(0);
  handleRearLights(0);
  systemState = STATE_EMERGENCY_STOP;
}

void resumeFromEmergencyStop() {
  systemState = STATE_NORMAL;
}

void setSystemError() {
  systemState = STATE_ERROR;
}

// ========================================
// End of file: main.cpp