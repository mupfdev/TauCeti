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
    Audio_FreeMusic(pstRes->pstMusic);
    Audio_Free(pstRes->pstAudio);
    Entity_FreeSprite(pstRes->pstSpPlayer);
    Entity_FreeSprite(pstRes->pstSpVehicles);
    Background_Free(pstRes->pstBg);
    Map_Free(pstRes->pstMap);

    for (Uint8 u8Index = 0; u8Index <= 4; u8Index++)
    {
        Entity_Free(pstRes->pstEntity[u8Index]);
    }

    Font_Free(pstRes->pstFont);
    Entity_FreeCamera(pstRes->pstCamera);
    Video_Free(pstRes->pstVideo);
}

Sint8 Init(Config* pstConfig, Res* pstRes)
{
    SDL_bool bFullscreen   = 0;
    Sint8    s8ReturnValue = 0;
    double   dPlayerSpawnX = 0.f;
    double   dPlayerSpawnY = 0.f;
    (void)pstConfig;

    const char* pacBgFileNames[3] =
        { "res/backgrounds/city-bg0.png",
          "res/backgrounds/city-bg1.png",
          "res/backgrounds/city-bg2.png" };

    s8ReturnValue = Video_Init("Tau Ceti", 640, 360, 384, 216, bFullscreen, &pstRes->pstVideo);
    if (-1 == s8ReturnValue)
    {
        return s8ReturnValue;
    }

    s8ReturnValue = Entity_InitCamera(&pstRes->pstCamera);
    if (-1 == s8ReturnValue)
    {
        return s8ReturnValue;
    };

    s8ReturnValue = Font_Init("res/fonts/samus.ttf", &pstRes->pstFont);
    if (-1 == s8ReturnValue)
    {
        return s8ReturnValue;
    };
    Font_SetColour(0xff, 0xff, 0xff, pstRes->pstFont);

    s8ReturnValue = Entity_Init(0, 0, 64, 64, &pstRes->pstEntity[0]);
    if (-1 == s8ReturnValue)
    {
        return s8ReturnValue;
    };

    // Set up background vehicles.
    s8ReturnValue = Entity_Init(-257, 192, 264, 104, &pstRes->pstEntity[1]);  // Truck.
    if (-1 == s8ReturnValue)
    {
        return s8ReturnValue;
    };
    Entity_SetDirection(LEFT, pstRes->pstEntity[1]);
    Entity_SetSpeed(3.5f, 2.f, pstRes->pstEntity[1]);
    Entity_Move(pstRes->pstEntity[1]);

    s8ReturnValue = Entity_Init(1000, 300, 168, 64, &pstRes->pstEntity[2]);  // Police.
    if (-1 == s8ReturnValue)
    {
        return s8ReturnValue;
    };
    Entity_SetFrameOffset(0, 2, pstRes->pstEntity[2]);
    Entity_SetSpeed(45.f, 10.f, pstRes->pstEntity[2]);
    Entity_Move(pstRes->pstEntity[2]);

    s8ReturnValue = Entity_Init(128, 150, 96, 64, &pstRes->pstEntity[3]);  // Misc 1.
    if (-1 == s8ReturnValue)
    {
        return s8ReturnValue;
    };
    Entity_SetFrameOffset(1, 3, pstRes->pstEntity[3]);
    Entity_SetSpeed(45.f, 8.f, pstRes->pstEntity[2]);
    Entity_SetDirection(LEFT, pstRes->pstEntity[3]);
    Entity_Move(pstRes->pstEntity[3]);

    s8ReturnValue = Entity_Init(800, 416, 96, 64, &pstRes->pstEntity[4]);  // Misc 2.
    if (-1 == s8ReturnValue)
    {
        return s8ReturnValue;
    };
    Entity_SetFrameOffset(0, 3, pstRes->pstEntity[4]);
    Entity_SetSpeed(45.f, 5.f, pstRes->pstEntity[2]);
    Entity_Move(pstRes->pstEntity[4]);

    s8ReturnValue = Map_Init("res/maps/city.tmx", "res/tilesets/city.png", 42, &pstRes->pstMap);
    Map_SetTileAnimationSpeed(5.f, pstRes->pstMap);
    if (-1 == s8ReturnValue)
    {
        return s8ReturnValue;
    };

    for (Uint16 u16Index = 0; u16Index < pstRes->pstMap->u16ObjectCount; u16Index++)
    {
        Object* pstObject = &pstRes->pstMap->astObject[u16Index];

        if (Map_IsObjectOfType("PlayerSpawn", pstObject))
        {
            dPlayerSpawnX = (double)pstObject->u32PosX;
            dPlayerSpawnY = (double)pstObject->u32PosY;
            break;
        }
    }
    Entity_SetPosition(dPlayerSpawnX, dPlayerSpawnY, pstRes->pstEntity[0]);
    Entity_SetSpawnPosition(dPlayerSpawnX, dPlayerSpawnY, pstRes->pstEntity[0]);

    s8ReturnValue = Background_Init(
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

    s8ReturnValue = Entity_InitSprite(
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

    s8ReturnValue = Entity_InitSprite(
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

    s8ReturnValue = Audio_Init(&pstRes->pstAudio);
    if (-1 == s8ReturnValue)
    {
        return s8ReturnValue;
    };

    pstRes->u32PrngSeed = SDL_GetTicks();
    s8ReturnValue =
        Audio_InitMusic("res/music/Dreams_of_Vain.ogg", -1, &pstRes->pstMusic);
    if (-1 == s8ReturnValue)
    {
        return s8ReturnValue;
    };

    Entity_LockCamera(pstRes->pstCamera);
    Audio_PlayMusic(2000, pstRes->pstMusic);
    Entity_SetFrameOffset(0, 0, pstRes->pstEntity[0]);

    pstRes->bGameIsRunning   = 1;
    pstRes->bThreadIsRunning = 1;

    return s8ReturnValue;
}
