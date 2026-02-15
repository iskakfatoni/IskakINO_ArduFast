/**
 * 🚀 IskakINO-ArduFast: Example 03 - MultiTasking
 * * APA ITU MULTITASKING?
 * Di Arduino standar, jika Anda menggunakan delay(1000), mikrokontroler akan "berhenti tidur" 
 * selama 1 detik dan tidak bisa mengerjakan tugas lain.
 * * SOLUSI ARDUFAST:
 * Fungsi `ArduFast.every(interval, id)` memungkinkan Anda menjalankan banyak perintah 
 * secara paralel berdasarkan waktu. Program akan terus berjalan (Non-Blocking).
 * * PARAMETER:
 * 1. interval : Waktu tunggu dalam milidetik (ms).
 * 2. id       : Identitas unik (0-9). Setiap tugas HARUS memiliki ID yang berbeda.
 */

#include <IskakINO_ArduFast.h>

// Gunakan FastPin untuk kontrol LED yang sangat efisien
FastPin<LED_BUILTIN> StatusLed;

void setup() {
    // Memulai sistem komunikasi Serial (Default 115200)
    ArduFast.begin(115200);
    
    // Inisialisasi LED
    StatusLed.mode(OUTPUT);
    
    ArduFast.log(F("--- MultiTasking Demo Dimulai ---"));
}

void loop() {
    /**
     * TUGAS 1: Heartbeat (Detak Jantung)
     * Membuat LED berkedip sangat cepat agar kita tahu sistem tidak 'hang'.
     */
    if (ArduFast.every(100, 0)) {
        StatusLed.toggle();
    }

    /**
     * TUGAS 2: Monitor Sensor
     * Membaca sensor Analog secara stabil setiap setengah detik.
     * Perhatikan ID yang digunakan adalah 1.
     */
    if (ArduFast.every(500, 1)) {
        int sensorData = ArduFast.readStable(A0, 8);
        ArduFast.log(F("Baca Sensor"), sensorData);
    }

    /**
     * TUGAS 3: Laporan Sistem (Uptime)
     * Mengirimkan status sistem ke komputer setiap 5 detik.
     * Perhatikan ID yang digunakan adalah 2.
     */
    if (ArduFast.every(5000, 2)) {
        ArduFast.log(F("Sistem berjalan selama (ms)"), millis());
        ArduFast.log(F("Status: Semua tugas lancar tanpa delay!"));
    }
}
