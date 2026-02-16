/**
 * @file 11_Ultimate_Benchmark.ino
 * @author Iskak Fatoni
 * @brief Head-to-Head: ArduFast vs Arduino Standard Library (Arduino.h).
 * @details 
 * Mengukur perbandingan kecepatan eksekusi mentah dan efisiensi multitasking.
 * @note Hasil pada AVR (Uno/Nano) akan menunjukkan perbedaan paling signifikan.
 */

#include <IskakINO_ArduFast.h>

// Definisikan pin testing
#define TEST_PIN 2
FastPin<TEST_PIN> MyFastPin;

void setup() {
    ArduFast.begin(115200);
    
    // Inisialisasi kedua metode
    pinMode(TEST_PIN, OUTPUT);
    MyFastPin.mode(OUTPUT);

    ArduFast.log(F("========================================"));
    ArduFast.log(F("       ARDUFAST ULTIMATE BENCHMARK      "));
    ArduFast.log(F("========================================"));
    delay(2000); // Tunggu Serial Monitor siap

    // --- TEST 1: RAW I/O SPEED (10.000 Siklus) ---
    ArduFast.log(F("Running Test 1: Raw Toggle Speed..."));

    // A. Standar Arduino.h
    uint32_t startStd = micros();
    for(int i = 0; i < 10000; i++) {
        digitalWrite(TEST_PIN, HIGH);
        digitalWrite(TEST_PIN, LOW);
    }
    uint32_t durationStd = micros() - startStd;

    // B. ArduFast FastPin
    uint32_t startFast = micros();
    for(int i = 0; i < 10000; i++) {
        MyFastPin.high();
        MyFastPin.low();
    }
    uint32_t durationFast = micros() - startFast;

    // Tampilkan Hasil Test 1
    ArduFast.log(F("Standard digitalWrite (us):"), durationStd);
    ArduFast.log(F("ArduFast FastPin (us)     :"), durationFast);
    
    if (durationFast > 0) {
        float speedUp = (float)durationStd / (float)durationFast;
        ArduFast.log(F("FastPin Faster by (x times):"), (int)speedUp);
    }

    ArduFast.log(F("----------------------------------------"));
    ArduFast.log(F("Running Test 2: Multitasking Efficiency"));
    ArduFast.log(F("Observe 'LPS' (Loops Per Second) below."));
}

long loopCount = 0;

void loop() {
    // Counter ini mewakili seberapa bebas CPU bekerja
    loopCount++;

    /**
     * @section Monitor_Efficiency
     * @id 0
     * @interval 1000ms
     */
    if (ArduFast.every(1000, 0)) {
        // Jika menggunakan delay(1000), LPS akan bernilai 1.
        // Dengan ArduFast, LPS akan tetap tinggi meski ada task aktif.
        ArduFast.log(F("Current LPS (Efficiency):"), loopCount);
        loopCount = 0; 
    }
}
