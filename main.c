#include <SFML/Graphics.h>
#include <SFML/System.h>
#include <SFML/Audio.h> // YENİ: Ses kütüphanesi eklendi!
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

    // --- YENİ: SES (AUDIO) DEĞİŞKENLERİ ---
    sfSoundBuffer *flapBuffer = sfSoundBuffer_createFromFile("swoosh.wav");
    if (!flapBuffer)
    {
        printf("HATA: swoosh.wav dosyasi bulunamadi! Lutfen klasore ekleyin.\n");
        return 1;
    }
    sfSound *flapSound = sfSound_create();
    sfSound_setBuffer(flapSound, flapBuffer);

    // --- YAZI TİPİ VE SKOR AYARLARI ---
    sfFont *font = sfFont_createFromFile("impact.ttf");
    if (!font)
        return 1;

    sfText *scoreText = sfText_create();
    sfText_setFont(scoreText, font);
    sfText_setCharacterSize(scoreText, 30);
    sfText_setFillColor(scoreText, sfBlack);
    sfText_setOutlineColor(scoreText, sfWhite);
    sfText_setOutlineThickness(scoreText, 2.0f);
    sfText_setPosition(scoreText, (sfVector2f){20.0f, 20.0f});

    int score = 0;
    int highScore = 0;
    bool scoredThisPipe = false;
    char scoreString[100];
    bool gameStarted = false;

    sfClock *blinkClock = sfClock_create();
    bool showText = true;

    // --- ARKA PLAN (PARALLAX) DEĞİŞKENLERİ ---
    sfTexture *bgTexture = sfTexture_createFromFile("bg.png", NULL);
    if (!bgTexture)
        bgTexture = sfTexture_createFromFile("bg.jpg", NULL);
    if (!bgTexture)
        return 1;

    sfSprite *bgSprite1 = sfSprite_create();
    sfSprite *bgSprite2 = sfSprite_create();
    sfSprite_setTexture(bgSprite1, bgTexture, sfTrue);
    sfSprite_setTexture(bgSprite2, bgTexture, sfTrue);

    sfVector2u texSize = sfTexture_getSize(bgTexture);
    float scaleX = 800.0f / (float)texSize.x;
    float scaleY = 600.0f / (float)texSize.y;
    sfSprite_setScale(bgSprite1, (sfVector2f){scaleX, scaleY});
    sfSprite_setScale(bgSprite2, (sfVector2f){scaleX, scaleY});

    float bg1X = 0.0f;
    float bg2X = 800.0f;
    float bgSpeed = 1.0f;

    // --- KUŞ GÖRSELİ (SPRITE) DEĞİŞKENLERİ ---
    sfTexture *birdTex = sfTexture_createFromFile("bird.png", NULL);
    if (!birdTex)
        return 1;

    sfSprite *bird = sfSprite_create();
    sfSprite_setTexture(bird, birdTex, sfTrue);

    sfVector2u bSize = sfTexture_getSize(birdTex);
    sfSprite_setScale(bird, (sfVector2f){40.0f / bSize.x, 30.0f / bSize.y});
    sfSprite_setOrigin(bird, (sfVector2f){bSize.x / 2.0f, bSize.y / 2.0f});

    float birdX = 100.0f;
    float birdY = 300.0f;
    float velocity = 0.0f;
    float gravity = 0.5f;
    float jump_power = -8.0f;

    // --- BORU DEĞİŞKENLERİ ---
    sfRectangleShape *topPipe = sfRectangleShape_create();
    sfRectangleShape *bottomPipe = sfRectangleShape_create();
    sfColor pipeColor = sfColor_fromRGB(184, 115, 51);
    sfRectangleShape_setFillColor(topPipe, pipeColor);
    sfRectangleShape_setFillColor(bottomPipe, pipeColor);

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

            if (event.type == sfEvtKeyPressed && event.key.code == sfKeySpace)
            {
                if (!gameStarted)
                {
                    gameStarted = true;
                }
                velocity = jump_power;

                // YENİ: Boşluğa her basıldığında sesi çal!
                sfSound_play(flapSound);
            }
        }

        // --- ARKA PLAN HAREKETİ ---
        bg1X -= bgSpeed;
        bg2X -= bgSpeed;
        if (bg1X <= -800.0f)
            bg1X = 800.0f;
        if (bg2X <= -800.0f)
            bg2X = 800.0f;

        sfSprite_setPosition(bgSprite1, (sfVector2f){bg1X, 0});
        sfSprite_setPosition(bgSprite2, (sfVector2f){bg2X, 0});

        // --- FİZİK VE HAREKET ---
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

            float angle = velocity * 3.5f;
            if (angle < -25.0f)
                angle = -25.0f;
            if (angle > 90.0f)
                angle = 90.0f;
            sfSprite_setRotation(bird, angle);
        }
        else
        {
            sfSprite_setRotation(bird, 0.0f);
        }

        // --- POZİSYON VE BOYUT GÜNCELLEMELERİ ---
        sfSprite_setPosition(bird, (sfVector2f){birdX, birdY});

        sfRectangleShape_setSize(topPipe, (sfVector2f){pipeWidth, topPipeHeight});
        sfRectangleShape_setPosition(topPipe, (sfVector2f){pipeX, 0});

        float bottomPipeY = topPipeHeight + gapSize;
        float bottomPipeHeight = 600.0f - bottomPipeY;
        sfRectangleShape_setSize(bottomPipe, (sfVector2f){pipeWidth, bottomPipeHeight});
        sfRectangleShape_setPosition(bottomPipe, (sfVector2f){pipeX, bottomPipeY});

        // --- ÇARPIŞMA TESPİTİ ---
        sfFloatRect birdRect = sfSprite_getGlobalBounds(bird);
        sfFloatRect topRect = sfRectangleShape_getGlobalBounds(topPipe);
        sfFloatRect bottomRect = sfRectangleShape_getGlobalBounds(bottomPipe);

        if (gameStarted && (sfFloatRect_intersects(&birdRect, &topRect, NULL) ||
                            sfFloatRect_intersects(&birdRect, &bottomRect, NULL) ||
                            birdY + 15.0f > 600.0f || birdY - 15.0f < 0.0f))
        {
            if (score > highScore)
            {
                highScore = score;
            }

            birdY = 300.0f;
            velocity = 0.0f;
            pipeX = 800.0f;
            score = 0;
            scoredThisPipe = false;
            gameStarted = false;
        }

        // --- METİN VE YANIP SÖNME ---
        if (!gameStarted && score == 0)
        {
            sprintf(scoreString, "Baslamak Icin SPACE'e Bas!\nEn Yuksek: %d", highScore);
            sfText_setString(scoreText, scoreString);

            if (sfTime_asMilliseconds(sfClock_getElapsedTime(blinkClock)) > 500)
            {
                showText = !showText;
                sfClock_restart(blinkClock);
            }
        }
        else
        {
            sprintf(scoreString, "Skor: %d | En Yuksek: %d", score, highScore);
            sfText_setString(scoreText, scoreString);
            showText = true;
        }

        // --- ÇİZİM AŞAMASI ---
        sfRenderWindow_clear(window, sfBlack);

        sfRenderWindow_drawSprite(window, bgSprite1, NULL);
        sfRenderWindow_drawSprite(window, bgSprite2, NULL);
        sfRenderWindow_drawRectangleShape(window, topPipe, NULL);
        sfRenderWindow_drawRectangleShape(window, bottomPipe, NULL);
        sfRenderWindow_drawSprite(window, bird, NULL);

        if (showText)
        {
            sfRenderWindow_drawText(window, scoreText, NULL);
        }

        sfRenderWindow_display(window);
    }

    // Ses hafızasını temizle
    sfSound_destroy(flapSound);
    sfSoundBuffer_destroy(flapBuffer);

    sfClock_destroy(blinkClock);
    sfSprite_destroy(bgSprite1);
    sfSprite_destroy(bgSprite2);
    sfTexture_destroy(bgTexture);
    sfSprite_destroy(bird);
    sfTexture_destroy(birdTex);
    sfRectangleShape_destroy(topPipe);
    sfRectangleShape_destroy(bottomPipe);
    sfText_destroy(scoreText);
    sfFont_destroy(font);
    sfRenderWindow_destroy(window);
    return 0;
}