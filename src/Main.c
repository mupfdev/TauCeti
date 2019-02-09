/**
 * @file Main.c
 * @ingroup Main
 * @defgroup Main
 * @author Michael Fitzmayer
 * @copyright "THE BEER-WARE LICENCE" (Revision 42)
 */

#include <SDL.h>
#include <eszfw.h>

static Audio      *pstAudio;
static Background *pstBg;
static Camera     *pstCamera;
static Entity     *pstEntity[5];
static Map        *pstMap;
static Music      *pstMusic;
static Sprite     *pstSpPlayer;
static Sprite     *pstSpVehicles;
static Video      *pstVideo;

static double      dBgVelocityX;
static double      dDeltaTime = 0.f;

static Sint8 Init();
static Sint8 Render();
static void  Quit();

#ifndef __ANDROID__
int main()
{
#else
int SDL_main(Sint8 s8ArgC, char *pacArgV[])
{
    (void)s8ArgC;
    (void)pacArgV;
#endif
    Sint8     s8ReturnValue = 0;
    SDL_bool  bIsRunning    = 1;
    double    dTimeA        = 0.f;
    double    dTimeB        = 0.f;
    SDL_bool  bIsOnPlatform = 1;
    SDL_bool  bOrientation  = LEFT;
    SDL_bool  bIsCrouching  = 0;
    SDL_bool  bIsMoving     = 0;
    SDL_Event stEvent;

    s8ReturnValue = Init();
    s8ReturnValue = Render();
    if (-1 == s8ReturnValue)
    {
        bIsRunning = 0;
    }

    #ifdef __ANDROID__
    double dZoomLevel;
    double dZoomLevelMin;
    double dZoomLevelMax;
    Sint32 s32TouchPosX;
    Sint32 s32TouchPosY;
    Sint32 s32WindowW;
    Sint32 s32WindowH;

    if (bIsRunning)
    {
        dZoomLevel    = 0.f;
        dZoomLevelMin = (double)pstVideo->s32WindowHeight / (double)pstMap->u16Height;
        dZoomLevelMax = pstVideo->dInitialZoomLevel;
        s32TouchPosX  = 0;
        s32WindowW    = pstVideo->s32LogicalWindowWidth;
        s32WindowH    = pstVideo->s32LogicalWindowHeight;
    }
    #endif

    InitFPSLimiter(&dTimeA, &dTimeB, &dDeltaTime);
    while (bIsRunning)
    {
        // Limit framerate.
        LimitFramerate(60, &dTimeA, &dTimeB, &dDeltaTime);

        // Render scene.
        s8ReturnValue = Render();
        RenderScene(pstVideo->pstRenderer);

        if (-1 == s8ReturnValue)
        {
            bIsRunning = 0;
            continue;
        }

        // Reset entity flags.
        ResetEntity(pstEntity[0]);

        // Handle events.
        while(SDL_PollEvent(&stEvent) != 0)
        {
            if (stEvent.type == SDL_QUIT)
            {
                bIsRunning = 0;
            }
            #ifndef __ANDROID__
            else if (SDL_KEYDOWN == stEvent.type)
            {
                bIsCrouching = 0;

                switch(stEvent.key.keysym.sym)
                {
                  case SDLK_q:
                      bIsRunning = 0;
                      break;
                  case SDLK_LEFT:
                      bOrientation = LEFT;
                      bIsMoving    = 1;
                      break;
                  case SDLK_RIGHT:
                      bOrientation = RIGHT;
                      bIsMoving    = 1;
                      break;
                  case SDLK_DOWN:
                      bIsCrouching = 1;
                      bIsMoving    = 0;
                      break;
                  case SDLK_SPACE:
                      JumpEntity(3.5f, pstEntity[0]);
                      break;
                  case SDLK_f:
                      ToggleFullscreen(pstVideo);
                      break;
                }
            }
            else if (SDL_KEYUP == stEvent.type)
            {
                switch(stEvent.key.keysym.sym)
                {
                  case SDLK_LEFT:
                      bIsMoving = 0;
                      break;
                  case SDLK_RIGHT:
                      bIsMoving = 0;
                      break;
                  case SDLK_DOWN:
                      bIsCrouching = 0;
                      bIsMoving    = 0;
                      break;
                  case SDLK_LSHIFT:
                      LockCamera(pstCamera);
                      break;
                }
            }
            #else // __ANDROID__
            else if (SDL_KEYDOWN == stEvent.type)
            {
                if (stEvent.key.keysym.sym == SDLK_AC_BACK)
                {
                    bIsRunning = 0;
                }
            }
            else if (SDL_FINGERDOWN == stEvent.type)
            {
                s32TouchPosX = (Sint32)SDL_round(stEvent.tfinger.x * s32WindowW);
                s32TouchPosY = (Sint32)SDL_round(stEvent.tfinger.y * s32WindowH);

                if (s32TouchPosX < (s32WindowW / 2))
                {
                    bOrientation = LEFT;
                    bIsMoving    = 1;
                }
                else
                {
                    bOrientation = RIGHT;
                    bIsMoving    = 1;
                }

                if (s32TouchPosY > (s32WindowH - 32))
                {
                    bIsCrouching = 1;
                    bIsMoving    = 0;
                }
            }
            else if (SDL_FINGERUP == stEvent.type)
            {
                bIsCrouching = 0;
                bIsMoving    = 0;
            }
            else if (SDL_FINGERMOTION == stEvent.type)
            {
                double dDY = fabs(stEvent.tfinger.dy);
                if (0.03 < dDY)
                {
                    JumpEntity(4.7f, pstEntity[0]);
                }
            }
            else if (SDL_MULTIGESTURE == stEvent.type)
            {
                bIsMoving  = 0;
                dZoomLevel = pstVideo->dZoomLevel;
                if (0.002 < fabs(stEvent.mgesture.dDist))
                {
                    if (0 < stEvent.mgesture.dDist)
                    {
                        // Pinch open.
                        dZoomLevel += 5.f * dDeltaTime;
                    }
                    else
                    {
                        // Pinch close.
                        dZoomLevel -= 5.f * dDeltaTime;
                    }

                    if (dZoomLevel <= dZoomLevelMin)
                    {
                        dZoomLevel = dZoomLevelMin;
                    }
                    if (dZoomLevel >= dZoomLevelMax)
                    {
                        dZoomLevel = dZoomLevelMax;
                    }

                    SetZoomLevel(dZoomLevel, pstVideo);
                }
            }
            #endif // __ANDROID__
        }

        // Set the player's idle animation.
        if (! IsEntityMoving(pstEntity[0]))
        {
            AnimateEntity(1, pstEntity[0]);
            SetFrameOffset(0, 1, pstEntity[0]);
            SetAnimation(0, 3, 5.f, pstEntity[0]);
        }

        // Move player entity.
        if (bIsMoving)
        {
            AnimateEntity(1, pstEntity[0]);
            MoveEntityFull(
                bOrientation, 6.0, 3.0, 0, 15,
                24.f,
                0,
                pstEntity[0]);
        }

        if (bIsCrouching)
        {
            AnimateEntity(0, pstEntity[0]);
            SetFrameOffset(0, 0, pstEntity[0]);
            SetOrientation(bOrientation, pstEntity[0]);
            SetAnimation(22, 22, 0.f, pstEntity[0]);
        }

        // ** Game logic start **

        // Collision detection.
        if (IsOnTileOfType(
                "Platform", pstEntity[0]->dPosX, pstEntity[0]->dPosY,
                pstEntity[0]->u16Height, pstMap))
        {
            bIsOnPlatform = 1;
        }
        else
        {
            bIsOnPlatform = 0;
        }

        if (pstEntity[0]->dPosX < 0.f || pstEntity[0]->dPosX > pstMap->u16Width)
        {
            bIsOnPlatform = 1;
        }

        if (! bIsOnPlatform)
        {
            SetFrameOffset(0, 1, pstEntity[0]);
            if (IsEntityRising(pstEntity[0]))
            {
                SetAnimation(12, 15, 12.5f, pstEntity[0]);
            }
            else
            {
                SetAnimation(15, 15, 12.5f, pstEntity[0]);
            }
            DropEntity(pstEntity[0]);
        }

        // Update game entities.
        UpdateEntity(dDeltaTime, pstMap->dGravitation, pstMap->u8MeterInPixel, pstEntity[0]);
        for (Uint8 u8Index = 1; u8Index <= 3; u8Index++)
        {
            UpdateEntity(dDeltaTime, 0, 24, pstEntity[u8Index]);
        }

        // Follow player entity and set camera boudnaries to map size.
        SetCameraTargetEntity(
            pstVideo->s32LogicalWindowWidth,
            pstVideo->s32LogicalWindowHeight,
            pstEntity[0],
            pstCamera);

        if (SetCameraBoundariesToMapSize(
            pstVideo->s32LogicalWindowWidth,
            pstVideo->s32LogicalWindowHeight,
            pstMap->u16Width,
            pstMap->u16Height,
            pstCamera))
        {
            // Do not move background when camera hits boundaries.
            dBgVelocityX = 0;
        }
        else
        {
            dBgVelocityX = pstEntity[0]->dVelocityX;
        }

        if (pstEntity[0]->dPosX <= (pstEntity[0]->u16Width / 3))
        {
            pstEntity[0]->dPosX = (pstEntity[0]->u16Width / 3);
        }

        if (pstEntity[0]->dPosY > (pstMap->u16Height + pstEntity[0]->u16Height))
        {
            ResetEntityToSpawnPosition(pstEntity[0]);
        }

        // Move background vehicles.
        for (Uint8 u8Index = 1; u8Index <= 3; u8Index++)
        {
            ConnectHorizontalMapEndsForEntity(pstMap->u16Width, pstEntity[u8Index]);
        }

        // ** Game logic end **
    }

    Quit();

    if (-1 == s8ReturnValue)
    {
        return EXIT_FAILURE;
    }
    else
    {
        return EXIT_SUCCESS;
    }
}

static Sint8 Init()
{
    SDL_bool bFullscreen   = 0;
    Sint8    s8ReturnValue = 0;

    const char *pacBgFileNames[3] = {
        "res/backgrounds/city-bg0.png",
        "res/backgrounds/city-bg1.png",
        "res/backgrounds/city-bg2.png"
    };

    s8ReturnValue = InitVideo("Tau Ceti", 640, 360, 384, 216, bFullscreen, &pstVideo);
    RETURN_ON_ERROR(s8ReturnValue);

    s8ReturnValue = InitCamera(&pstCamera);
    RETURN_ON_ERROR(s8ReturnValue);

    s8ReturnValue = InitEntity(112, 192, 64, 64, &pstEntity[0]);
    SetSpawnPosition(112, 192, pstEntity[0]);
    RETURN_ON_ERROR(s8ReturnValue);

    // Set up background vehicles.
    s8ReturnValue = InitEntity(-257, 192, 264, 104, &pstEntity[1]); // Truck.
    RETURN_ON_ERROR(s8ReturnValue);
    SetOrientation(LEFT, pstEntity[1]);
    SetSpeed(5.f, 2.f, pstEntity[1]);
    MoveEntity(pstEntity[1]);

    s8ReturnValue = InitEntity(1000, 300, 168, 64, &pstEntity[2]);  // Police.
    RETURN_ON_ERROR(s8ReturnValue);
    SetFrameOffset(0, 2, pstEntity[2]);
    SetSpeed(50.f, 10.f, pstEntity[2]);
    MoveEntity(pstEntity[2]);

    s8ReturnValue = InitEntity(128, 150, 96, 64, &pstEntity[3]);    // Misc 1.
    RETURN_ON_ERROR(s8ReturnValue);
    SetFrameOffset(1, 3, pstEntity[3]);
    SetSpeed(50.f, 8.f, pstEntity[2]);
    SetOrientation(LEFT, pstEntity[3]);
    MoveEntity(pstEntity[3]);

    s8ReturnValue = InitEntity(800, 416, 96, 64, &pstEntity[4]);    // Misc 2.
    RETURN_ON_ERROR(s8ReturnValue);
    SetFrameOffset(0, 3, pstEntity[4]);
    SetSpeed(50.f, 5.f, pstEntity[2]);
    MoveEntity(pstEntity[4]);

    s8ReturnValue = InitMap("res/maps/city.tmx", "res/tilesets/city.png", 32, &pstMap);
    SetTileAnimationSpeed(5.f, pstMap);
    RETURN_ON_ERROR(s8ReturnValue);

    s8ReturnValue = InitBackground(
        3, pacBgFileNames, pstVideo->s32WindowWidth, BOTTOM,
        pstVideo->pstRenderer, &pstBg);
    RETURN_ON_ERROR(s8ReturnValue);

    s8ReturnValue = InitSprite(
        "res/sprites/player.png", 1536, 128, 0, 0, &pstSpPlayer, pstVideo->pstRenderer);
    RETURN_ON_ERROR(s8ReturnValue);

    s8ReturnValue = InitSprite(
        "res/sprites/vehicles.png", 264, 256, 0, 0, &pstSpVehicles, pstVideo->pstRenderer);
    RETURN_ON_ERROR(s8ReturnValue);

    s8ReturnValue = InitAudio(&pstAudio);
    RETURN_ON_ERROR(s8ReturnValue);

    s8ReturnValue = InitMusic(
        "res/music/FutureAmbient_3.ogg", -1,
        &pstMusic);
    RETURN_ON_ERROR(s8ReturnValue);

    LockCamera(pstCamera);
    PlayMusic(2000, pstMusic);
    SetFrameOffset(0, 0, pstEntity[0]);

    return s8ReturnValue;
}

static Sint8 Render()
{
    int s8ReturnValue = 0;

    s8ReturnValue = DrawBackground(
        pstEntity[0]->bOrientation,
        pstVideo->s32LogicalWindowHeight,
        pstCamera->dPosY,
        dBgVelocityX,
        pstVideo->pstRenderer,
        pstBg);

    for (Uint8 u8Index = 1; u8Index <= 3; u8Index++)
    {
        s8ReturnValue = DrawEntity(
            pstEntity[u8Index],
            pstCamera,
            pstSpVehicles,
            pstVideo->pstRenderer);
    }

    s8ReturnValue = DrawMap(
        0, 1, 1, "BG",
        pstCamera->dPosX,
        pstCamera->dPosY,
        dDeltaTime,
        pstMap,
        pstVideo->pstRenderer);

    s8ReturnValue = DrawEntity(
        pstEntity[0],
        pstCamera,
        pstSpPlayer,
        pstVideo->pstRenderer);

    s8ReturnValue = DrawMap(
        1, 0, 0, "FG",
        pstCamera->dPosX,
        pstCamera->dPosY,
        dDeltaTime,
        pstMap,
        pstVideo->pstRenderer);

    return s8ReturnValue;
}

static void Quit()
{
    FreeMusic(pstMusic);
    FreeAudio(pstAudio);
    FreeSprite(pstSpPlayer);
    FreeSprite(pstSpVehicles);
    FreeBackground(pstBg);
    FreeMap(pstMap);
    FreeEntity(pstEntity[0]);
    FreeCamera(pstCamera);
    FreeVideo(pstVideo);
    SDL_Quit();
}
