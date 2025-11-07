# Hull Controller Implementation Guide

## 1. Overview
This document provides a step-by-step implementation plan for the **RC Tank Hull Controller**, now updated with **diagnostics integration**, **I2C telemetry (0xF1)**, and **hardened motor control**. The firmware runs on an Arduino Nano under PlatformIO, acting as the primary control unit for propulsion, turret, lights, and weapons.

### Key Enhancements
- **Diagnostics system:** Every subsystem provides a diagnostic string summarizing its health and state.
- **Central diagnostics manager:** `diagnostics.cpp` aggregates module data and exposes it via Serial or I2C.
- **New I2C command (0xF1):** Allows the Raspberry Pi or any I2C master to request a short diagnostic report.
- **Hardened motor control:** Supports both L293D and Cytron drivers, with dynamic voltage compensation using battery feedback.

---

## 2. Module Architecture
Each subsystem is implemented as a separate module with a `.cpp` and `.h` pair:

| Module | Purpose | Key Functions |
|---------|----------|----------------|
| **main.cpp / main.h** | Core logic, I2C command handling, system state control | `processCommand()`, `prepareDiagnosticsResponse()` |
| **config.h** | Pin mappings and constants | All hardware definitions |
| **bms.cpp / bms.h** | Battery voltage monitoring and safety | `updateBatteryVoltage()`, `prepareBatteryResponse()`, `getBmsDiagnosticString()` |
| **motor_control.cpp / motor_control.h** | Motor registration, ramping, and diagnostics | `setMotorSpeed()`, `updateMotors()`, `printAllMotorDiagnostics()` |
| **tracks.cpp / tracks.h** | Differential drive control | `handleTracksMove()`, `getTracksDiagnosticString()` |
| **turret.cpp / turret.h** | Rotation and elevation motor control | `handleTurretRotate()`, `getTurretDiagnosticString()` |
| **weapons.cpp / weapons.h** | Cannon, laser, and IR management | `handleCannonFire()`, `getWeaponsDiagnosticString()` |
| **lights.cpp / lights.h** | Front/rear lights with blink modes | `handleFrontLights()`, `getLightsDiagnosticString()` |
| **diagnostics.cpp / diagnostics.h** | Aggregated system diagnostics | `printSystemDiagnostics()`, `getSystemDiagnosticsSummary()` |


### 2.1 File Layout (PlatformIO)
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
 ├── diagnostics.cpp / diagnostics.h
```
Each module is responsible for setup, update, and specific command handling.

---

## 3. Implementation Steps

### Step 1: Configure Hardware
- Define all pin mappings in `config.h` according to your wiring.
- Ensure I2C address `0x08` matches the Raspberry Pi master configuration.

### Step 2: Initialize Subsystems
In `setup()`, initialize all modules:
```cpp
setupBatteryManagement();
setupMotorControl();
setupTracks();
setupTurret();
setupWeapons();
setupLights();
setupDiagnostics();
```

### Step 3: Handle Commands
The `processCommand()` function routes incoming I2C commands to the correct handlers. New addition:
```cpp
case CMD_GET_DIAGNOSTICS:
  prepareDiagnosticsResponse();
  break;
```

### Step 4: Diagnostics Integration
Each subsystem implements two new functions:
```cpp
String get<Module>DiagnosticString();
void print<Module>Diagnostics();
```
These return concise text for I2C and full detail for Serial.

The central diagnostics manager (`diagnostics.cpp`) provides:
```cpp
String getSystemDiagnosticsSummary(); // Short I2C-safe text
void printSystemDiagnostics();         // Full serial printout
```

### Step 5: I2C Telemetry Command (0xF1)
A new command `CMD_GET_DIAGNOSTICS (0xF1)` allows remote systems to request the 32-byte diagnostic summary.

Example from Raspberry Pi:
```python
bus.write_byte(0x08, 0xF1)
data = bus.read_i2c_block_data(0x08, 0, 32)
print(''.join(chr(x) for x in data))
```
Expected output:
```
Batt=12.3V Tracks:L=10 R=12 Weapons:L=1 I=0 C=READY
```

### Step 6: Testing and Validation
- **Serial Diagnostics:** Call `printSystemDiagnostics()` to print module status.
- **I2C Diagnostics:** Use `CMD_GET_DIAGNOSTICS` to request a short summary string.
- **Error Simulation:** Disconnect sensors to confirm `STATE_ERROR` and `STATE_EMERGENCY_STOP` behavior.

### Step 7: Motor Control Hardening
- Cytron motor support (PWM + DIR).
- Battery voltage compensation adjusts motor speed using measured `batteryVoltage`.
- Ramp-based acceleration prevents jerky movement.

---

## 4. Testing Workflow
1. **Compile and flash** using PlatformIO (`board = nanoatmega328`).
2. **Connect via Serial Monitor** (115200 baud) and verify system startup logs.
3. **Test I2C commands** individually:
   - `0xE0` → Battery data
   - `0xF0` → System status
   - `0xF1` → Diagnostics summary
4. **Confirm ramping and voltage compensation** under varying supply levels.
5. **Trigger emergency stop** (`0xFF 0x00`) and verify full system halt.

---

## 5. Future Enhancements
| Planned Feature | Description |
|------------------|-------------|
| **EEPROM error logging** | Persistent log of last N errors for post-test review. |
| **Watchdog integration** | Reset protection during communication lockup. |
| **Diagnostics expansion** | Add temperature, relay health, and uptime. |
| **Extended I2C frames** | Support multi-block diagnostic packets. |

---

## 6. Summary
This updated guide reflects the modernized **Hull Controller architecture**, emphasizing reliability, observability, and diagnostic access. Each module now self-reports its operational state, enabling fast debugging both locally (via Serial) and remotely (via I2C telemetry).
