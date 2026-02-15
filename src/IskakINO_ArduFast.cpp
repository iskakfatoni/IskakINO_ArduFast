#include "IskakINO_ArduFast.h"

// 1. Constructor: Inisialisasi semua timer ke 0
IskakINO_ArduFast::IskakINO_ArduFast() {
    for (int i = 0; i < 10; i++) {
        _prevMillis[i] = 0;
    }
}

// 2. Memulai Serial
void IskakINO_ArduFast::begin(unsigned long baud) {
    Serial.begin(baud);
}

// 3. Engine Multitasking (Non-Blocking)
bool IskakINO_ArduFast::every(unsigned long interval, uint8_t id) {
    if (id >= 10) return false;
    unsigned long current = millis();
    if (current - _prevMillis[id] >= interval) {
        _prevMillis[id] = current;
        return true;
    }
    return false;
}

// 4. Analog Read yang sudah Dinormalisasi (0-1023)
int IskakINO_ArduFast::readNorm(uint8_t pin) {
    int val = analogRead(pin);
    #if defined(ESP32)
        return val >> 2; // Konversi 12-bit ke 10-bit
    #else
        return val;      // AVR & ESP8266 sudah 10-bit
    #endif
}

// 5. Analog Read Stabil (Oversampling)
int IskakINO_ArduFast::readStable(uint8_t pin, uint8_t samples) {
    uint32_t sum = 0;
    for (uint8_t i = 0; i < samples; i++) {
        sum += readNorm(pin);
    }
    return (int)(sum / samples);
}

// 6. Sistem Logging Efisien (Flash Memory)
void IskakINO_ArduFast::log(const __FlashStringHelper* msg, int val) {
    Serial.print(F("[LOG] "));
    Serial.print(msg);
    if (val != -32768) {
        Serial.print(F(": "));
        Serial.print(val);
    }
    Serial.println();
}

// 7. Instansiasi Objek Global agar bisa dipanggil sebagai 'ArduFast'
IskakINO_ArduFast ArduFast;
