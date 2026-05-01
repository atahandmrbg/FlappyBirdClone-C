#include <SFML/Graphics.h>
#include <stdio.h>

int main()
{
    sfVideoMode mode = {800, 600, 32};
    sfRenderWindow *window;

    // Pencereyi oluşturuyoruz
    window = sfRenderWindow_create(mode, "Flappy Bird Clone", sfResize | sfClose, NULL);

    if (!window)
        return 1;

    while (sfRenderWindow_isOpen(window))
    {
        sfEvent event;
        while (sfRenderWindow_pollEvent(window, &event))
        {
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window);
        }

        sfRenderWindow_clear(window, sfRed); // Ekranı kırmızı yap
        sfRenderWindow_display(window);
    }

    sfRenderWindow_destroy(window);
    return 0;
}