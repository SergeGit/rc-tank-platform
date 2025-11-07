// ========================
// File: turret.h
// ========================
#ifndef TURRET_H
#define TURRET_H

void setupTurret();
void handleTurretRotate(uint8_t data);
void handleTurretElevate(uint8_t data);
void stopTurret();
String getTurretDiagnosticString();
void printTurretDiagnostics();

#endif // TURRET_H

// ========================================
// End of file: turret.h