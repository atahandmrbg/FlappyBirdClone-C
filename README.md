# flappy-bird
Flappy Bird Clone - YZM104 Project
# 🐦 Flappy Bird Clone (C & SFML)

Bu proje, C programlama dili ve CSFML kütüphanesi kullanılarak geliştirilmiş bir Flappy Bird klonudur. Üniversite proje gereksinimleri doğrultusunda temel oyun mekanikleri inşa edilmiş, üzerine ekstra grafik ve ses özellikleri eklenerek tam bir Arcade oyun deneyimi sunulmuştur.

## 🚀 Geliştirilen Özellikler
- **Fizik ve Hareket:** Gerçekçi yerçekimi ve zıplama (itki) mekaniği.
- **Rastgele Nesne Üretimi:** Her seferinde farklı yüksekliklerde gelen engeller (Boru üretimi).
- **Çarpışma Tespiti (Collision):** Kuşun borulara, yere veya tavana çarpma durumunun algılanması.
- **Skor Sistemi:** Anlık skor sayacı ve oyun kapansa da (oyun döngüsü içinde) tutulan En Yüksek Skor (High Score) kaydı.
- **Game State Yönetimi:** Oyuncuyu karşılayan "Başlamak için SPACE'e bas" bekleme ekranı.

## 🎨 Ekstra Tasarım ve Oyun Hissi (Juice)
- **Parallax Arka Plan:** Hız hissini artıran, sonsuz kayan sonbahar temalı arka plan.
- **Dinamik Rotasyon:** Kuşun düşme ve zıplama ivmesine göre burnunun havaya kalkıp inmesi.
- **Ses Efektleri (SFX):** Zıplama anında çalan tok "swoosh" ses efekti.
- **Retro UI:** Atari salonlarını anımsatan yanıp sönen (blinking) metin animasyonları ve Impact font kullanımı.

## 🎮 Nasıl Oynanır?
- Zıplamak ve oyunu başlatmak için **`SPACE` (Boşluk)** tuşuna basın.
- Boruların arasından geçerek en yüksek skoru yapmaya çalışın!

## 🛠️ Kurulum ve Derleme
Proje, geliştirme sürecini hızlandırmak için bir `Makefile` içermektedir. Terminal üzerinden oyunu derlemek ve çalıştırmak için tek yapmanız gereken:

```bash
mingw32-make