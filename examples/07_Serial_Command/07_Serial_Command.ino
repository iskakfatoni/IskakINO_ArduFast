/**
 * @file 07_Serial_Command.ino
 * @author Iskak Fatoni
 * @brief Mengontrol Arduino melalui perintah teks dari Serial Monitor.
 * @details 
 * Menunjukkan cara membaca input Serial secara non-blocking. 
 * Anda bisa mengetik 'ON' untuk menyalakan LED, atau 'OFF' untuk mematikannya.
 */

#include <IskakINO_ArduFast.h>
IskakINO_ArduFast ArduFast;
FastPin<LED_BUILTIN> MyLed;

void setup() {
    ArduFast.begin(115200);
    MyLed.mode(OUTPUT);

    ArduFast.log(F("--- ArduFast Terminal Ready ---"));
    ArduFast.log(F("Ketik 'ON' atau 'OFF' di Serial Monitor"));
}

void loop() {
    /**
     * @section Task_Serial_Reader
     * Kita tidak menggunakan id timer di sini karena kita ingin 
     * mengecek Serial setiap kali loop berjalan (sangat responsif).
     */
    if (Serial.available() > 0) {
        String input = Serial.readStringUntil('\n');
        input.trim(); // Menghapus spasi atau enter tambahan

        if (input.equalsIgnoreCase("ON")) {
            MyLed.high();
            ArduFast.log(F("Command Received: LED is now ON"));
        } 
        else if (input.equalsIgnoreCase("OFF")) {
            MyLed.low();
            ArduFast.log(F("Command Received: LED is now OFF"));
        }
        else if (input.equalsIgnoreCase("STATUS")) {
            bool state = MyLed.read();
            ArduFast.log(F("Current LED Status:"), state);
        }
        else {
            ArduFast.log(F("Unknown Command:"), 0);
        }
    }

    /**
     * @section Task_Background
     * @id 0
     * Menjalankan tugas latar belakang untuk membuktikan Serial 
     * tidak menghambat proses lain.
     */
    if (ArduFast.every(5000, 0)) {
        ArduFast.log(F("System is alive and waiting for commands..."));
    }
}
