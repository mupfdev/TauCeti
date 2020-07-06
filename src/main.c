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
    esz_Status       status = ESZ_OK;
    esz_Window*      window = NULL;
    esz_WindowConfig config = { 640, 360, 384, 216, SDL_FALSE };
    esz_Core*        core_1 = NULL;

    status = esz_CreateWindow("Tau Ceti", &config, &window);

    status = esz_InitCore(&core_1);
    if (ESZ_OK != status)
    {
        goto quit;
    }

    esz_LoadMap("res/maps/city.tmx", window, core_1);

    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);
    esz_RegisterEventCallback(EVENT_KEYDOWN, &key_down_callback_1, (void*)core_1);

    while (esz_IsCoreActive(core_1))
    {
        Uint32 time_a = 0;
        Uint32 time_b = 0;

        esz_UpdateCore(window, core_1);

        esz_DrawFrame(&time_a, &time_b, window, core_1);
    }

quit:
    esz_UnloadMap(window, core_1);
    esz_DestroyCore(core_1);
    esz_DestroyWindow(window);

    if (ESZ_OK != status)
    {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

static void key_down_callback_1(void* window, void* core)
{
    switch (esz_GetKeycode(core))
    {
        case SDLK_f:
            esz_ToggleFullscreen(window);
            break;
        case SDLK_q:
            esz_DeactivateCore(core);
            break;
        case SDLK_F4:
            esz_LoadMap("res/maps/city.tmx", window, core);
            break;
        case SDLK_F5:
            esz_UnloadMap(window, core);
    }
}
