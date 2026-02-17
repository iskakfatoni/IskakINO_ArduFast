#include <IskakINO_ArduFast.h>
IskakINO_ArduFast ArduFast;
// Definisi LED menggunakan FastPin untuk performa maksimal
FastPin<LED_BUILTIN> MyLed;

void setup() {
    // Memulai sistem log dan serial
    ArduFast.begin(115200);
    
    // Set mode pin melalui template FastPin
    MyLed.mode(OUTPUT);
}

void loop() {
    // Task 1: Kedipkan LED setiap 1 detik (ID: 0)
    if (ArduFast.every(1000, 0)) {
        MyLed.toggle();
        ArduFast.log(F("LED Berkedip"));
    }

    // Task 2: Baca sensor setiap 5 detik (ID: 1)
    if (ArduFast.every(5000, 1)) {
        int val = ArduFast.readNorm(A0);
        ArduFast.log(F("Nilai Sensor (0-1023)"), val);
    }
}
