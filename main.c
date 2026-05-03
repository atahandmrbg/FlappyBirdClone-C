#include <SFML/Graphics.h>
#include <stdio.h>
#include <stdlib.h> // rand() fonksiyonu için (Rastgele sayı)
#include <time.h>   // Rastgelelik tohumu için

int main() {
    sfVideoMode mode = {800, 600, 32};
    sfRenderWindow* window = sfRenderWindow_create(mode, "Flappy Bird Clone", sfResize | sfClose, NULL);
    if (!window) return 1;
    sfRenderWindow_setFramerateLimit(window, 60);

    // Rastgele sayılar için zamanı tohum olarak veriyoruz (Yoksa hep aynı borular gelir)
    srand(time(NULL)); 

    // --- KUŞ DEĞİŞKENLERİ ---
    sfRectangleShape* bird = sfRectangleShape_create();
    sfVector2f birdSize = {40.0f, 30.0f};
    sfRectangleShape_setSize(bird, birdSize);
    sfRectangleShape_setFillColor(bird, sfYellow);

    float birdX = 100.0f;
    float birdY = 300.0f;
    float velocity = 0.0f;
    float gravity = 0.5f;
    float jump_power = -8.0f;

    // --- BORU (ENGEL) DEĞİŞKENLERİ ---
    sfRectangleShape* topPipe = sfRectangleShape_create();
    sfRectangleShape* bottomPipe = sfRectangleShape_create();
    sfRectangleShape_setFillColor(topPipe, sfGreen);
    sfRectangleShape_setFillColor(bottomPipe, sfGreen);

    float pipeX = 800.0f;       // Boru sağdan başlasın
    float pipeWidth = 60.0f;    // Borunun kalınlığı
    float pipeSpeed = 3.0f;     // Borunun sola kayma hızı
    float gapSize = 160.0f;     // İki boru arasındaki boşluk (Kuşun geçeceği yer)
    
    // Üst borunun yüksekliğini rastgele belirle (100 ile 350 piksel arası)
    float topPipeHeight = (rand() % 250) + 100.0f; 

    while (sfRenderWindow_isOpen(window)) {
        sfEvent event;
        while (sfRenderWindow_pollEvent(window, &event)) {
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window);

            if (event.type == sfEvtKeyPressed && event.key.code == sfKeySpace) {
                velocity = jump_power;
            }
        }

        // --- FİZİK VE HAREKET ---
        velocity += gravity;
        birdY += velocity;

        // Boruyu sola doğru kaydır
        pipeX -= pipeSpeed;

        // Boru ekrandan tamamen çıkarsa, sağa geri al ve yüksekliklerini RASTGELE değiştir
        if (pipeX < -pipeWidth) {
            pipeX = 800.0f;
            topPipeHeight = (rand() % 250) + 100.0f; 
        }

        // --- POZİSYONLARI GÜNCELLE ---
        sfRectangleShape_setPosition(bird, (sfVector2f){birdX, birdY});

        // Üst borunun boyutunu ve yerini ayarla
        sfRectangleShape_setSize(topPipe, (sfVector2f){pipeWidth, topPipeHeight});
        sfRectangleShape_setPosition(topPipe, (sfVector2f){pipeX, 0});

        // Alt borunun boyutunu ve yerini ayarla
        float bottomPipeY = topPipeHeight + gapSize;
        float bottomPipeHeight = 600.0f - bottomPipeY;
        sfRectangleShape_setSize(bottomPipe, (sfVector2f){pipeWidth, bottomPipeHeight});
        sfRectangleShape_setPosition(bottomPipe, (sfVector2f){pipeX, bottomPipeY});

        // --- ÇARPIŞMA TESPİTİ (COLLISION) ---
        // Kuşun, üst borunun ve alt borunun ekrandaki kapladığı dörtgen alanı alıyoruz
        sfFloatRect birdRect = sfRectangleShape_getGlobalBounds(bird);
        sfFloatRect topRect = sfRectangleShape_getGlobalBounds(topPipe);
        sfFloatRect bottomRect = sfRectangleShape_getGlobalBounds(bottomPipe);

        // Kesişim var mı? (Kuş borulara değdi mi?) VEYA (Kuş yere/tavana çarptı mı?)
        if (sfFloatRect_intersects(&birdRect, &topRect, NULL) || 
            sfFloatRect_intersects(&birdRect, &bottomRect, NULL) ||
            birdY > 600.0f || birdY < 0.0f) {
            
            printf("OYUN BITTI! Carpisma gerceklesti.\n");
            
            // Şimdilik oyunu sıfırlayalım (Başa sarsın)
            birdY = 300.0f;
            velocity = 0.0f;
            pipeX = 800.0f;
        }

        // --- ÇİZİM AŞAMASI ---
        sfRenderWindow_clear(window, sfCyan);
        
        sfRenderWindow_drawRectangleShape(window, topPipe, NULL);
        sfRenderWindow_drawRectangleShape(window, bottomPipe, NULL);
        sfRenderWindow_drawRectangleShape(window, bird, NULL);
        
        sfRenderWindow_display(window);
    }

    // Hafıza temizliği
    sfRectangleShape_destroy(bird);
    sfRectangleShape_destroy(topPipe);
    sfRectangleShape_destroy(bottomPipe);
    sfRenderWindow_destroy(window);
    return 0;
}