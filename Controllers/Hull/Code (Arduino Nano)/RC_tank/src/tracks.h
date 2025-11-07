// ========================
// File: tracks.h
// ========================
#ifndef TRACKS_H
#define TRACKS_H

void setupTracks();
void handleTracksMove(uint8_t data);
void handleTracksTurn(uint8_t data);
void setArcadeDriveInput(int forwardBackward, int leftRight);
void trackdrive(int leftSpeed, int rightSpeed);
void emergencyStopTracks();
String getTracksDiagnosticString();
void printTracksDiagnostics();

#endif // TRACKS_H

// ========================================
// End of file: tracks.h