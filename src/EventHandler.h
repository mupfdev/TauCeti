/**
 * @file EventHandler.h
 */
#pragma once

#include <SDL.h>
#include <eszFW.h>

typedef struct Events_t
{
    Direction eDirection;
    SDL_bool  bIsCameraUnlocked;
    SDL_bool  bIsCrouching;
    SDL_bool  bIsJumping;
    SDL_bool  bIsOnPlatform;
    SDL_bool  bIsMoving;
    SDL_bool  bToggleFullscreen;
    SDL_Event stEvent;
    #ifdef __ANDROID__
    SDL_bool bPinchClose;
    SDL_bool bPinchOpen;
    Sint32   s32WindowW;
    Sint32   s32WindowH;
    #endif
} Events;

void  FreeEvents(Events* pstEvents);
Sint8 InitEvents(Events** pstEvents);
Sint8 UpdateEvents(Events* pstEvents);
