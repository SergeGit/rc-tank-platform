// ========================
// File: lights.h
// ========================
#ifndef LIGHTS_H
#define LIGHTS_H

void setupLights();
void updateLights();
void handleFrontLights(uint8_t data);
void handleRearLights(uint8_t data);
uint8_t getFrontLightState();
uint8_t getRearLightState();
String getLightsDiagnosticString();
void printLightsDiagnostics();

#endif // LIGHTS_H

// ========================================
// End of file: lights.h