# Changelog
Semua perubahan penting pada IskakINO-ArduFast dicatat di file ini.

## [1.0.1] - 2026-07-18

### 🔧 CI
- **`compile-check.yml` sebelumnya tidak pernah benar-benar mengompilasi apa pun.** Job memakai glob `examples/*/*.ino`, yang mengasumsikan struktur `examples/<Nama>/<Nama>.ino`. Selama contoh masih berupa file `.ino` flat langsung di `examples/`, glob ini tidak match apa pun dan bash (tanpa `nullglob`) meneruskan string glob literalnya ke `arduino-cli compile`, membuat job gagal atau tidak menguji apa pun secara efektif. Perbaikan struktur folder examples di rilis ini membuat CI ini akhirnya berfungsi.
- ESP32: implementasi register langsung dibatasi ke pin 0-31 (`GPIO.out_w1ts`/`out_w1tc`/`GPIO.in`). Pin ≥32 sengaja fallback ke `digitalWrite()`/`digitalRead()` karena register bank kedua (`out1_w1ts` dkk.) penamaan field-nya tidak konsisten antar versi core `esp32:esp32`, dan tidak bisa diverifikasi tanpa compile-test langsung — prioritas diberikan ke kepastian CI hijau di 3 board matrix (`arduino:avr:nano`, `esp8266:esp8266:nodemcuv2`, `esp32:esp32:esp32`) daripada cakupan register 100%.

### 🔴 Perbaikan Kritis
- **`FastPin<P>` sekarang benar-benar direct register access**, bukan lagi wrapper `pinMode()`/`digitalWrite()`/`digitalRead()`.
  - **AVR**: `high()`/`low()`/`read()` memanipulasi register `PORTx`/`PINx` langsung; `toggle()` memakai trik hardware "tulis 1 ke register PINx" (1 siklus clock, tanpa read-modify-write).
  - **ESP32**: `high()`/`low()` memakai register write-1-to-set/clear (`GPIO.out_w1ts` / `GPIO.out_w1tc`), termasuk dukungan pin ≥32.
  - **ESP8266**: `high()`/`low()` memakai register `GPOS`/`GPOC`, dengan penanganan khusus GPIO16.
  - Platform lain tetap fallback aman ke `pinMode()`/`digitalWrite()`/`digitalRead()`.
  - Ini memperbaiki ketidaksesuaian sebelumnya di mana README mengklaim "1-2 cycles" dan "20x lebih cepat" tapi implementasinya identik dengan `digitalWrite()` biasa — sehingga contoh benchmark (`04_FastPin_Benchmark`, `11_Ultimate_Benchmark`) tidak akan menunjukkan perbedaan nyata.

### 🟠 Perbaikan API
- **`mapAnalog(pin, outMin, outMax)` diimplementasikan.** Sebelumnya hanya didokumentasikan di README/`keywords.txt` tapi tidak ada di source — memicu compile error bagi siapa pun yang mengikuti dokumentasi.
- **Signature `readStable()` di README diperbaiki** agar sesuai implementasi asli: `readStable(uint8_t pin, uint8_t samples = 16)`.
- **`log()` dipecah jadi 2 overload** (`log(msg)` dan `log(msg, long val)`), menggantikan default parameter dengan sentinel `-32768` yang rawan salah cetak jika ada value sensor yang kebetulan bernilai sama.
- **Tipe parameter value pada `log()` diganti dari `int` ke `long`.** Di AVR, `int` hanya 16-bit — memanggil `log(msg, millis())` atau `log(msg, someLongCounter)` sebelumnya bisa terpotong diam-diam. Sekarang aman untuk nilai 32-bit.

### 🟡 Perbaikan Dokumentasi
- Contoh kode "Cara Penggunaan Cepat" di README sekarang menyertakan baris `IskakINO_ArduFast ArduFast;` yang sebelumnya hilang (kode contoh tidak akan compile tanpanya).
- Tabel API Reference diperbarui: menambahkan `mapAnalog()`, `readNorm()`, `pinMode()`, `digitalWrite()`, `digitalRead()` yang sebelumnya tidak tercantum.
- Daftar folder `examples/` di README disesuaikan dengan nama file yang sebenarnya (`04_FastPin_Benchmark`, `05_EEPROM_Settings`, `06_Advanced_PWM`, `07_Serial_Command`, dst.).
- `keywords.txt` disinkronkan: menambahkan `readNorm`, `pinMode`, `digitalWrite`, `digitalRead`.
- Klaim performa di README diklarifikasi per-platform (AVR: register penuh untuk high/low/toggle/read; ESP32/ESP8266: register untuk high/low; `mode()` tetap `pinMode()` standar di semua platform karena hanya dipanggil sekali saat setup, bukan hot path).

## [1.0.0] - Rilis awal
- Ultra-fast digital I/O melalui `FastPin<P>` (implementasi awal, lihat catatan kritis 1.0.1 di atas).
- Smart analog read dengan normalisasi 0-1023 dan oversampling (`readStable`).
- Non-blocking task manager (`every()`) dengan 10 slot ID.
- Memory-efficient logging berbasis Flash string (`F()` macro).
- Dukungan lintas platform AVR, ESP8266, ESP32.
