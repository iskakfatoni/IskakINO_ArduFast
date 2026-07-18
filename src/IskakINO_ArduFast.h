//
#ifndef ISKAKINO_ARDUFAST_H
#define ISKAKINO_ARDUFAST_H

#include <Arduino.h>

#if defined(ESP32)
  #include "soc/gpio_struct.h"
#endif

// Penanganan khusus jika board tidak mendefinisikan LED_BUILTIN (Sering terjadi di ESP32)
#ifndef LED_BUILTIN
  #define LED_BUILTIN 2 
#endif

// ============================================================================
// FastPin<P> — Direct Register I/O
// ----------------------------------------------------------------------------
// AVR (Uno/Nano/Mega dll.)   : high()/low()/toggle()/read() bekerja langsung
//                               di register PORTx/PINx via bit-manipulation.
//                               toggle() memakai "AVR PINx write-toggle trick"
//                               (menulis 1 ke register PINx membalik output
//                               dalam 1 siklus clock, tanpa read-modify-write).
// ESP32                      : high()/low()/read() memakai register write-1-
//                               to-set/clear (GPIO.out_w1ts/out_w1tc/GPIO.in)
//                               untuk pin 0-31, melewati overhead validasi
//                               pin milik digitalWrite() standar. Pin >=32
//                               (GPIO32-39 dst.) fallback ke digitalWrite()/
//                               digitalRead() karena register bank kedua
//                               penamaannya tidak konsisten antar versi core.
// ESP8266                    : high()/low() memakai register GPOS/GPOC
//                               (GPIO Output Set/Clear), dengan penanganan
//                               khusus untuk GPIO16 (register RTC terpisah).
// Platform lain               : fallback aman ke pinMode()/digitalWrite()/
//                               digitalRead() standar agar tetap kompatibel
//                               dan tetap benar secara fungsional.
//
// CATATAN: mode() tetap memakai pinMode() standar di semua platform. Ini
// karena konfigurasi arah pin biasanya hanya dipanggil sekali di setup()
// (bukan hot path), sedangkan high()/low()/toggle()/read() adalah operasi
// yang dipanggil berulang-ulang di loop() sehingga paling diuntungkan dari
// akses register langsung. Pengecualian: AVR mendapat mode() versi register
// juga karena biayanya sama murahnya dan menjaga konsistensi kelas ini
// tanpa dependency ke Serial/inisialisasi apa pun.
// ============================================================================
template <uint8_t P>
class FastPin {
public:
    inline void mode(uint8_t m) __attribute__((always_inline)) {
#if defined(__AVR__)
        volatile uint8_t *ddr  = (volatile uint8_t*)portModeRegister(digitalPinToPort(P));
        volatile uint8_t *port = (volatile uint8_t*)portOutputRegister(digitalPinToPort(P));
        const uint8_t bit = digitalPinToBitMask(P);
        if (m == OUTPUT) {
            *ddr |= bit;
        } else {
            *ddr &= ~bit;
            if (m == INPUT_PULLUP) {
                *port |= bit;
            } else {
                *port &= ~bit;
            }
        }
#else
        pinMode(P, m);
#endif
    }

    inline void high() __attribute__((always_inline)) {
#if defined(__AVR__)
        *(volatile uint8_t*)portOutputRegister(digitalPinToPort(P)) |= digitalPinToBitMask(P);
#elif defined(ESP32)
        // Hanya pin 0-31 yang dijamin lewat register write-1-to-set utama
        // (GPIO.out_w1ts). Pin >=32 (GPIO32-39 dst.) memakai register bank
        // kedua yang penamaan field-nya berbeda-beda antar versi core
        // arduino-esp32 — supaya tidak berisiko gagal compile di CI matrix,
        // pin >=32 sengaja fallback ke digitalWrite() standar.
        if (P < 32) {
            GPIO.out_w1ts = (1UL << P);
        } else {
            digitalWrite(P, HIGH);
        }
#elif defined(ESP8266)
        if (P < 16) {
            GPOS = (1UL << P);
        } else if (P == 16) {
            GP16O |= 1;
        } else {
            digitalWrite(P, HIGH);
        }
#else
        digitalWrite(P, HIGH);
#endif
    }

    inline void low() __attribute__((always_inline)) {
#if defined(__AVR__)
        *(volatile uint8_t*)portOutputRegister(digitalPinToPort(P)) &= ~digitalPinToBitMask(P);
#elif defined(ESP32)
        if (P < 32) {
            GPIO.out_w1tc = (1UL << P);
        } else {
            digitalWrite(P, LOW);
        }
#elif defined(ESP8266)
        if (P < 16) {
            GPOC = (1UL << P);
        } else if (P == 16) {
            GP16O &= ~1;
        } else {
            digitalWrite(P, LOW);
        }
#else
        digitalWrite(P, LOW);
#endif
    }

    inline void toggle() __attribute__((always_inline)) {
#if defined(__AVR__)
        // Trik hardware AVR: menulis 1 ke register PINx (bukan PORTx)
        // membalik bit output yang bersangkutan dalam 1 siklus clock,
        // tanpa perlu read-modify-write seperti pendekatan biasa.
        *(volatile uint8_t*)portInputRegister(digitalPinToPort(P)) = digitalPinToBitMask(P);
#else
        if (read()) low(); else high();
#endif
    }

    inline bool read() __attribute__((always_inline)) {
#if defined(__AVR__)
        return (*(volatile uint8_t*)portInputRegister(digitalPinToPort(P)) & digitalPinToBitMask(P)) != 0;
#elif defined(ESP32)
        if (P < 32) {
            return (GPIO.in & (1UL << P)) != 0;
        } else {
            return digitalRead(P);
        }
#elif defined(ESP8266)
        if (P < 16) {
            return (GPI & (1UL << P)) != 0;
        } else if (P == 16) {
            return (GP16I & 1) != 0;
        } else {
            return digitalRead(P);
        }
#else
        return digitalRead(P);
#endif
    }
};

// --- Class Framework ---
class IskakINO_ArduFast {
private:
    unsigned long _prevMillis[10];

public:
    IskakINO_ArduFast(); // Constructor

    void begin(unsigned long baud = 115200);
    bool every(unsigned long interval, uint8_t id);

    // --- Analog ---
    int readNorm(uint8_t pin);
    int readStable(uint8_t pin, uint8_t samples = 16);
    int mapAnalog(uint8_t pin, int outMin, int outMax);

    // --- Logging ---
    // Dipecah jadi 2 overload (bukan default parameter + sentinel value)
    // supaya tidak ada angka ajaib yang bisa keliru dengan value sensor asli.
    void log(const __FlashStringHelper* msg);
    void log(const __FlashStringHelper* msg, long val);

    // --- Wrapper IO standar (untuk pin dinamis / non-template) ---
    void pinMode(uint8_t pin, uint8_t mode);
    void digitalWrite(uint8_t pin, uint8_t val);
    int digitalRead(uint8_t pin);
};

// Deklarasi instance agar bisa diakses global
//extern IskakINO_ArduFast ArduFast;

#endif
