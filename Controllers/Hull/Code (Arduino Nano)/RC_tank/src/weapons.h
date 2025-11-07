// ========================
// File: weapons.h
// ========================
#ifndef WEAPONS_H
#define WEAPONS_H

void setupWeapons();
void updateWeapons();
void handleLaserToggle(uint8_t data);
void handleIrToggle(uint8_t data);
void handleCannonFire(uint8_t duration);
bool getIsCannonReady();
void disableWeapons();
String getWeaponsDiagnosticString();
void printWeaponsDiagnostics();

#endif // WEAPONS_H

// ========================================
// End of file: weapons.h