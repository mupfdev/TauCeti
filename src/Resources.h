/**
 * @file Resources.h
 * @ingroup Resources
 */

#pragma once
#include <SDL.h>
#include <eszFW.h>

typedef struct Res_t
{
    SDL_bool    bGameIsRunning;
    SDL_bool    bThreadIsRunning;
    SDL_bool    bShowMessage;

    Audio      *pstAudio;
    Background *pstBg;
    Camera     *pstCamera;
    Entity     *pstEntity[5];
    Font       *pstFont;
    Map        *pstMap;
    Music      *pstMusic;
    Sprite     *pstSpPlayer;
    Sprite     *pstSpVehicles;
    Video      *pstVideo;

    double      dBgVelocityX;
    Uint32      u32PrngSeed;
    char        acMessage[OBJECT_NAME_LEN];
} Res;

void Free(Res *pstRes);
Sint8 Init(Res *pstRes);
