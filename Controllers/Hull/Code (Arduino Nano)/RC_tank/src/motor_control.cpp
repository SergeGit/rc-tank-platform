// ========================
// File: motor_control.cpp
// ========================
#include "motor_control.h"
#include "config.h"
#include "bms.h"
#include <Arduino.h>

struct Motor {
  bool used = false;
  MotorType type;
  uint8_t enablePin;
  uint8_t in1;
  uint8_t in2;
  int currentSpeed = 0; // -100..100
  int targetSpeed = 0;
  uint8_t rampStep = DEFAULT_RAMP_STEP;
  uint8_t minThreshold = 0;
  String name;
};

static Motor motors[MAX_MOTORS];

// Forward declarations
static int scale100to255(int v);
static void applyMotorOutput(byte i, int pwmValue, bool forward);

// ========================
// Registration
// ========================
byte registerL293DMotor(uint8_t enablePin, uint8_t in1, uint8_t in2, const char* name, uint8_t minThreshold) {
  for (byte i=0;i<MAX_MOTORS;i++) {
    if (!motors[i].used) {
      motors[i].used = true;
      motors[i].type = MT_L293D;
      motors[i].enablePin = enablePin;
      motors[i].in1 = in1;
      motors[i].in2 = in2;
      motors[i].currentSpeed = 0;
      motors[i].targetSpeed = 0;
      motors[i].minThreshold = minThreshold;
      motors[i].name = name;
      pinMode(enablePin, OUTPUT);
      pinMode(in1, OUTPUT);
      pinMode(in2, OUTPUT);
      analogWrite(enablePin, 0);
      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);
      return i;
    }
  }
  return 0xFF;
}

byte registerCytronMotor(uint8_t pwmPin, uint8_t dirPin, const char* name, uint8_t minThreshold) {
  for (byte i=0;i<MAX_MOTORS;i++) {
    if (!motors[i].used) {
      motors[i].used = true;
      motors[i].type = MT_CYTRON;
      motors[i].enablePin = pwmPin;
      motors[i].in1 = dirPin; // reusing in1 as DIR pin
      motors[i].in2 = 0;
      motors[i].currentSpeed = 0;
      motors[i].targetSpeed = 0;
      motors[i].minThreshold = minThreshold;
      motors[i].name = name;
      pinMode(pwmPin, OUTPUT);
      pinMode(dirPin, OUTPUT);
      analogWrite(pwmPin, 0);
      digitalWrite(dirPin, LOW);
      return i;
    }
  }
  return 0xFF;
}

void setupMotorControl() {
  for (byte i=0;i<MAX_MOTORS;i++) motors[i].used = false;
}

// ========================
// Utility Functions
// ========================
static int scale100to255(int v) {
  int s = constrain(v, 0, 100);
  return (s * 255) / 100;
}

static void applyMotorOutput(byte i, int pwmValue, bool forward) {
  if (motors[i].type == MT_L293D) {
    digitalWrite(motors[i].in1, forward ? HIGH : LOW);
    digitalWrite(motors[i].in2, forward ? LOW : HIGH);
    analogWrite(motors[i].enablePin, pwmValue);
  } else if (motors[i].type == MT_CYTRON) {
    digitalWrite(motors[i].in1, forward ? HIGH : LOW); // DIR
    analogWrite(motors[i].enablePin, pwmValue);
  }
}

// ========================
// Control Functions
// ========================
void setMotorSpeed(byte motorIndex, int speed) {
  if (motorIndex >= MAX_MOTORS) return;
  if (!motors[motorIndex].used) return;
  motors[motorIndex].targetSpeed = constrain(speed, -100, 100);
}

void stopMotor(byte motorIndex) {
  if (motorIndex >= MAX_MOTORS) return;
  if (!motors[motorIndex].used) return;
  motors[motorIndex].targetSpeed = 0;
  motors[motorIndex].currentSpeed = 0;
  analogWrite(motors[motorIndex].enablePin, 0);
  if (motors[motorIndex].type == MT_L293D) {
    digitalWrite(motors[motorIndex].in1, LOW);
    digitalWrite(motors[motorIndex].in2, LOW);
  } else if (motors[motorIndex].type == MT_CYTRON) {
    digitalWrite(motors[motorIndex].in1, LOW);
  }
}

void stopAllMotors() {
  for (byte i=0;i<MAX_MOTORS;i++) if (motors[i].used) stopMotor(i);
}

// ========================
// Motor Update Loop
// ========================
void updateMotors() {
  float voltageCompFactor = 1.0f;
  if (batteryVoltage > 0.1f) {
    float nominalVoltage = 12.0f;
    voltageCompFactor = nominalVoltage / batteryVoltage;
    voltageCompFactor = constrain(voltageCompFactor, 0.8f, 1.3f);
  }

  for (byte i=0;i<MAX_MOTORS;i++) {
    if (!motors[i].used) continue;
    int cur = motors[i].currentSpeed;
    int tgt = motors[i].targetSpeed;

    // ramp control
    if (abs(tgt - cur) <= motors[i].rampStep) cur = tgt;
    else if (tgt > cur) cur += motors[i].rampStep;
    else cur -= motors[i].rampStep;
    motors[i].currentSpeed = cur;

    int absSpeed = abs(cur);
    if (absSpeed < motors[i].minThreshold && cur != 0) absSpeed = 0;

    // Apply voltage compensation
    absSpeed = (int)((float)absSpeed * voltageCompFactor);
    absSpeed = constrain(absSpeed, 0, 100);

    int pwmValue = scale100to255(absSpeed);
    bool forward = (cur >= 0);

    applyMotorOutput(i, pwmValue, forward);
  }
}

// ========================
// Diagnostics and Telemetry
// ========================
int getMotorSpeed(byte motorIndex) {
  if (motorIndex>=MAX_MOTORS) return 0;
  return motors[motorIndex].currentSpeed;
}

int getMotorTargetSpeed(byte motorIndex) {
  if (motorIndex>=MAX_MOTORS) return 0;
  return motors[motorIndex].targetSpeed;
}

String getMotorDiagnostic(byte motorIndex) {
  if (motorIndex>=MAX_MOTORS || !motors[motorIndex].used) return String("Invalid Motor");
  String diag = String(motors[motorIndex].name) + ": Curr=" + String(motors[motorIndex].currentSpeed) + ", Target=" + String(motors[motorIndex].targetSpeed);
  diag += ", Type=" + String(motors[motorIndex].type == MT_L293D ? "L293D" : "Cytron");
  return diag;
}

void printAllMotorDiagnostics() {
  Serial.println(F("--- Motor Diagnostics ---"));
  for (byte i=0;i<MAX_MOTORS;i++) {
    if (motors[i].used) Serial.println(getMotorDiagnostic(i));
  }
}

// ========================================
// End of file: motor_control.cpp