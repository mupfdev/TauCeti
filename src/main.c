// SPDX-License-Identifier: Beerware
/**
 * @file      main.c
 * @author    Michael Fitzmayer
 * @copyright "THE BEER-WARE LICENCE" (Revision 42)
 */

#define SDL_MAIN_HANDLED

#include <stdlib.h>
#include <SDL.h>
#include <esz.h>

static void key_down_callback_1(void* window, void* core);

int main()
{
    esz_status        status = ESZ_OK;
    esz_window*       window = NULL;
    esz_window_config config = { 640, 360, 384, 216, SDL_FALSE, SDL_FALSE };
    esz_core*         core_1 = NULL;

    status = esz_create_window("Tau Ceti", &config, &window);

    status = esz_init_core(&core_1);
    if (ESZ_OK != status)
    {
        goto quit;
    }

    // DEBUG!
    core_1->camera.pos_x = 128;
    core_1->camera.pos_y = 500;
    // DEBUG!

    esz_load_map("res/maps/city.tmx", window, core_1);

    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);
    esz_register_event_callback(EVENT_KEYDOWN, &key_down_callback_1, (void*)core_1);

    while (esz_is_core_active(core_1))
    {
        Uint32 time_a = 0;
        Uint32 time_b = 0;

        esz_update_core(window, core_1);

        esz_draw_frame(&time_a, &time_b, window, core_1);
    }

quit:
    esz_unload_map(window, core_1);
    esz_destroy_core(core_1);
    esz_destroy_window(window);

    if (ESZ_OK != status)
    {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

static void key_down_callback_1(void* window, void* core)
{
    switch (esz_get_keycode(core))
    {
        case SDLK_f:
            esz_toggle_fullscreen(window);
            break;
        case SDLK_q:
            esz_deactivate_core(core);
            break;
        case SDLK_F4:
            esz_load_map("res/maps/city.tmx", window, core);
            break;
        case SDLK_F5:
            esz_unload_map(window, core);
    }
}
