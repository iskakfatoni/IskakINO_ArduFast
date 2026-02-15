#ifndef ISKAKINO_ARDUFAST_H
#define ISKAKINO_ARDUFAST_H

#include <Arduino.h>

/**
 * IskakINO-ArduFast Framework
 * Optimized I/O and Task Management for AVR, ESP8266, & ESP32
 */

// --- 1. FastPin Template (Ultra Fast Digital I/O) ---
template <uint8_t P>
class FastPin {
public:
    void mode(uint8_t m) {
        pinMode(P, m);
    }

    void high() {
        digitalWrite(P, HIGH);
    }

    void low() {
        digitalWrite(P, LOW);
    }

    void toggle() {
        digitalWrite(P, !digitalRead(P));
    }

    bool read() {
        return digitalRead(P);
    }
};

// --- 2. Main Framework Class ---
class IskakINO_ArduFast {
private:
    unsigned long _prevMillis[10]; // Kapasitas 10 Task (bisa ditambah)

public:
    IskakINO_ArduFast() {
        for (int i = 0; i < 10; i++) _prevMillis[i] = 0;
    }

    // Inisialisasi Serial & System
    void begin(unsigned long baud = 115200) {
        Serial.begin(baud);
    }

    // Multi-tasking Engine
    bool every(unsigned long interval, uint8_t id) {
        if (id >= 10) return false;
        unsigned long current = millis();
        if (current - _prevMillis[id] >= interval) {
            _prevMillis[id] = current;
            return true;
        }
        return false;
    }

    // Smart Analog (Normalized 0-1023)
    int readNorm(uint8_t pin) {
        int val = analogRead(pin);
        #if defined(ESP32)
            return val >> 2; // 12-bit to 10-bit
        #else
            return val;      // AVR/ESP8266 is already 10-bit
        #endif
    }

    // Stable Analog with Oversampling
    int readStable(uint8_t pin, uint8_t samples = 16) {
        uint32_t sum = 0;
        for (uint8_t i = 0; i < samples; i++) {
            sum += readNorm(pin);
        }
        return (int)(sum / samples);
    }

    // Flash-Efficient Logging
    void log(const __FlashStringHelper* msg, int val = -32768) {
        Serial.print(F("[LOG] "));
        Serial.print(msg);
        if (val != -32768) {
            Serial.print(F(": "));
            Serial.print(val);
        }
        Serial.println();
    }
};

// Deklarasi instance agar bisa dipakai di .ino (extern)
extern IskakINO_ArduFast ArduFast;

#endif // ISKAKINO_ARDUFAST_H
