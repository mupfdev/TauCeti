/**
 * @file Resources.h
 * @ingroup Resources
 */

#ifndef _RESOURCES_H_
#define _RESOURCES_H_

#include <SDL.h>
#include <eszFW.h>

typedef struct Res_t
{
    SDL_bool    bGameIsRunning;
    SDL_bool    bThreadIsRunning;

    Audio      *pstAudio;
    Background *pstBg;
    Camera     *pstCamera;
    Entity     *pstEntity[5];
    Map        *pstMap;
    Music      *pstMusic;
    Sprite     *pstSpPlayer;
    Sprite     *pstSpVehicles;
    Video      *pstVideo;

    double      dBgVelocityX;
    Uint32      u32PrngSeed;
} Res;

void Free(Res *pstRes);
Sint8 Init(Res *pstRes);

#endif // _RESOURCES_H_
