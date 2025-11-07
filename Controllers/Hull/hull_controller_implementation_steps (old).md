# Hull Controller – Detailed Implementation Guide

## 1. Project Structure Setup

### 1.1 Create Modular File Layout (PlatformIO)
```
/src
 ├── main.cpp
 ├── config.h
 ├── main.h
 ├── bms.cpp / bms.h
 ├── motor_control.cpp / motor_control.h
 ├── tracks.cpp / tracks.h
 ├── turret.cpp / turret.h
 ├── weapons.cpp / weapons.h
 ├── lights.cpp / lights.h
```
Each module is responsible for setup, update, and specific command handling.

### 1.2 Common Header
`main.h` includes:
```cpp
#include <Arduino.h>
#include <Wire.h>
#include "config.h"
#include "bms.h"
#include "motor_control.h"
#include "tracks.h"
#include "turret.h"
#include "weapons.h"
#include "lights.h"
```
Define system states, command codes, and shared global variables.

---

## 2. Hardware Configuration

### 2.1 Define Pin Mappings
In `config.h`:
- Include all pin mappings (A0–A7, D2–D13)
- Define `#define I2C_ADDRESS 0x08`
- Add motor thresholds, ramp parameters, and timing constants.

### 2.2 Wiring Validation
- Follow wiring table in documentation.
- Verify all grounds are connected.
- Separate logic and motor power.
- Ensure 4.7kΩ pull-ups on SDA/SCL lines.

---

## 3. Firmware Core Implementation

### 3.1 Setup Phase
```cpp
void setup() {
  Wire.begin(I2C_ADDRESS);
  setupBatteryManagement();
  setupMotorControl();
  setupTracks();
  setupTurret();
  setupWeapons();
  setupLights();
  setupWatchdogTimer();
}
```

### 3.2 Loop Phase
```cpp
void loop() {
  updateBatteryManagement();
  updateMotors();
  updateWeapons();
  updateLights();
  checkSystemState();
  wdt_reset();
}
```

---

## 4. I2C Communication

### 4.1 Slave Receive Event
```cpp
void receiveEvent(int howMany) {
  if (howMany >= 2) {
    byte command = Wire.read();
    byte data = Wire.read();
    processCommand(command, data);
  }
}
```

### 4.2 Slave Request Event
```cpp
void requestEvent() {
  Wire.write(responseBuffer, responseLength);
}
```

### 4.3 Register Events
```cpp
Wire.onReceive(receiveEvent);
Wire.onRequest(requestEvent);
```

---

## 5. Command Processing

### 5.1 Main Command Handler
```cpp
switch (cmd) {
  case 0x20: handleTracksMove(data); break;
  case 0x21: handleTracksTurn(data); break;
  case 0x10: handleTurretRotate(data); break;
  case 0x11: handleTurretElevate(data); break;
  case 0x30: handleLaserToggle(data); break;
  case 0x31: handleIrToggle(data); break;
  case 0x32: handleCannonFire(data); break;
  case 0x40: handleFrontLights(data); break;
  case 0x41: handleRearLights(data); break;
  case 0xE0: prepareBatteryResponse(); break;
  case 0xF0: prepareStatusResponse(); break;
  case 0xFF: if (data==0) emergencyStop(); else resumeFromEmergencyStop(); break;
  default: setErrorState(); break;
}
```

### 5.2 Safety State Management
```cpp
void emergencyStop() {
  stopAllMotors();
  disableWeapons();
  setSystemState(STATE_EMERGENCY_STOP);
}

void resumeFromEmergencyStop() {
  setSystemState(STATE_NORMAL);
}
```

---

## 6. Subsystem Implementation Steps

### 6.1 Battery Management (bms.cpp)
- Read voltage from analog input.
- Convert via divider formula.
- Update every 2 seconds.
- Provide `updateBatteryResponseData()` for I2C requests.

### 6.2 Motor Control (motor_control.cpp)
- Implement registration and control of L293D and Cytron motors.
- Include ramping, thresholds, and voltage compensation.
- Periodically call `updateMotors()`.

### 6.3 Tracks (tracks.cpp)
- Control movement via differential drive logic.
- Support both command mode and direct arcade drive.

### 6.4 Turret (turret.cpp)
- Use L293D channels for rotation/elevation.
- Map command byte to signed speed.
- Stop automatically on error or E-stop.

### 6.5 Weapons (weapons.cpp)
- Fire cannon with timed pulse and cooldown.
- Laser and IR toggles via digital pins.
- Implement `getIsCannonReady()` for safety.

### 6.6 Lights (lights.cpp)
- Manage ON/OFF/BLINK states.
- `updateLights()` handles blink timing.

---

## 7. Status and Telemetry

### 7.1 Response Buffer Preparation
```cpp
void prepareStatusResponse() {
  responseBuffer[0] = buildStatusByte();
  responseLength = 1;
}
```

### 7.2 Status Byte Composition
```cpp
byte buildStatusByte() {
  byte status = 0;
  if (getIsCannonReady()) status |= (1 << 0);
  if (getIsLaserOn()) status |= (1 << 1);
  if (getIsIrOn()) status |= (1 << 2);
  if (getFrontLightState()) status |= (1 << 3);
  if (getRearLightState()) status |= (1 << 4);
  if (isRelayEnabled) status |= (1 << 5);
  if (systemError) status |= (1 << 6);
  if (systemState == STATE_EMERGENCY_STOP) status |= (1 << 7);
  return status;
}
```

---

## 8. Testing & Verification

### 8.1 Initial Power-Up
1. Disconnect motors.
2. Verify I2C device at `0x08` (`i2cdetect -y 1`).
3. Confirm battery reading response.

### 8.2 Subsystem Testing
- Lights → Weapons → Tracks → Turret.
- Validate PWM outputs with oscilloscope or LED indicator.

### 8.3 Communication Test
Use Python script from documentation to issue commands and verify replies.

### 8.4 Safety Test
- Issue `Emergency Stop` command and confirm full system halt.
- Disconnect I2C → observe watchdog recovery.

---

## 9. Optional Enhancements

| Enhancement | Description |
|--------------|-------------|
| **Diagnostics output** | Serial print subsystem states for debugging |
| **Battery type detection** | Adjust cutoff voltage automatically |
| **Error logging** | Store last 10 errors in EEPROM |
| **I2C checksum** | Add data integrity validation |
| **Startup self-test** | Spin-test motors, flash LEDs, validate sensors |

