#include <SFML/Graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

int main()
{
    sfVideoMode mode = {800, 600, 32};
    sfRenderWindow *window = sfRenderWindow_create(mode, "Flappy Bird Clone", sfResize | sfClose, NULL);
    if (!window)
        return 1;
    sfRenderWindow_setFramerateLimit(window, 60);
    srand(time(NULL));

    // --- YAZI TİPİ VE SKOR AYARLARI ---
    sfFont *font = sfFont_createFromFile("impact.ttf");
    if (!font)
    {
        printf("HATA: impact.ttf dosyasi klasorde bulunamadi!\n");
        return 1;
    }

    sfText *scoreText = sfText_create();
    sfText_setFont(scoreText, font);
    sfText_setCharacterSize(scoreText, 30);

    // Yazı tasarımı: İçi SİYAH, Çerçevesi BEYAZ
    sfText_setFillColor(scoreText, sfBlack);
    sfText_setOutlineColor(scoreText, sfWhite);
    sfText_setOutlineThickness(scoreText, 2.0f);
    sfText_setPosition(scoreText, (sfVector2f){20.0f, 20.0f});

    int score = 0;
    int highScore = 0;
    bool scoredThisPipe = false;
    char scoreString[100];

    // --- YENİ: OYUN DURUMU DEĞİŞKENİ ---
    bool gameStarted = false; // Oyun başlangıçta duraklatılmış halde başlar

    // --- KUŞ DEĞİŞKENLERİ ---
    sfRectangleShape *bird = sfRectangleShape_create();
    sfVector2f birdSize = {40.0f, 30.0f};
    sfRectangleShape_setSize(bird, birdSize);
    sfRectangleShape_setFillColor(bird, sfYellow);

    float birdX = 100.0f;
    float birdY = 300.0f;
    float velocity = 0.0f;
    float gravity = 0.5f;
    float jump_power = -8.0f;

    // --- BORU DEĞİŞKENLERİ ---
    sfRectangleShape *topPipe = sfRectangleShape_create();
    sfRectangleShape *bottomPipe = sfRectangleShape_create();
    sfRectangleShape_setFillColor(topPipe, sfGreen);
    sfRectangleShape_setFillColor(bottomPipe, sfGreen);

    float pipeX = 800.0f;
    float pipeWidth = 60.0f;
    float pipeSpeed = 3.5f;
    float gapSize = 160.0f;
    float topPipeHeight = (rand() % 250) + 100.0f;

    while (sfRenderWindow_isOpen(window))
    {
        sfEvent event;
        while (sfRenderWindow_pollEvent(window, &event))
        {
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window);

            // Space tuşuna basıldığında
            if (event.type == sfEvtKeyPressed && event.key.code == sfKeySpace)
            {
                // Eğer oyun henüz başlamadıysa başlat
                if (!gameStarted)
                {
                    gameStarted = true;
                }
                // Kuşu zıplat (Oyun yeni başlamış olsa da, devam ediyor olsa da zıplar)
                velocity = jump_power;
            }
        }

        // --- FİZİK VE HAREKET (SADECE OYUN BAŞLADIYSA ÇALIŞIR) ---
        if (gameStarted)
        {
            velocity += gravity;
            birdY += velocity;
            pipeX -= pipeSpeed;

            if (pipeX < -pipeWidth)
            {
                pipeX = 800.0f;
                topPipeHeight = (rand() % 250) + 100.0f;
                scoredThisPipe = false;
            }

            if (birdX > pipeX + pipeWidth && !scoredThisPipe)
            {
                score++;
                scoredThisPipe = true;
            }
        }

        // --- POZİSYONLARI GÜNCELLE ---
        sfRectangleShape_setPosition(bird, (sfVector2f){birdX, birdY});
        sfRectangleShape_setSize(topPipe, (sfVector2f){pipeWidth, topPipeHeight});
        sfRectangleShape_setPosition(topPipe, (sfVector2f){pipeX, 0});

        float bottomPipeY = topPipeHeight + gapSize;
        float bottomPipeHeight = 600.0f - bottomPipeY;
        sfRectangleShape_setSize(bottomPipe, (sfVector2f){pipeWidth, bottomPipeHeight});
        sfRectangleShape_setPosition(bottomPipe, (sfVector2f){pipeX, bottomPipeY});

        // --- ÇARPIŞMA TESPİTİ ---
        sfFloatRect birdRect = sfRectangleShape_getGlobalBounds(bird);
        sfFloatRect topRect = sfRectangleShape_getGlobalBounds(topPipe);
        sfFloatRect bottomRect = sfRectangleShape_getGlobalBounds(bottomPipe);

        // Eğer oyundayken bir yere çarparsan
        if (gameStarted && (sfFloatRect_intersects(&birdRect, &topRect, NULL) ||
                            sfFloatRect_intersects(&birdRect, &bottomRect, NULL) ||
                            birdY > 600.0f || birdY < 0.0f))
        {

            if (score > highScore)
            {
                highScore = score;
            }

            // Oyunu başlangıç durumuna (bekleme moduna) sıfırla
            birdY = 300.0f;
            velocity = 0.0f;
            pipeX = 800.0f;
            score = 0;
            scoredThisPipe = false;
            gameStarted = false; // YANDIKTAN SONRA TEKRAR BEKLEME MODUNA GEÇ
        }

        // --- METNİ GÜNCELLE ---
        if (!gameStarted && score == 0)
        {
            // Başlangıç ekranındayken bilgi verelim
            sprintf(scoreString, "Baslamak Icin SPACE'e Bas!\nEn Yuksek: %d", highScore);
        }
        else
        {
            // Oyun içindeyken skoru göster
            sprintf(scoreString, "Skor: %d | En Yuksek: %d", score, highScore);
        }
        sfText_setString(scoreText, scoreString);

        // --- ÇİZİM AŞAMASI ---
        sfRenderWindow_clear(window, sfCyan);

        sfRenderWindow_drawRectangleShape(window, topPipe, NULL);
        sfRenderWindow_drawRectangleShape(window, bottomPipe, NULL);
        sfRenderWindow_drawRectangleShape(window, bird, NULL);
        sfRenderWindow_drawText(window, scoreText, NULL);

        sfRenderWindow_display(window);
    }

    // Hafıza temizliği
    sfText_destroy(scoreText);
    sfFont_destroy(font);
    sfRectangleShape_destroy(bird);
    sfRectangleShape_destroy(topPipe);
    sfRectangleShape_destroy(bottomPipe);
    sfRenderWindow_destroy(window);
    return 0;
}