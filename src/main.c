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

static void CoreStartedCallback_(void* core);
static void KeyDownCallback_(void* core);

int main()
{
    esz_Status status = ESZ_OK;
    esz_Core*  core   = NULL;
    esz_Config config = { 640, 360, 384, 216, SDL_FALSE };

    status = esz_InitCore(&config, &core);
    if (ESZ_OK != status)
    {
        goto quit;
    }

    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);
    esz_RegisterEventCallback(EVENT_CORE_STARTED, &CoreStartedCallback_, (void*)core);
    esz_RegisterEventCallback(EVENT_KEYDOWN,      &KeyDownCallback_,     (void*)core);

    status = esz_StartCore("Tau Ceti", core);
    if (ESZ_OK != status)
    {
        goto quit;
    }

quit:
    if (ESZ_OK != status)
    {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

static void CoreStartedCallback_(void* core)
{
    esz_LoadMap("res/maps/city.tmx", core);
}

static void KeyDownCallback_(void* core)
{
    switch (esz_GetKeycode(core))
    {
        case SDLK_f:
            esz_ToggleFullscreen(core);
            break;
        case SDLK_q:
            esz_ExitCore(core);
            break;
        case SDLK_F4:
            esz_LoadMap("res/maps/city.tmx", core);
            break;
        case SDLK_F5:
            esz_UnloadMap(core);
            break;
    }
}
