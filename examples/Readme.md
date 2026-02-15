# 📚 Panduan Contoh Kode (Examples Guide)

Halaman ini menjelaskan standar dokumentasi dan struktur kode yang digunakan dalam contoh library **IskakINO-ArduFast**.

## 🛠 Standar Blok Komentar
Setiap contoh kode wajib menyertakan header dokumentasi menggunakan kata kunci berikut agar mudah dipahami oleh pemula dan terdeteksi oleh sistem dokumentasi otomatis.

| Kata Kunci | Deskripsi |
| :--- | :--- |
| **@brief** | Penjelasan singkat mengenai tujuan utama sketsa/fungsi. |
| **@param** | Penjelasan input yang dibutuhkan (misal: Pin, Interval, atau ID). |
| **@id** | (Khusus ArduFast) Menunjukkan ID task yang digunakan agar tidak bentrok. |
| **@note** | Catatan tambahan atau tips penggunaan. |
| **@warning** | Peringatan penting agar kode tidak merusak hardware atau error. |

---

## 📂 Struktur Folder Examples
Pastikan setiap contoh baru mengikuti struktur berikut:
```text
examples/
├── 01_BasicIO/          # Dasar input/output digital
├── 02_AdvancedAnalog/   # Pembacaan sensor stabil & normalisasi
├── 03_MultiTasking/     # Eksekusi paralel tanpa delay()
└── 04_NamaContohBaru/   # (Template untuk kontribusi baru)
