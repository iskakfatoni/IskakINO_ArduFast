/**
 * @file 05_EEPROM_Settings.ino
 * @author Iskak Fatoni
 * @brief Cara menyimpan dan membaca data permanen di memori EEPROM.
 * @details 
 * Sketsa ini mendemonstrasikan penyimpanan "Boot Count" (berapa kali alat menyala)
 * yang tetap tersimpan meskipun power dimatikan.
 * @note Library ini menangani perbedaan cara kerja EEPROM pada AVR, ESP8266, dan ESP32.
 */

#include <IskakINO_ArduFast.h>
#include <EEPROM.h>

// Struktur data yang ingin disimpan
struct UserSettings {
    int bootCount;
    int lastSensorValue;
};

UserSettings myData;
const int EEPROM_SIZE = sizeof(UserSettings);

void setup() {
    ArduFast.begin(115200);
    
    // --- Inisialisasi EEPROM (Wajib untuk ESP8266/ESP32) ---
    #if defined(ESP32) || defined(ESP8266)
        EEPROM.begin(EEPROM_SIZE);
    #endif

    // 1. Baca data dari EEPROM
    EEPROM.get(0, myData);

    // 2. Jika data korup atau baru pertama kali (misal: nilai minus), reset ke 0
    if (myData.bootCount < 0) myData.bootCount = 0;

    // 3. Tambah hitungan boot
    myData.bootCount++;
    
    ArduFast.log(F("Alat ini sudah menyala sebanyak"), myData.bootCount);

    // 4. Simpan kembali data terbaru ke EEPROM
    EEPROM.put(0, myData);
    
    #if defined(ESP32) || defined(ESP8266)
        EEPROM.commit(); // Wajib untuk menyimpan permanen di ESP
    #endif

    ArduFast.log(F("Data tersimpan aman di EEPROM"));
}

void loop() {
    /**
     * @section Task_Update_Sensor
     * @id 0
     * Simpan nilai sensor ke EEPROM setiap 1 menit (60.000 ms) 
     * agar memori EEPROM tidak cepat rusak (wear-leveling).
     */
    if (ArduFast.every(60000, 0)) {
        myData.lastSensorValue = ArduFast.readStable(A0);
        
        EEPROM.put(0, myData);
        #if defined(ESP32) || defined(ESP8266)
            EEPROM.commit();
        #endif
        
        ArduFast.log(F("Auto-Save Sensor ke EEPROM:"), myData.lastSensorValue);
    }
}
