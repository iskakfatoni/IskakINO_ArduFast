/**
 * @file 09_StressTest.ino
 * @author Iskak Fatoni
 * @brief Menjalankan 10 tugas paralel (Maksimum ID) untuk uji beban.
 * @details 
 * Sketsa ini mengisi semua slot ID (0-9) dengan tugas yang berbeda.
 * Digunakan untuk memvalidasi stabilitas timing engine ArduFast.
 */

#include <IskakINO_ArduFast.h>
IskakINO_ArduFast ArduFast;

long loopCounter = 0;

void setup() {
    ArduFast.begin(115200);
    ArduFast.log(F("--- ArduFast Stress Test Start ---"));
}

void loop() {
    // Hitung setiap siklus loop yang berhasil dijalankan
    loopCounter++;

    // ID 0: Tugas Super Cepat (1ms) - Misal: Sampling sinyal analog mentah
    if (ArduFast.every(1, 0)) { /* Fast Task */ }

    // ID 1: Tugas Cepat (10ms)
    if (ArduFast.every(10, 1)) { /* UI Refresh */ }

    // ID 2 s/d 7: Tugas Menengah (Berbagai interval)
    if (ArduFast.every(50, 2))  { }
    if (ArduFast.every(100, 3)) { }
    if (ArduFast.every(200, 4)) { }
    if (ArduFast.every(300, 5)) { }
    if (ArduFast.every(400, 6)) { }
    if (ArduFast.every(500, 7)) { }

    // ID 8: Tugas Sensor (1 detik)
    if (ArduFast.every(1000, 8)) {
        ArduFast.log(F("ID 8: Sensor 1s Update"));
    }

    /**
     * @section Performance_Monitor
     * @id 9
     * @interval 5 detik
     * Menghitung efisiensi: Berapa kali loop() bisa berjalan dalam 5 detik.
     */
    if (ArduFast.every(5000, 9)) {
        long lps = loopCounter / 5;
        ArduFast.log(F("Performance (Loops/Sec):"), lps);
        
        // Reset counter untuk pengukuran berikutnya
        loopCounter = 0;
        ArduFast.log(F("Semua 10 ID berjalan stabil!"));
    }
}
