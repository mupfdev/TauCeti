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

static void key_down_callback(esz_window* window, esz_core* core);

int main()
{
    const Uint8* keystate = esz_get_keyboard_state();

    esz_status        status;
    esz_window*       window = NULL;
    esz_window_config config = { 640, 360, 384, 216, SDL_FALSE, SDL_TRUE };
    esz_core*         core   = NULL;

    status = esz_create_window("Tau Ceti", &config, &window);
    if (ESZ_OK != status)
    {
        goto quit;
    }

    status = esz_init_core(&core);
    if (ESZ_OK != status)
    {
        goto quit;
    }

    esz_load_map("res/maps/city.tmx", window, core);
    esz_register_event_callback(EVENT_KEYDOWN, &key_down_callback, core);
    // DEBUG
    esz_set_camera_position(128.f, 500.f, SDL_FALSE, window, core);
    // DEBUG

    while (esz_is_core_active(core))
    {
        double camera_x = 0;
        double camera_y = 0;

        esz_update_core(window, core);

        if (keystate[SDL_SCANCODE_UP])
        {
            camera_y -= 0.3f;
        }
        if (keystate[SDL_SCANCODE_DOWN])
        {
            camera_y += 0.3f;
        }
        if (keystate[SDL_SCANCODE_LEFT])
        {
            camera_x -= 0.3f;
        }
        if (keystate[SDL_SCANCODE_RIGHT])
        {
            camera_x += 0.3f;
        }

        esz_set_camera_position(camera_x, camera_y, SDL_TRUE, window, core);
        status = esz_draw_frame(window, core);
        if (ESZ_ERROR_CRITICAL == status)
        {
            break;
        }
    }

quit:
    if (esz_is_map_loaded(core))
    {
        esz_unload_map(window, core);
    }
    if (core)
    {
        esz_destroy_core(core);
    }
    if (window)
    {
        esz_destroy_window(window);
    }

    if (ESZ_OK != status)
    {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

static void key_down_callback(esz_window* window, esz_core* core)
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
            // DEBUG
            esz_set_camera_position(128.f, 500.f, SDL_FALSE, window, core);
            // DEBUG
            break;
        case SDLK_F5:
            esz_unload_map(window, core);
    }
}
