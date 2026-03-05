# 🚀 IskakINO-ArduFast
[![Compile-Check](https://github.com/iskakfatoni/IskakINO_ArduFast/actions/workflows/compile-check.yml/badge.svg)](https://github.com/iskakfatoni/IskakINO_ArduFast/actions/workflows/compile-check.yml)
[![GitHub release (latest by date)](https://img.shields.io/github/v/release/iskakfatoni/IskakINO_ArduFast?color=blue&logo=github)](https://github.com/iskakfatoni/IskakINO_ArduFast/releases)
[![Arduino Library Manager](https://img.shields.io/badge/Arduino-Library%20Manager-00979D?style=flat&logo=arduino)](https://www.arduino.cc/reference/en/libraries/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Framework](https://img.shields.io/badge/Framework-Arduino-00979D?logo=arduino)](https://www.arduino.cc/)

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
├── .github/
│   └── workflows/           # Konfigurasi CI/CD (GitHub Actions & Lint)
├── examples/                # Koleksi 11 contoh penggunaan library
│   ├── 01_BasicIO/          # Dasar FastPin & Logging
│   ├── 02_AdvancedAnalog/   # Pembacaan sensor stabil & normalisasi
│   ├── 03_MultiTasking/     # Scheduler tanpa delay()
│   ├── 04_Benchmark/        # Uji kecepatan FastPin vs digitalWrite
│   ├── 05_EEPROM/           # Penyimpanan data permanen (Cross-platform)
│   ├── 06_AdvancedPWM/      # Efek LED Breathing non-blocking
│   ├── 07_SerialCommand/    # Kontrol interaktif via Serial Monitor
│   ├── 08_ButtonDebounce/   # Input tombol fisik yang stabil
│   ├── 09_StressTest/       # Uji beban 10 task sekaligus (Max ID)
│   ├── 10_WatchdogSafety/   # Sistem proteksi kesehatan task
│   └── 11_UltimateBenchmark/# Head-to-Head: ArduFast vs Arduino.h
├── src/                     # Source code utama (Core)
│   ├── IskakINO_ArduFast.h  # Header utama & template FastPin
│   └── IskakINO_ArduFast.cpp# Implementasi logic multitasking
├── library.properties       # Metadata resmi untuk Arduino Library Manager
├── keywords.txt             # Syntax highlighting untuk Arduino IDE
└── README.md                # Dokumentasi utama (halaman ini)
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


## 📚 API REFERENCE - ISKAKINO ARDUFAST
 * Berikut adalah prototipe fungsi dan cara penggunaan library IskakINO-ArduFast. Gunakan referensi ini untuk memahami parameter dan fungsionalitas utama.
```cpp
// --- [ 1. MANAJEMEN TUGAS & WAKTU ] ---
// Slot ID tersedia: 0-9. Memungkinkan multitasking tanpa delay().
bool ArduFast.every(uint32_t interval_ms, uint8_t id);
void ArduFast.begin(long baudrate);


// --- [ 2. ANALOG & SENSOR ] ---
// Fitur stabilisasi pembacaan analog (Oversampling & Mapping).
int ArduFast.readStable(uint8_t pin);
int ArduFast.mapAnalog(uint8_t pin, int outMin, int outMax);


// --- [ 3. FAST DIGITAL I/O ] ---
// Template-based I/O yang dieksekusi langsung pada level register.
FastPin<PIN_NUMBER> PinName;

void PinName.mode(uint8_t mode); // Mengatur arah data
void PinName.high();             // Output HIGH
void PinName.low();              // Output LOW
void PinName.toggle();           // Membalikkan status (HIGH <-> LOW)
bool PinName.read();             // Membaca input digital


// --- [ 4. DEBUGGING & UTILITY ] ---
// Mencetak pesan log ke Serial Monitor dengan prefix khusus.
void ArduFast.log(const __FlashStringHelper* msg, long value);
```
---
## 📊 Perbandingan Performa

| Fungsi | Arduino Standar | IskakINO-ArduFast | Performa |
| :--- | :--- | :--- | :--- |
| **Digital Write** | ~50-100 cycles | **1-2 cycles** | 🔥 Ekstrim |
| **Analog Read** | Raw (Beda board) | **Normalized (0-1023)** | 💎 Konsisten |
| **Memory** | RAM Hungry Strings | **Flash-Optimized** | 🍃 Ringan |
---

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
