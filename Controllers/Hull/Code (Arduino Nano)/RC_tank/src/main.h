// ========================
// File: main.h
// ========================
#ifndef MAIN_H
#define MAIN_H

#include "config.h"
#include <Wire.h>

// System states
#define STATE_NORMAL 0
#define STATE_ERROR 1
#define STATE_EMERGENCY_STOP 2

// Commands
#define CMD_TURRET_ROTATE 0x10
#define CMD_TURRET_ELEVATE 0x11
#define CMD_TRACKS_MOVE 0x20
#define CMD_TRACKS_TURN 0x21
#define CMD_LASER_TOGGLE 0x30
#define CMD_IR_TOGGLE 0x31
#define CMD_CANNON_FIRE 0x32
#define CMD_FRONT_LIGHTS 0x40
#define CMD_REAR_LIGHTS 0x41
#define CMD_GET_BATTERY 0xE0
#define CMD_GET_STATUS 0xF0
#define CMD_GET_DIAGNOSTICS 0xF1
#define CMD_EMERGENCY_STOP 0xFF

// Response buffer
extern uint8_t responseBuffer[I2C_MAX_RESPONSE];
extern uint8_t responseLength;

void processCommand(uint8_t command, uint8_t data);
void prepareResponseData(uint8_t command);
void prepareDiagnosticsResponse();
void emergencyStop();
void resumeFromEmergencyStop();
void setSystemError();

extern volatile uint8_t systemState;

#endif // MAIN_H

// ========================================
// End of file: main.h