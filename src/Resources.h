// SPDX-License-Identifier: Beerware
/**
 * @file Resources.h
 */
#pragma once

#include <SDL.h>
#include <eszFW.h>

typedef struct Config_t
{
    SDL_bool bMusic;
    SDL_bool bFullscreen;
    SDL_bool bWidescreen;
    Uint16   u16Width;
} Config;

typedef struct Res_t
{
    SDL_bool bGameIsRunning;
    SDL_bool bThreadIsRunning;
    SDL_bool bShowMessage;

    Audio*      pstAudio;
    Background* pstBg;
    Camera*     pstCamera;
    Entity*     pstEntity[5];
    Font*       pstFont;
    Map*        pstMap;
    Music*      pstMusic;
    Sprite*     pstSpPlayer;
    Sprite*     pstSpVehicles;
    Video*      pstVideo;

    double dBgVelocityX;
    Uint32 u32PrngSeed;
    char   acMessage[OBJECT_NAME_LEN];
} Res;

int ConfigHandler(void* pConfig, const char* pacSection, const char* pacName, const char* pacValue);
void  Free(Res* pstRes);
Sint8 Init(Config* pstConfig, Res* pstRes);
