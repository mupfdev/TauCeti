// SPDX-License-Identifier: Beerware
/**
 * @file      main.c
 * @author    Michael Fitzmayer
 * @copyright "THE BEER-WARE LICENCE" (Revision 42)
 */

#include <SDL.h>
#include <eszFW.h>
#include <ini.h>
#include "Render.h"
#include "Resources.h"
#include "World.h"

int main(int argc, char* argv[])
{
    char        pacIni[20]    = "res/default.ini";
    Config      stConfig      = { 0 };
    Res         stRes         = { 0 };
    Sint8       s8ReturnValue = 0;
    SDL_Thread* hUpdateWorldThread;

    SDL_SetMainReady();

    if (argc <= 1)
    {
        #ifdef __ANDROID__
        SDL_strlcpy(pacIni, "res/android.ini", 15);
        #endif
    }
    else
    {
        SDL_strlcpy(pacIni, argv[1], 20);
    }
    SDL_Log("Load INI configuration file: %s.\n", pacIni);
    ini_parse(pacIni, ConfigHandler, &stConfig);
    // TODO check if ini_parse fails!
    
    //
    s8ReturnValue = Init(&stConfig, &stRes);
    if (0 == s8ReturnValue)
    {
        hUpdateWorldThread = SDL_CreateThread(UpdateWorld, "UpdateWorldThread", &stRes);
        if (!hUpdateWorldThread)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s\n", SDL_GetError());
            stRes.bGameIsRunning = 0;
        }
    }
    else
    {
        stRes.bGameIsRunning = 0;
    }

    while (stRes.bGameIsRunning)
    {
        UpdateZoomLevel(&stRes);
        s8ReturnValue = Render(&stRes);

        if (s8ReturnValue != 0)
        {
            stRes.bGameIsRunning = 0;
            continue;
        }
    }

    SDL_WaitThread(hUpdateWorldThread, NULL);
    Free(&stRes);
    SDL_Quit();

    if (-1 == s8ReturnValue)
    {
        return 1;
    }

    return 0;
}
