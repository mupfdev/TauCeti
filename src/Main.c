/**
 * @file Main.c
 * @ingroup Main
 * @defgroup Main
 * @author Michael Fitzmayer
 * @copyright "THE BEER-WARE LICENCE" (Revision 42)
 */

#ifdef _WIN32
#define SDL_MAIN_HANDLED
#endif

#include <SDL.h>
#include <eszFW.h>
#include "Render.h"
#include "Resources.h"
#include "World.h"

#ifndef __ANDROID__
int main()
{
#else
int SDL_main(int sArgC, char *pacArgV[])
{
    (void)sArgC; (void)pacArgV;
#endif
    Res         stRes          = { 0 };
    Sint8       s8ReturnValue  = 0;
    SDL_Thread *hUpdateWorldThread;

    s8ReturnValue = Init(&stRes);
    if (0 == s8ReturnValue)
    {
        hUpdateWorldThread = SDL_CreateThread(UpdateWorld, "UpdateWorldThread", &stRes);
        if (! hUpdateWorldThread)
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
