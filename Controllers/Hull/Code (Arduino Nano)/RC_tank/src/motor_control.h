// ========================
// File: motor_control.h
// ========================
#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

#include <Arduino.h>

#define MAX_MOTORS 8

typedef enum {MT_L293D, MT_CYTRON} MotorType;

byte registerL293DMotor(uint8_t enablePin, uint8_t in1, uint8_t in2, const char* name, uint8_t minThreshold);
byte registerCytronMotor(uint8_t pwmPin, uint8_t dirPin, const char* name, uint8_t minThreshold);
void setupMotorControl();
void setMotorSpeed(byte motorIndex, int speed); // -100..100
void stopMotor(byte motorIndex);
void stopAllMotors();
void updateMotors();
int getMotorSpeed(byte motorIndex);
int getMotorTargetSpeed(byte motorIndex);
String getMotorDiagnostic(byte motorIndex);
void printAllMotorDiagnostics();

#endif // MOTOR_CONTROL_H

// ========================================
// End of file: motor_control.h