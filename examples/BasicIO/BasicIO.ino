#include <IskakINO_ArduFast.h>

// Definisi LED menggunakan FastPin (Puncak Performa)
FastPin<LED_BUILTIN> MyLed;

void setup() {
    ArduFast.begin(115200);
    MyLed.mode(OUTPUT);
}

void loop() {
    // Multitasking tanpa delay
    if (ArduFast.every(1000, 0)) {
        MyLed.toggle();
        ArduFast.log(F("Heartbeat Toggled"));
    }

    if (ArduFast.every(5000, 1)) {
        int val = ArduFast.readNorm(A0);
        ArduFast.log(F("Sensor Value"), val);
    }
}
