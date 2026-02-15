#ifndef ISKAKINO_ARDUFAST_H
#define ISKAKINO_ARDUFAST_H

#include <Arduino.h>

// --- FastPin Template (Tetap di Header karena Template) ---
template <uint8_t P>
class FastPin {
public:
    void mode(uint8_t m) { pinMode(P, m); }
    void high() { digitalWrite(P, HIGH); }
    void low() { digitalWrite(P, LOW); }
    void toggle() { digitalWrite(P, !digitalRead(P)); }
    bool read() { return digitalRead(P); }
};

// --- Class Framework ---
class IskakINO_ArduFast {
private:
    unsigned long _prevMillis[10];

public:
    IskakINO_ArduFast(); // Constructor
    
    void begin(unsigned long baud = 115200);
    bool every(unsigned long interval, uint8_t id);
    int readNorm(uint8_t pin);
    int readStable(uint8_t pin, uint8_t samples = 16);
    void log(const __FlashStringHelper* msg, int val = -32768);
};

// Deklarasi instance agar bisa diakses global
extern IskakINO_ArduFast ArduFast;

#endif
