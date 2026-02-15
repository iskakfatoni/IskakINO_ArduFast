#include <IskakINO_ArduFast.h>

/**
 * IskakINO-ArduFast Example: 02_AdvancedAnalog
 * Mendemonstrasikan pembacaan sensor yang stabil (Anti-Noise)
 * dan normalisasi data lintas platform.
 */

const uint8_t SENSOR_PIN = A0;

void setup() {
    // Memulai sistem dengan baudrate tinggi untuk log yang cepat
    ArduFast.begin(115200);
    
    ArduFast.log(F("Advanced Analog Demo Started"));
    ArduFast.log(F("Membaca sensor dengan 32x Oversampling..."));
}

void loop() {
    // Task 1: Membaca data stabil setiap 200ms (ID: 0)
    if (ArduFast.every(200, 0)) {
        // readStable mengambil 32 sampel dan merata-ratakannya
        // Sangat efektif untuk menghilangkan 'jitter' pada sensor analog
        int cleanVal = ArduFast.readStable(SENSOR_PIN, 32);
        
        // Kirim ke Serial Plotter atau Log
        ArduFast.log(F("Stable Value"), cleanVal);
    }

    // Task 2: Membandingkan pembacaan setiap 2 detik (ID: 1)
    if (ArduFast.every(2000, 1)) {
        int raw = ArduFast.readNorm(SENSOR_PIN);
        ArduFast.log(F("--- Quick Check (Raw Normalized)"), raw);
    }
}
