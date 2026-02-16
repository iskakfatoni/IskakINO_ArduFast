/**
 * @file 08_Button_Debounce.ino
 * @author Iskak Fatoni
 * @brief Menangani input tombol fisik agar stabil (Anti-Chatter).
 * @details 
 * Menggunakan ArduFast.every() untuk melakukan sampling status tombol.
 * Menghindari pembacaan ganda akibat getaran mekanis pada switch.
 */

#include <IskakINO_ArduFast.h>

// Gunakan FastPin untuk input yang responsif
const uint8_t BUTTON_PIN = 4;
FastPin<BUTTON_PIN> MyButton;
FastPin<LED_BUILTIN> MyLed;

bool lastState = HIGH;
int pressCount = 0;

void setup() {
    ArduFast.begin(115200);
    
    // Gunakan INPUT_PULLUP agar tidak butuh resistor eksternal
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    MyLed.mode(OUTPUT);

    ArduFast.log(F("Button Debounce Ready on Pin"), BUTTON_PIN);
}

void loop() {
    /**
     * @section Task_Debounce
     * @id 0
     * @interval 50ms
     * Membaca status tombol setiap 50ms. Ini adalah waktu standar
     * untuk memastikan getaran mekanis tombol sudah stabil.
     */
    if (ArduFast.every(50, 0)) {
        bool currentState = MyButton.read();

        // Cek jika status berubah dari HIGH ke LOW (Tombol ditekan)
        if (currentState == LOW && lastState == HIGH) {
            pressCount++;
            MyLed.toggle(); // Balikkan status LED setiap ditekan
            
            ArduFast.log(F("Tombol Ditekan! Total:"), pressCount);
        }

        lastState = currentState;
    }
}
