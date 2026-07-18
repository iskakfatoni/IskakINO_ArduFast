# 🚀 IskakINO-ArduFast
[![Compile-Check](https://github.com/iskakfatoni/IskakINO_ArduFast/actions/workflows/compile-check.yml/badge.svg)](https://github.com/iskakfatoni/IskakINO_ArduFast/actions/workflows/compile-check.yml)
[![GitHub release (latest by date)](https://img.shields.io/github/v/release/iskakfatoni/IskakINO_ArduFast?color=blue&logo=github)](https://github.com/iskakfatoni/IskakINO_ArduFast/releases)
[![Arduino Library Manager](https://img.shields.io/badge/Arduino-Library%20Manager-00979D?style=flat&logo=arduino)](https://www.arduino.cc/reference/en/libraries/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Framework](https://img.shields.io/badge/Framework-Arduino-00979D?logo=arduino)](https://www.arduino.cc/)

**High-Performance & Lightweight Framework for AVR, ESP8266, and ESP32.**
IskakINO-ArduFast adalah framework Arduino yang dirancang untuk kecepatan eksekusi maksimal dan sintaks yang bersih. Dengan menggabungkan teknik *Template Metaprogramming* dan *Direct Register Access* pada `FastPin<P>`, framework ini memangkas overhead fungsi standar Arduino tanpa mengorbankan kemudahan penggunaan.

---

## ✨ Fitur Unggulan

* **⚡ Ultra-Fast Digital I/O**: `FastPin<P>` bekerja langsung di level register perangkat keras — bukan sekadar wrapper `digitalWrite()`.
  * **AVR** (Uno/Nano/Mega dll.): `high()`/`low()`/`read()` memanipulasi `PORTx`/`PINx` langsung; `toggle()` memakai trik hardware AVR (tulis 1 ke register `PINx`) untuk pembalikan status dalam 1 siklus clock.
  * **ESP32**: `high()`/`low()`/`read()` memakai register write-1-to-set/clear (`GPIO.out_w1ts`/`out_w1tc`/`GPIO.in`) untuk pin 0-31, melewati overhead validasi pin pada `digitalWrite()` standar. Pin ≥32 (GPIO32-39 dst.) fallback ke `digitalWrite()`/`digitalRead()` standar karena register bank kedua tidak konsisten antar versi core.
  * **ESP8266**: `high()`/`low()` memakai register `GPOS`/`GPOC` (dengan penanganan khusus GPIO16).
  * Platform lain: fallback otomatis ke `pinMode()`/`digitalWrite()`/`digitalRead()` standar sehingga kode tetap kompatibel.
  * *Catatan: `mode()` tetap memakai `pinMode()` standar di semua platform karena hanya dipanggil sekali saat `setup()`, bukan operasi berulang di `loop()`.*
* **🎯 Smart Analog**: Normalisasi otomatis (0-1023) untuk semua board, *Internal Averaging* untuk pembacaan sensor stabil (`readStable`), dan pemetaan langsung ke rentang custom (`mapAnalog`).
* **⏱️ Non-Blocking Task Manager**: Jalankan hingga 10 task paralel (Multitasking) tanpa menggunakan `delay()`.
* **📑 Memory-Efficient Logging**: Sistem log yang otomatis menggunakan Flash Memory (`F()` macro) untuk menjaga RAM tetap lega.
* **🌐 Cross-Platform**: Satu kode untuk semua. Berjalan mulus di Arduino Nano (AVR), NodeMCU (ESP8266), hingga ESP32.

---

## 🛠️ Struktur Folder
```text
IskakINO-ArduFast/
├── .github/
│   └── workflows/               # Konfigurasi CI/CD (GitHub Actions & Lint)
├── examples/                    # Koleksi 11 contoh penggunaan library
│   ├── 01_BasicIO/               # Dasar FastPin & Logging
│   ├── 02_AdvancedAnalog/         # Pembacaan sensor stabil & normalisasi
│   ├── 03_MultiTasking/           # Scheduler tanpa delay()
│   ├── 04_FastPin_Benchmark/      # Uji kecepatan FastPin vs digitalWrite
│   ├── 05_EEPROM_Settings/        # Penyimpanan data permanen (Cross-platform)
│   ├── 06_Advanced_PWM/           # Efek LED Breathing non-blocking
│   ├── 07_Serial_Command/         # Kontrol interaktif via Serial Monitor
│   ├── 08_Button_Debounce/        # Input tombol fisik yang stabil
│   ├── 09_StressTest/             # Uji beban 10 task sekaligus (Max ID)
│   ├── 10_Watchdog_Safety/        # Sistem proteksi kesehatan task
│   └── 11_Ultimate_Benchmark/     # Head-to-Head: ArduFast vs Arduino.h
├── src/                          # Source code utama (Core)
│   ├── IskakINO_ArduFast.h        # Header utama, template FastPin, & register I/O
│   └── IskakINO_ArduFast.cpp      # Implementasi logic multitasking & analog
├── library.properties            # Metadata resmi untuk Arduino Library Manager
├── keywords.txt                  # Syntax highlighting untuk Arduino IDE
├── CHANGELOG.md                  # Riwayat perubahan tiap rilis
└── README.md                     # Dokumentasi utama (halaman ini)
```
> 📌 Setiap sketsa di `examples/` harus berada di dalam folder dengan nama **persis sama** dengan file `.ino`-nya (mis. `examples/01_BasicIO/01_BasicIO.ino`) — ini syarat wajib Arduino Library Manager.

## 🚀 Cara Penggunaan Cepat
### 1. Digital I/O (Mode Ultra Fast — Direct Register Access)
Gunakan `FastPin<Pin>` untuk pin statis guna mendapatkan kecepatan setara akses register langsung.
```cpp
#include <IskakINO_ArduFast.h>

// Definisi pin dilakukan di luar setup/loop (Global)
FastPin<13> LedBawaan;

void setup() {
    LedBawaan.mode(OUTPUT);
}

void loop() {
    LedBawaan.high();    // Set HIGH via register langsung
    delay(500);
    LedBawaan.low();     // Set LOW via register langsung
    delay(500);
    LedBawaan.toggle();  // Balikkan status (1 siklus clock di AVR)
}
```
### 2. Analog I/O (Smart & Stable)
Baca sensor dengan nilai yang konsisten di semua jenis board (0-1023), atau langsung dipetakan ke rentang lain.
```cpp
#include <IskakINO_ArduFast.h>

IskakINO_ArduFast ArduFast;

void setup() {
    ArduFast.begin(115200);
}

void loop() {
    // Rata-rata 16 sampel, normalisasi otomatis ke 10-bit
    int sensorValue = ArduFast.readStable(A0, 16);
    ArduFast.log(F("Sensor Terfilter"), sensorValue);

    // Langsung dipetakan ke rentang 0-255 (mis. untuk PWM)
    int mapped = ArduFast.mapAnalog(A0, 0, 255);
    ArduFast.log(F("Sensor Termap"), mapped);

    delay(100);
}
```
### 3. Multitasking (Non-Blocking)
Jalankan banyak tugas secara bersamaan tanpa menghentikan proses latar belakang.
```cpp
#include <IskakINO_ArduFast.h>

IskakINO_ArduFast ArduFast;
FastPin<13> LedBawaan;

void setup() {
    ArduFast.begin(115200);
    LedBawaan.mode(OUTPUT);
}

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
Berikut adalah prototipe fungsi dan cara penggunaan library IskakINO-ArduFast, sinkron dengan implementasi di `src/`.
```cpp
// --- [ 1. MANAJEMEN TUGAS & WAKTU ] ---
// Slot ID tersedia: 0-9. Memungkinkan multitasking tanpa delay().
void ArduFast.begin(unsigned long baud = 115200);
bool ArduFast.every(unsigned long interval_ms, uint8_t id);


// --- [ 2. ANALOG & SENSOR ] ---
// Fitur stabilisasi & pemetaan pembacaan analog (Oversampling & Mapping).
int ArduFast.readNorm(uint8_t pin);
int ArduFast.readStable(uint8_t pin, uint8_t samples = 16);
int ArduFast.mapAnalog(uint8_t pin, int outMin, int outMax);


// --- [ 3. FAST DIGITAL I/O — DIRECT REGISTER ACCESS ] ---
// Template-based I/O yang dieksekusi langsung pada level register
// (AVR: PORT/PIN penuh; ESP32/ESP8266: register set/clear untuk high/low).
FastPin<PIN_NUMBER> PinName;

void PinName.mode(uint8_t mode); // pinMode() standar (dipanggil sekali di setup)
void PinName.high();             // Output HIGH via register langsung
void PinName.low();              // Output LOW via register langsung
void PinName.toggle();           // Balikkan status (1 siklus clock di AVR)
bool PinName.read();              // Membaca input digital via register langsung


// --- [ 4. WRAPPER IO STANDAR ] ---
// Untuk pin yang ditentukan secara dinamis (variabel, bukan konstanta template).
void ArduFast.pinMode(uint8_t pin, uint8_t mode);
void ArduFast.digitalWrite(uint8_t pin, uint8_t val);
int ArduFast.digitalRead(uint8_t pin);


// --- [ 5. DEBUGGING & UTILITY ] ---
// Mencetak pesan log ke Serial Monitor dengan prefix khusus, hemat RAM (F-macro).
void ArduFast.log(const __FlashStringHelper* msg);
void ArduFast.log(const __FlashStringHelper* msg, long val);
```
---
## 📊 Perbandingan Performa

| Fungsi | Arduino Standar | IskakINO-ArduFast | Performa |
| :--- | :--- | :--- | :--- |
| **Digital Write (AVR)** | ~50-100 cycles | **1-2 cycles** (akses register langsung) | 🔥 Ekstrim |
| **Digital Write (ESP32/ESP8266)** | Overhead validasi pin + fungsi | **Register write-1-set/clear langsung** | ⚡ Signifikan |
| **Analog Read** | Raw (beda tiap board) | **Normalized (0-1023) + Mapping** | 💎 Konsisten |
| **Memory** | RAM Hungry Strings | **Flash-Optimized** | 🍃 Ringan |

> Angka performa aktual bisa dicek langsung lewat `examples/04_FastPin_Benchmark/` dan `examples/11_Ultimate_Benchmark/`, yang membandingkan `FastPin` melawan `digitalWrite()` standar di board Anda.

---

## 🔧 Instalasi
1. Download repository ini sebagai **.zip**.
2. Di Arduino IDE, buka menu **Sketch** -> **Include Library** -> **Add .ZIP Library**.
3. Pilih file yang baru didownload.
4. Selesai! IskakINO-ArduFast siap digunakan.

---

## 📝 Catatan Rilis

### v1.0.1
* **[Perbaikan Kritis]** `FastPin<P>` kini benar-benar direct register access (AVR: PORT/PIN penuh; ESP32: `GPIO.out_w1ts`/`out_w1tc`; ESP8266: `GPOS`/`GPOC`) — sebelumnya hanya wrapper `digitalWrite()`/`digitalRead()` standar.
* **[Perbaikan API]** `mapAnalog()` diimplementasikan (sebelumnya hanya didokumentasikan, memicu compile error).
* **[Perbaikan API]** `log()` dipecah menjadi 2 overload dan tipe value diganti ke `long` (menghindari truncation di AVR).
* **[Dokumentasi]** Contoh Quick Start, tabel API, dan `keywords.txt` disinkronkan penuh dengan source code.

Lihat [CHANGELOG.md](CHANGELOG.md) untuk detail lengkap.

### v1.0.0
* Optimalisasi Digital & Analog I/O untuk performa tinggi.
* Sinkronisasi lintas platform (Cross-Platform) AVR, ESP8266, dan ESP32.
* Penanganan perbedaan `LED_BUILTIN` pada berbagai varian board.

---
**Developed with ❤️ by Iskak Fatoni**
