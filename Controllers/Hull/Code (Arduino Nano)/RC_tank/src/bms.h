// ========================
// File: bms.h
// ========================
#ifndef BMS_H
#define BMS_H

void setupBatteryManagement();
void updateBatteryManagement();
float updateBatteryVoltage();
void prepareBatteryResponse();
String getBmsDiagnosticString();
void printBmsDiagnostics();

extern float batteryVoltage;
extern bool isRelayEnabled;

#endif // BMS_H

// ========================================
// End of file: bms.h