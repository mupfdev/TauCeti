/**
 * @file      Resources.c
 * @author    Michael Fitzmayer
 * @copyright "THE BEER-WARE LICENCE" (Revision 42)
 */

#include <SDL.h>
#include <eszFW.h>
#include "Resources.h"

#define MATCH(s, n) (SDL_strncmp(pacSection, s, 20) == 0 && SDL_strncmp(pacName, n, 20) == 0)

int ConfigHandler(void* pConfig, const char* pacSection, const char* pacName, const char* pacValue)
{
    Config* pstConfig = (Config*)pConfig;
    Sint32  s32Value  = SDL_abs(SDL_atoi(pacValue));

    if (MATCH("Audio", "Music"))
    {
        pstConfig->bMusic = (SDL_bool)s32Value;
    }
    else if (MATCH("Video", "Fullscreen"))
    {
        pstConfig->bFullscreen = (SDL_bool)s32Value;
    }
    else if (MATCH("Video", "Widescreen"))
    {
        pstConfig->bWidescreen = (SDL_bool)s32Value;
    }
    else if (MATCH("Video", "Width"))
    {
        pstConfig->u16Width = (Uint16)s32Value;
    }
    else
    {
        return 0;
    }

    return 1;
}

void Free(Res* pstRes)
{
    FreeMusic(pstRes->pstMusic);
    FreeAudio(pstRes->pstAudio);
    FreeSprite(pstRes->pstSpPlayer);
    FreeSprite(pstRes->pstSpVehicles);
    FreeBackground(pstRes->pstBg);
    FreeMap(pstRes->pstMap);

    for (Uint8 u8Index = 0; u8Index <= 4; u8Index++)
    {
        FreeEntity(pstRes->pstEntity[u8Index]);
    }

    FreeFont(pstRes->pstFont);
    FreeCamera(pstRes->pstCamera);
    FreeVideo(pstRes->pstVideo);
}

Sint8 Init(Config* pstConfig, Res* pstRes)
{
    SDL_bool bFullscreen   = 0;
    Sint8    s8ReturnValue = 0;
    double   dPlayerSpawnX = 0.f;
    double   dPlayerSpawnY = 0.f;
    (void)pstConfig;

    const char* pacBgFileNames[3] = { "res/backgrounds/city-bg0.png",
                                      "res/backgrounds/city-bg1.png",
                                      "res/backgrounds/city-bg2.png" };

    s8ReturnValue = InitVideo("Tau Ceti", 640, 360, 384, 216, bFullscreen, &pstRes->pstVideo);
    if (-1 == s8ReturnValue)
    {
        return s8ReturnValue;
    }

    s8ReturnValue = InitCamera(&pstRes->pstCamera);
    if (-1 == s8ReturnValue)
    {
        return s8ReturnValue;
    };

    s8ReturnValue = InitFont("res/fonts/samus.ttf", &pstRes->pstFont);
    if (-1 == s8ReturnValue)
    {
        return s8ReturnValue;
    };
    SetFontColour(0xff, 0xff, 0xff, pstRes->pstFont);

    s8ReturnValue = InitEntity(0, 0, 64, 64, &pstRes->pstEntity[0]);
    if (-1 == s8ReturnValue)
    {
        return s8ReturnValue;
    };

    // Set up background vehicles.
    s8ReturnValue = InitEntity(-257, 192, 264, 104, &pstRes->pstEntity[1]);  // Truck.
    if (-1 == s8ReturnValue)
    {
        return s8ReturnValue;
    };
    SetDirection(LEFT, pstRes->pstEntity[1]);
    SetSpeed(3.5f, 2.f, pstRes->pstEntity[1]);
    MoveEntity(pstRes->pstEntity[1]);

    s8ReturnValue = InitEntity(1000, 300, 168, 64, &pstRes->pstEntity[2]);  // Police.
    if (-1 == s8ReturnValue)
    {
        return s8ReturnValue;
    };
    SetFrameOffset(0, 2, pstRes->pstEntity[2]);
    SetSpeed(45.f, 10.f, pstRes->pstEntity[2]);
    MoveEntity(pstRes->pstEntity[2]);

    s8ReturnValue = InitEntity(128, 150, 96, 64, &pstRes->pstEntity[3]);  // Misc 1.
    if (-1 == s8ReturnValue)
    {
        return s8ReturnValue;
    };
    SetFrameOffset(1, 3, pstRes->pstEntity[3]);
    SetSpeed(45.f, 8.f, pstRes->pstEntity[2]);
    SetDirection(LEFT, pstRes->pstEntity[3]);
    MoveEntity(pstRes->pstEntity[3]);

    s8ReturnValue = InitEntity(800, 416, 96, 64, &pstRes->pstEntity[4]);  // Misc 2.
    if (-1 == s8ReturnValue)
    {
        return s8ReturnValue;
    };
    SetFrameOffset(0, 3, pstRes->pstEntity[4]);
    SetSpeed(45.f, 5.f, pstRes->pstEntity[2]);
    MoveEntity(pstRes->pstEntity[4]);

    s8ReturnValue = InitMap("res/maps/city.tmx", "res/tilesets/city.png", 42, &pstRes->pstMap);
    SetTileAnimationSpeed(5.f, pstRes->pstMap);
    if (-1 == s8ReturnValue)
    {
        return s8ReturnValue;
    };

    for (Uint16 u16Index = 0; u16Index < pstRes->pstMap->u16ObjectCount; u16Index++)
    {
        Object* pstObject = &pstRes->pstMap->astObject[u16Index];

        if (IsObjectOfType("PlayerSpawn", pstObject))
        {
            dPlayerSpawnX = (double)pstObject->u32PosX;
            dPlayerSpawnY = (double)pstObject->u32PosY;
            break;
        }
    }
    SetPosition(dPlayerSpawnX, dPlayerSpawnY, pstRes->pstEntity[0]);
    SetSpawnPosition(dPlayerSpawnX, dPlayerSpawnY, pstRes->pstEntity[0]);

    s8ReturnValue = InitBackground(
        3,
        pacBgFileNames,
        pstRes->pstVideo->s32WindowWidth,
        BOTTOM,
        pstRes->pstVideo->pstRenderer,
        &pstRes->pstBg);
    if (-1 == s8ReturnValue)
    {
        return s8ReturnValue;
    };

    s8ReturnValue = InitSprite(
        "res/sprites/player.png",
        1536,
        128,
        0,
        0,
        &pstRes->pstSpPlayer,
        pstRes->pstVideo->pstRenderer);
    if (-1 == s8ReturnValue)
    {
        return s8ReturnValue;
    };

    s8ReturnValue = InitSprite(
        "res/sprites/vehicles.png",
        264,
        256,
        0,
        0,
        &pstRes->pstSpVehicles,
        pstRes->pstVideo->pstRenderer);
    if (-1 == s8ReturnValue)
    {
        return s8ReturnValue;
    };

    s8ReturnValue = InitAudio(&pstRes->pstAudio);
    if (-1 == s8ReturnValue)
    {
        return s8ReturnValue;
    };

    pstRes->u32PrngSeed = SDL_GetTicks();
    s8ReturnValue =
        InitMusic("res/music/Dreams_of_Vain.ogg", -1, &pstRes->pstMusic);
    if (-1 == s8ReturnValue)
    {
        return s8ReturnValue;
    };

    LockCamera(pstRes->pstCamera);
    PlayMusic(2000, pstRes->pstMusic);
    SetFrameOffset(0, 0, pstRes->pstEntity[0]);

    pstRes->bGameIsRunning   = 1;
    pstRes->bThreadIsRunning = 1;

    return s8ReturnValue;
}
