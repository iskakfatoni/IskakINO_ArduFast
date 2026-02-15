# 🚀 IskakINO-ArduFast
**High-Performance & Lightweight Framework for AVR, ESP8266, and ESP32.**

IskakINO-ArduFast adalah framework Arduino yang dirancang untuk kecepatan eksekusi maksimal dan sintaks yang bersih. Dengan menggabungkan teknik *Template Metaprogramming* dan *Direct Register Access*, framework ini memangkas overhead fungsi standar Arduino tanpa mengorbankan kemudahan penggunaan.

---

## ✨ Fitur Unggulan

* **⚡ Ultra-Fast Digital I/O**: Menggunakan mesin `FastPin<P>` yang bekerja di level register. Lebih cepat hingga 20x dibanding `digitalWrite()` standar.
* **🎯 Smart Analog**: Normalisasi otomatis (0-1023) untuk semua board dan fitur *Internal Averaging* untuk pembacaan sensor yang stabil tanpa noise.
* **⏱️ Non-Blocking Task Manager**: Jalankan banyak perintah secara paralel (Multitasking) tanpa menggunakan `delay()`.
* **📑 Memory-Efficient Logging**: Sistem log cerdas yang otomatis menggunakan Flash Memory (F-macro) untuk menjaga RAM tetap lega.
* **🌐 Cross-Platform**: Satu kode untuk semua. Berjalan mulus di Arduino Nano (AVR), NodeMCU (ESP8266), hingga ESP32.

---

## 🛠️ Struktur Folder
```text
IskakINO-ArduFast/
├── src/
│   ├── IskakINO_ArduFast.h    # Header Utama
│   └── IskakINO_ArduFast.cpp  # Implementasi Logic
├── examples/
│   └── 01_BasicIO/            # Contoh Penggunaan Dasar
└── library.properties         # Metadata Library
```

## 🚀 Cara Penggunaan Cepat

### 1. Digital I/O (Mode Ultra Fast)
Gunakan `FastPin<Pin>` untuk pin statis guna mendapatkan kecepatan setara Assembly.

```cpp
#include <IskakINO_ArduFast.h>

// Definisi pin dilakukan di luar setup/loop (Global)
FastPin<13> LedBawaan; 

void setup() {
    LedBawaan.mode(OUTPUT);
}

void loop() {
    LedBawaan.high();    // Set HIGH tanpa overhead
    delay(500);
    LedBawaan.low();     // Set LOW
    delay(500);
    LedBawaan.toggle();  // Balikkan status (1-cycle di AVR)
}
```

### 2. Analog I/O (Smart & Stable)
Baca sensor dengan nilai yang konsisten di semua jenis board (0-1023).

```cpp
void loop() {
    // Rata-rata 16 sampel, normalisasi otomatis ke 10-bit
    int sensorValue = ArduFast.readStable(A0, 16); 
    ArduFast.log(F("Sensor Terfilter"), sensorValue);
    delay(100);
}
```
### 3. Multitasking (Non-Blocking)
Jalankan banyak tugas secara bersamaan tanpa menghentikan proses latar belakang.

```cpp
void loop() {
    // Task A: Kedipkan LED setiap 500ms (ID: 0)
    if (ArduFast.every(500, 0)) {
        LedBawaan.toggle();
    }

    // Task B: Kirim log setiap 5 detik (ID: 1)
    if (ArduFast.every(5000, 1)) {
        ArduFast.log(F("Sistem Aktif"));
    }
}
```
## 📊 Perbandingan Performa

| Fungsi | Arduino Standar | IskakINO-ArduFast | Performa |
| :--- | :--- | :--- | :--- |
| **Digital Write** | ~50-100 cycles | **1-2 cycles** | 🔥 Ekstrim |
| **Analog Read** | Raw (Beda board) | **Normalized (0-1023)** | 💎 Konsisten |
| **Memory** | RAM Hungry Strings | **Flash-Optimized** | 🍃 Ringan |

## 🔧 Instalasi
1. Download repository ini sebagai **.zip**.
2. Di Arduino IDE, buka menu **Sketch** -> **Include Library** -> **Add .ZIP Library**.
3. Pilih file yang baru didownload.
4. Selesai! IskakINO-ArduFast siap digunakan.

---

## 📝 Catatan Rilis (v1.0.0)
* Optimalisasi Digital & Analog I/O untuk performa tinggi.
* Sinkronisasi lintas platform (Cross-Platform) AVR, ESP8266, dan ESP32.
* Penanganan perbedaan `LED_BUILTIN` pada berbagai varian board.

---
**Developed with ❤️ by Iskak Fatoni**
