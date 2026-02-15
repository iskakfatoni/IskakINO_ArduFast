#include "IskakINO_ArduFast.h"

// Definisi instance global
IskakINO_ArduFast ArduFast;

IskakINO::IskakINO() {
    for (uint8_t i = 0; i < 10; i++) _lastMillis[i] = 0;
}

void IskakINO::begin(uint32_t baud) {
    #if ISKAKINO_LOG_LEVEL > LOG_NONE
        Serial.begin(baud);
        log(F("System Initialized"));
    #endif
}

bool IskakINO::every(uint32_t interval, uint8_t id) {
    if (id >= 10) return false;
    uint32_t current = millis();
    if (current - _lastMillis[id] >= interval) {
        _lastMillis[id] = current;
        return true;
    }
    return false;
}

void IskakINO::write(uint8_t pin, uint8_t val) {
    digitalWrite(pin, val);
}

int IskakINO::readNorm(uint8_t pin) {
    #if defined(ARDUINO_ARCH_ESP32)
        return analogRead(pin) >> 2;
    #else
        return analogRead(pin);
    #endif
}

int IskakINO::readStable(uint8_t pin, uint8_t samples) {
    uint32_t sum = 0;
    for (uint8_t i = 0; i < samples; i++) sum += analogRead(pin);
    return sum / samples;
}

void IskakINO::printPrefix(const __FlashStringHelper* type) {
    Serial.print(F("["));
    Serial.print(millis());
    Serial.print(F("] ["));
    Serial.print(type);
    Serial.print(F("] "));
}

void IskakINO::log(const __FlashStringHelper* msg) {
    #if ISKAKINO_LOG_LEVEL >= LOG_INFO
        printPrefix(F("INFO"));
        Serial.println(msg);
    #endif
}

void IskakINO::log(const __FlashStringHelper* label, float val) {
    #if ISKAKINO_LOG_LEVEL >= LOG_INFO
        printPrefix(F("INFO"));
        Serial.print(label);
        Serial.print(F(": "));
        Serial.println(val);
    #endif
}

// Pre-instantiate object
IskakINO ArduFast;
