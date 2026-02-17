/**
 * @file 10_Watchdog_Safety.ino
 * @author Iskak Fatoni
 * @brief Sistem pemantauan kesehatan tugas (Software Watchdog).
 * @details 
 * Menggunakan ArduFast untuk memastikan tugas-tugas kritis tetap berjalan.
 * Jika tugas utama berhenti merespons, sistem akan memberikan peringatan.
 */

#include <IskakINO_ArduFast.h>
IskakINO_ArduFast ArduFast;

long lastHeartbeat = 0;
bool systemCritical = false;

void setup() {
    ArduFast.begin(115200);
    ArduFast.log(F("--- ArduFast Safety System Active ---"));
}

void loop() {
    /**
     * @section Task_Critical
     * @id 0
     * @interval 1000ms
     * Simulasikan tugas penting (misal: baca suhu mesin).
     */
    if (ArduFast.every(1000, 0)) {
        if (!systemCritical) {
            lastHeartbeat = millis(); // Perbarui tanda vital
            ArduFast.log(F("Tugas Utama: Normal"));
        }
    }

    /**
     * @section Task_Watchdog
     * @id 1
     * @interval 3000ms
     * Mengecek apakah 'Task_Critical' masih memberikan tanda vital.
     */
    if (ArduFast.every(3000, 1)) {
        if (millis() - lastHeartbeat > 2500) {
            ArduFast.log(F("PERINGATAN: Tugas Utama Macet!"), 0);
            systemCritical = true; // Mode darurat
        } else {
            ArduFast.log(F("Watchdog: Sistem Sehat"));
        }
    }

    /**
     * @section Task_Simulate_Failure
     * @id 2
     * Simulasi kerusakan: Setelah 10 detik, kita buat sistem macet.
     */
    if (ArduFast.every(10000, 2)) {
        ArduFast.log(F("Simulasi Kegagalan Sistem Dimulai..."));
        systemCritical = true; 
    }
}
