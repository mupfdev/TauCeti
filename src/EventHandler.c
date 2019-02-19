/**
 * @file EventHandler.c
 * @ingroup EventHandler
 * @defgroup EventHandler
 * @author Michael Fitzmayer
 * @copyright "THE BEER-WARE LICENCE" (Revision 42)
 */

#include <SDL.h>
#include "EventHandler.h"

void FreeEvents(Events *pstEvents)
{
    free(pstEvents);
}

Sint8 InitEvents(Events **pstEvents)
{
    *pstEvents = SDL_calloc(sizeof(struct Events_t), sizeof(Sint8));
    if (! *pstEvents)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "InitEvents(): error allocating memory.\n");
        return -1;
    }

    (*pstEvents)->eDirection    = LEFT;
    (*pstEvents)->bIsOnPlatform = 1;

    return 0;
}

Sint8 UpdateEvents(Events *pstEvents)
{
    #ifdef __ANDROID__
    Sint32 s32TouchPosX  = 0;
    Sint32 s32TouchPosY  = 0;
    #endif

    while(SDL_PollEvent(&pstEvents->stEvent) != 0)
    {
        if (pstEvents->stEvent.type == SDL_QUIT)
        {
            return 0;
        }
        #ifndef __ANDROID__
        else if (SDL_KEYDOWN == pstEvents->stEvent.type)
        {
            switch(pstEvents->stEvent.key.keysym.sym)
            {
              case SDLK_q:
                  return 0;
              case SDLK_LEFT:
                  pstEvents->eDirection = LEFT;
                  pstEvents->bIsMoving  = 1;
                  break;
              case SDLK_RIGHT:
                  pstEvents->eDirection = RIGHT;
                  pstEvents->bIsMoving  = 1;
                  break;
              case SDLK_DOWN:
                  pstEvents->bIsCrouching = 1;
                  pstEvents->bIsMoving    = 0;
                  break;
              case SDLK_SPACE:
                  pstEvents->bIsJumping = 1;
                  break;
              case SDLK_f:
                  pstEvents->bToggleFullscreen = 1;
                  break;
              case SDLK_LSHIFT:
                  pstEvents->bIsCameraUnlocked = 1;
            }
        }
        else if (SDL_KEYUP == pstEvents->stEvent.type)
        {
            switch(pstEvents->stEvent.key.keysym.sym)
            {
              case SDLK_LEFT:
                  pstEvents->bIsMoving = 0;
                  break;
              case SDLK_RIGHT:
                  pstEvents->bIsMoving = 0;
                  break;
              case SDLK_DOWN:
                  pstEvents->bIsCrouching = 0;
                  pstEvents->bIsMoving    = 0;
                  break;
              case SDLK_LSHIFT:
                  pstEvents->bIsCameraUnlocked = 0;
                  break;
            }
        }
        #else // __ANDROID__
        else if (SDL_KEYDOWN == pstEvents->stEvent.type)
        {
            if (pstEvents->stEvent.key.keysym.sym == SDLK_AC_BACK)
            {
                return 0;
            }
        }
        else if (SDL_FINGERDOWN == pstEvents->stEvent.type)
        {
            s32TouchPosX = (Sint32)SDL_floor(pstEvents->stEvent.tfinger.x * pstEvents->s32WindowW);
            s32TouchPosY = (Sint32)SDL_floor(pstEvents->stEvent.tfinger.y * pstEvents->s32WindowH);

            if (s32TouchPosX < (pstEvents->s32WindowW / 2))
            {
                pstEvents->eDirection = LEFT;
                pstEvents->bIsMoving  = 1;
            }
            else
            {
                pstEvents->eDirection = RIGHT;
                pstEvents->bIsMoving  = 1;
            }

            if (s32TouchPosY > (pstEvents->s32WindowH - 48))
            {
                pstEvents->bIsCrouching = 1;
                pstEvents->bIsMoving    = 0;
            }
        }
        else if (SDL_FINGERUP == pstEvents->stEvent.type)
        {
            pstEvents->bIsCrouching = 0;
            pstEvents->bIsMoving    = 0;
        }
        else if (SDL_FINGERMOTION == pstEvents->stEvent.type)
        {
            double dDY = SDL_fabs(pstEvents->stEvent.tfinger.dy);
            if (0.03f < dDY)
            {
                pstEvents->bIsJumping = 1;
            }
        }
        else if (SDL_MULTIGESTURE == pstEvents->stEvent.type)
        {
            if (0.002f < SDL_fabs(pstEvents->stEvent.mgesture.dDist))
            {
                if (0 < pstEvents->stEvent.mgesture.dDist)
                {
                    pstEvents->bIsMoving  = 0;
                    pstEvents->bPinchOpen = 1;
                }
                else
                {
                    pstEvents->bIsMoving   = 0;
                    pstEvents->bPinchClose = 1;
                }
            }
        }
        else
        {
            pstEvents->bPinchOpen  = 0;
            pstEvents->bPinchClose = 0;
        }
        #endif // __ANDROID__
    }

    return 1;
}
