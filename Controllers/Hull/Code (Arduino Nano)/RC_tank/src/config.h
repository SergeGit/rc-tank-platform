// ========================
// File: config.h
// ========================
#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// I2C
#define I2C_ADDRESS 0x08

// Pins
#define PIN_BATT_VOLTAGE A0
#define PIN_BATT_RELAY A1

#define PIN_FRONT_LIGHT A2
#define PIN_LASER A3
#define PIN_TURRET_ELEV_IN1 4
#define PIN_TURRET_ROT_EN 5
#define PIN_TURRET_ELEV_EN 6
#define PIN_TURRET_ROT_IN2 7
#define PIN_TURRET_ROT_IN1 8
#define PIN_LEFT_PWM1 3
#define PIN_LEFT_PWM2 9
#define PIN_RIGHT_PWM1 10
#define PIN_RIGHT_PWM2 11
#define PIN_TURRET_ELEV_IN2 12
#define PIN_CANNON_FIRE 13
#define PIN_CANNON_FEEDBACK 2
#define PIN_REAR_LIGHT A7
#define PIN_IR 6

// Timing
#define BATTERY_CHECK_INTERVAL 2000UL
#define MOTOR_UPDATE_INTERVAL 20UL
#define LIGHT_BLINK_INTERVAL 500UL
#define CANNON_COOLDOWN 2000UL

// Motor control
#define DEFAULT_RAMP_STEP 5
#define TRACK_MIN_THRESHOLD 20
#define TURRET_MIN_THRESHOLD 25

// Voltage divider
#define R1 100000.0f
#define R2 22000.0f
#define ADC_REF_V 5.0f

// Limits
#define I2C_MAX_RESPONSE 32

#endif // CONFIG_H

// ========================================
// End of file: config.h