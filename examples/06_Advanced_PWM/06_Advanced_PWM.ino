/**
 * @file 06_Advanced_PWM.ino
 * @author Iskak Fatoni
 * @brief Membuat efek Fading LED (Breathing) tanpa menghentikan program.
 * @details 
 * Menggunakan ArduFast.every() untuk mengubah duty cycle PWM secara bertahap.
 * Ini memungkinkan LED memudar halus sambil tetap menjalankan tugas lain.
 */

#include <IskakINO_ArduFast.h>

// Gunakan pin yang mendukung PWM (simbol ~ di Arduino)
// Untuk ESP32/ESP8266 hampir semua pin mendukung PWM
#if defined(ESP32) || defined(ESP8266)
  const uint8_t pwmPin = 2; 
#else
  const uint8_t pwmPin = 9; // Pin 9 untuk Arduino Uno/Nano
#endif

int brightness = 0;    // Tingkat terang LED
int fadeAmount = 5;    // Besarnya perubahan terang

void setup() {
    ArduFast.begin(115200);
    
    pinMode(pwmPin, OUTPUT);
    
    ArduFast.log(F("Breathing LED dimulai pada Pin"), pwmPin);
}

void loop() {
    /**
     * @section Task_Fading
     * @id 0
     * @interval 30ms
     * Mengubah tingkat terang setiap 30 milidetik untuk transisi halus.
     */
    if (ArduFast.every(30, 0)) {
        analogWrite(pwmPin, brightness);

        // Ubah kecerahan untuk langkah berikutnya
        brightness = brightness + fadeAmount;

        // Balikkan arah fading jika sudah mencapai batas
        if (brightness <= 0 || brightness >= 255) {
            fadeAmount = -fadeAmount;
        }
    }

    /**
     * @section Task_Heartbeat_Serial
     * @id 1
     * Membuktikan bahwa sistem tetap bekerja meski LED sedang fading.
     */
    if (ArduFast.every(2000, 1)) {
        ArduFast.log(F("Sistem tetap responsif, Brightness:"), brightness);
    }
}
