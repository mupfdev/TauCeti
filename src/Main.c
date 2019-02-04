/**
 * @file Main.c
 * @ingroup Main
 * @defgroup Main
 * @author Michael Fitzmayer
 * @copyright "THE BEER-WARE LICENCE" (Revision 42)
 */

#include <SDL.h>
#include <eszfw.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>

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

static int  Init();
static int  Render();
static void Quit();

#ifndef __ANDROID__
int main(int sArgC, char *pacArgV[])
#else
int SDL_main(int sArgC, char *pacArgV[])
#endif
{
    int        sReturnValue  = 0;
    bool       bIsRunning    = true;
    double     dTimeA        = 0.f;
    double     dTimeB        = 0.f;
    bool       bIsOnPlatform = true;
    bool       bOrientation  = LEFT;
    bool       bIsCrouching  = false;
    bool       bIsMoving     = false;
    SDL_Event  stEvent;

    (void)sArgC;
    (void)pacArgV;

    sReturnValue = Init();
    sReturnValue = Render();
    if (-1 == sReturnValue)
    {
        bIsRunning = false;
    }

    #ifdef __ANDROID__
    double  dZoomLevel;
    double  dZoomLevelMin;
    double  dZoomLevelMax;
    int32_t s32TouchPosX;
    int32_t s32TouchPosY;
    int32_t s32WindowW;
    int32_t s32WindowH;

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
        sReturnValue = Render();
        RenderScene(pstVideo->pstRenderer);

        if (-1 == sReturnValue)
        {
            bIsRunning = false;
            continue;
        }

        // Reset entity flags.
        ResetEntity(pstEntity[0]);

        // Handle events.
        while(SDL_PollEvent(&stEvent) != 0)
        {
            if (stEvent.type == SDL_QUIT)
            {
                bIsRunning = false;
            }
            #ifndef __ANDROID__
            else if (SDL_KEYDOWN == stEvent.type)
            {
                bIsCrouching = false;

                switch(stEvent.key.keysym.sym)
                {
                  case SDLK_q:
                      bIsRunning = false;
                      break;
                  case SDLK_LEFT:
                      bOrientation = LEFT;
                      bIsMoving    = true;
                      break;
                  case SDLK_RIGHT:
                      bOrientation = RIGHT;
                      bIsMoving    = true;
                      break;
                  case SDLK_DOWN:
                      bIsCrouching = true;
                      bIsMoving    = false;
                      break;
                  case SDLK_SPACE:
                      JumpEntity(3.5f, pstEntity[0]);
                      break;
                }
            }
            else if (SDL_KEYUP == stEvent.type)
            {
                switch(stEvent.key.keysym.sym)
                {
                  case SDLK_LEFT:
                      bIsMoving = false;
                      break;
                  case SDLK_RIGHT:
                      bIsMoving = false;
                      break;
                  case SDLK_DOWN:
                      bIsCrouching = false;
                      bIsMoving    = false;
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
                    bIsRunning = false;
                }
            }
            else if (SDL_FINGERDOWN == stEvent.type)
            {
                s32TouchPosX = (int32_t)round(stEvent.tfinger.x * s32WindowW);
                s32TouchPosY = (int32_t)round(stEvent.tfinger.y * s32WindowH);

                if (s32TouchPosX < (s32WindowW / 2))
                {
                    bOrientation = LEFT;
                    bIsMoving    = true;
                }
                else
                {
                    bOrientation = RIGHT;
                    bIsMoving    = true;
                }

                if (s32TouchPosY > (s32WindowH - 32))
                {
                    bIsCrouching = true;
                    bIsMoving    = false;
                }
            }
            else if (SDL_FINGERUP == stEvent.type)
            {
                bIsCrouching = false;
                bIsMoving    = false;
            }
            else if (SDL_FINGERMOTION == stEvent.type)
            {
                double dDY = fabs(stEvent.tfinger.dy);
                if (0.03 < dDY)
                {
                    JumpEntity(4.5f, pstEntity[0]);
                }
            }
            else if (SDL_MULTIGESTURE == stEvent.type)
            {
                bIsMoving  = false;
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
            AnimateEntity(true, pstEntity[0]);
            SetFrameOffset(0, 1, pstEntity[0]);
            SetAnimation(0, 3, 5.f, pstEntity[0]);
        }

        // Move player entity.
        if (bIsMoving)
        {
            AnimateEntity(true, pstEntity[0]);
            MoveEntityFull(
                bOrientation, 6.0, 3.0, 0, 15,
                24.f,
                0,
                pstEntity[0]);
        }

        if (bIsCrouching)
        {
            AnimateEntity(false, pstEntity[0]);
            SetFrameOffset(0, 0, pstEntity[0]);
            SetOrientation(bOrientation, pstEntity[0]);
            SetAnimation(22, 22, 0.f, pstEntity[0]);
        }

        // Update game logic.
        // Set up collision detection.
        if (IsOnTileOfType("Platform", pstEntity[0]->dPosX, pstEntity[0]->dPosY, pstEntity[0]->u16Height, pstMap))
        {
            bIsOnPlatform = true;
        }
        else
        {
            bIsOnPlatform = false;
        }

        if (pstEntity[0]->dPosX < 0.f || pstEntity[0]->dPosX > pstMap->u16Width)
        {
            bIsOnPlatform = true;
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

        UpdateEntity(dDeltaTime, pstMap->dGravitation, pstMap->u8MeterInPixel, pstEntity[0]);
        for (uint8_t u8Index = 1; u8Index <= 3; u8Index++)
        {
            UpdateEntity(dDeltaTime, 0, 24, pstEntity[u8Index]);
        }

        // Follow player entity and set camera boudnaries to map size.
        SetCameraTargetEntity(
            pstVideo->s32LogicalWindowWidth,
            pstVideo->s32LogicalWindowHeight,
            pstCamera,
            pstEntity[0]);

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
        for (uint8_t u8Index = 1; u8Index <= 3; u8Index++)
        {
            ConnectHorizontalMapEndsForEntity(pstMap->u16Width, pstEntity[u8Index]);
        }
    }

    Quit();

    if (-1 == sReturnValue)
    {
        return EXIT_FAILURE;
    }
    else
    {
        return EXIT_SUCCESS;
    }
}

static int Init()
{
    bool bFullscreen  = false;
    int  sReturnValue = 0;

    const char *pacBgFileNames[3] = {
        "res/backgrounds/city-bg0.png",
        "res/backgrounds/city-bg1.png",
        "res/backgrounds/city-bg2.png"
    };

    #ifndef __ANDROID__
    bFullscreen = true;
    #endif

    sReturnValue = InitVideo(
        "Tau Ceti", 640, 360, 384, 216,
        bFullscreen, &pstVideo);
    RETURN_ON_ERROR(sReturnValue);

    sReturnValue = InitCamera(&pstCamera);
    RETURN_ON_ERROR(sReturnValue);

    sReturnValue = InitEntity(112, 192, 64, 64, &pstEntity[0]);
    SetSpawnPosition(112, 192, pstEntity[0]);
    RETURN_ON_ERROR(sReturnValue);

    // Set up background vehicles.
    sReturnValue = InitEntity(-257, 192, 264, 104, &pstEntity[1]); // Truck.
    RETURN_ON_ERROR(sReturnValue);
    SetOrientation(LEFT, pstEntity[1]);
    SetSpeed(5.f, 2.f, pstEntity[1]);
    MoveEntity(pstEntity[1]);

    sReturnValue = InitEntity(1000, 300, 168, 64, &pstEntity[2]);  // Police.
    RETURN_ON_ERROR(sReturnValue);
    SetFrameOffset(0, 2, pstEntity[2]);
    SetSpeed(50.f, 10.f, pstEntity[2]);
    MoveEntity(pstEntity[2]);

    sReturnValue = InitEntity(128, 150, 96, 64, &pstEntity[3]);    // Misc 1.
    RETURN_ON_ERROR(sReturnValue);
    SetFrameOffset(1, 3, pstEntity[3]);
    SetSpeed(50.f, 8.f, pstEntity[2]);
    SetOrientation(LEFT, pstEntity[3]);
    MoveEntity(pstEntity[3]);

    sReturnValue = InitEntity(800, 416, 96, 64, &pstEntity[4]);    // Misc 2.
    RETURN_ON_ERROR(sReturnValue);
    SetFrameOffset(0, 3, pstEntity[4]);
    SetSpeed(50.f, 5.f, pstEntity[2]);
    MoveEntity(pstEntity[4]);

    sReturnValue = InitMap("res/maps/city.tmx", "res/tilesets/city.png", 32, &pstMap);
    SetTileAnimationSpeed(5.f, pstMap);
    RETURN_ON_ERROR(sReturnValue);

    sReturnValue = InitBackground(
        3, pacBgFileNames, pstVideo->s32WindowWidth, BOTTOM,
        pstVideo->pstRenderer, &pstBg);
    RETURN_ON_ERROR(sReturnValue);

    sReturnValue = InitSprite(
        "res/sprites/player.png", 1536, 128, 0, 0, &pstSpPlayer, pstVideo->pstRenderer);
    RETURN_ON_ERROR(sReturnValue);

    sReturnValue = InitSprite(
        "res/sprites/vehicles.png", 264, 256, 0, 0, &pstSpVehicles, pstVideo->pstRenderer);
    RETURN_ON_ERROR(sReturnValue);

    sReturnValue = InitAudio(&pstAudio);
    RETURN_ON_ERROR(sReturnValue);

    sReturnValue = InitMusic(
        "res/music/FutureAmbient_3.ogg", -1,
        &pstMusic);
    RETURN_ON_ERROR(sReturnValue);

    LockCamera(pstCamera);
    PlayMusic(2000, pstMusic);
    SetFrameOffset(0, 0, pstEntity[0]);

    return sReturnValue;
}

static int Render()
{
    int sReturnValue = 0;

    sReturnValue = DrawBackground(
        pstEntity[0]->bOrientation,
        pstVideo->s32LogicalWindowHeight,
        pstCamera->dPosY,
        dBgVelocityX,
        pstVideo->pstRenderer,
        pstBg);

    for (uint8_t u8Index = 1; u8Index <= 3; u8Index++)
    {
        sReturnValue = DrawEntity(
            pstEntity[u8Index],
            pstCamera,
            pstSpVehicles,
            pstVideo->pstRenderer);
    }

    sReturnValue = DrawMap(
        0, true, true, "BG",
        pstCamera->dPosX,
        pstCamera->dPosY,
        dDeltaTime,
        pstMap,
        pstVideo->pstRenderer);

    sReturnValue = DrawEntity(
        pstEntity[0],
        pstCamera,
        pstSpPlayer,
        pstVideo->pstRenderer);

    sReturnValue = DrawMap(
        1, false, false, "FG",
        pstCamera->dPosX,
        pstCamera->dPosY,
        dDeltaTime,
        pstMap,
        pstVideo->pstRenderer);

    return sReturnValue;
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
