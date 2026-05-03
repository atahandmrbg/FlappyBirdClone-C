#include <SFML/Graphics.h>
#include <stdio.h>

int main() {
    sfVideoMode mode = {800, 600, 32};
    sfRenderWindow* window = sfRenderWindow_create(mode, "Flappy Bird Clone", sfResize | sfClose, NULL);

    if (!window) return 1;

    // 1. ÇOK ÖNEMLİ: Oyunun çok hızlı çalışmasını engellemek için FPS'i 60'a sabitliyoruz. 
    // Yoksa kuş ışık hızında yere düşer!
    sfRenderWindow_setFramerateLimit(window, 60);

    // --- 2. KUŞUMUZU OLUŞTURALIM ---
    sfRectangleShape* bird = sfRectangleShape_create();
    sfVector2f birdSize = {40.0f, 30.0f}; // Kuşun boyutu (Genişlik, Yükseklik)
    sfRectangleShape_setSize(bird, birdSize);
    sfRectangleShape_setFillColor(bird, sfYellow); // Flappy Bird sarısı :)

    // --- 3. FİZİK DEĞİŞKENLERİ ---
    float birdX = 100.0f;
    float birdY = 300.0f;     // Ekranın ortasından başlasın
    float velocity = 0.0f;    // Düşme/Çıkma hızı
    float gravity = 0.5f;     // Yerçekimi gücü (Sürekli aşağı çeker)
    float jump_power = -8.0f; // Zıplama gücü (Eksi olmalı çünkü ekranda Y ekseni yukarı doğru azalır)

    while (sfRenderWindow_isOpen(window)) {
        sfEvent event;
        while (sfRenderWindow_pollEvent(window, &event)) {
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window);

            // Boşluk tuşuna basılınca zıpla (İtki fiziği)
            if (event.type == sfEvtKeyPressed && event.key.code == sfKeySpace) {
                velocity = jump_power;
            }
        }

        // --- 4. FİZİK HESAPLAMALARI ---
        velocity += gravity; // Yerçekimi her an hızı artırır (kuşu aşağı çeker)
        birdY += velocity;   // Hız, kuşun Y pozisyonuna eklenir

        // Kuşun yeni pozisyonunu ayarla
        sfVector2f birdPos = {birdX, birdY};
        sfRectangleShape_setPosition(bird, birdPos);

        // --- 5. ÇİZİM AŞAMASI ---
        sfRenderWindow_clear(window, sfCyan); // Gökyüzü rengi (Açık Mavi)

        sfRenderWindow_drawRectangleShape(window, bird, NULL); // Kuşu ekrana çiz

        sfRenderWindow_display(window);
    }

    // Hafızayı temizle (C dilinin altın kuralı)
    sfRectangleShape_destroy(bird);
    sfRenderWindow_destroy(window);
    return 0;
}