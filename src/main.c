// SPDX-License-Identifier: Beerware
/**
 * @file      main.c
 * @author    Michael Fitzmayer
 * @copyright "THE BEER-WARE LICENCE" (Revision 42)
 */

#define SDL_MAIN_HANDLED

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <SDL.h>
#include <esz.h>

static void key_down_callback(esz_window_t* window, esz_core_t* core);

#ifdef USE_LIBTMX
    #define MAP_FILE "res/maps/city.tmx"
#elif  USE_CUTE_TILED
    #define MAP_FILE "res/maps/city.json"
#endif

int main()
{
    const uint8_t*      keystate = esz_get_keyboard_state();
    esz_status          status;
    esz_window_t*       window   = NULL;
    esz_window_config_t config   = { 640, 360, 384, 216, false, true };
    esz_core_t*         core     = NULL;

    status = esz_create_window("Tau Ceti", &config, &window);
    if (ESZ_OK != status)
    {
        goto quit;
    }

    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);

    status = esz_init_core(&core);
    if (ESZ_OK != status)
    {
        goto quit;
    }

    if (ESZ_OK == esz_load_map(MAP_FILE, window, core))
    {
        esz_register_event_callback(EVENT_KEYDOWN, &key_down_callback, core);
        esz_set_camera_position(0.f, 475.f, false, window, core);
    }
    else
    {
        esz_deactivate_core(core);
    }

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

        esz_set_camera_position(camera_x, camera_y, true, window, core);
        status = esz_show_scene(window, core);
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

static void key_down_callback(esz_window_t* window, esz_core_t* core)
{
    switch (esz_get_keycode(core))
    {
        case SDLK_q:
            esz_deactivate_core(core);
            break;
        case SDLK_F4:
            esz_load_map(MAP_FILE, window, core);
            esz_set_camera_position(0.f, 475.f, false, window, core);
            break;
        case SDLK_F5:
            esz_unload_map(window, core);
            break;
        case SDLK_F11:
            esz_toggle_fullscreen(window);
            break;
    }
}
