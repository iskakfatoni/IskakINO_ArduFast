/**
 * @file 04_FastPin_Benchmark.ino
 * @author Iskak Fatoni
 * @brief Membuktikan kecepatan FastPin dibandingkan digitalWrite standar.
 * @details 
 * Sketsa ini melakukan toggle pin sebanyak 10.000 kali dan mengukur 
 * waktu yang dibutuhkan dalam mikrodetik (us).
 * @note Hasil akan bervariasi tergantung kecepatan clock board (Uno vs ESP32).
 */

#include <IskakINO_ArduFast.h>

// Kita gunakan pin 2 sebagai target testing
#define TEST_PIN 2
FastPin<TEST_PIN> FastLed;

void setup() {
    ArduFast.begin(115200);
    FastLed.mode(OUTPUT);
    pinMode(TEST_PIN, OUTPUT);

    ArduFast.log(F("--- Benchmarking Start ---"));
    
    // 1. Benchmark digitalWrite Standar
    uint32_t startStd = micros();
    for(int i = 0; i < 10000; i++) {
        digitalWrite(TEST_PIN, HIGH);
        digitalWrite(TEST_PIN, LOW);
    }
    uint32_t endStd = micros() - startStd;

    // 2. Benchmark ArduFast FastPin
    uint32_t startFast = micros();
    for(int i = 0; i < 10000; i++) {
        FastLed.high();
        FastLed.low();
    }
    uint32_t endFast = micros() - startFast;

    // Laporan Hasil
    ArduFast.log(F("Standard digitalWrite (10k cycles)"), endStd);
    ArduFast.log(F("ArduFast FastPin (10k cycles)"), endFast);
    
    if (endFast < endStd) {
        ArduFast.log(F("Kesimpulan: FastPin lebih cepat (us)"), endStd - endFast);
    }
}

void loop() {
    // Tidak ada proses di loop untuk benchmark ini
}
