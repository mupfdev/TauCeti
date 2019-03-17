/**
 * @file World.c
 * @ingroup World
 * @defgroup World
 * @author Michael Fitzmayer
 * @copyright "THE BEER-WARE LICENCE" (Revision 42)
 */

#include <SDL.h>
#include <eszFW.h>
#include "EventHandler.h"
#include "Resources.h"
#include "World.h"

int UpdateWorld(void *pData)
{
    Sint8   s8ReturnValue = 0;
    Res    *pstRes        = (Res *)pData;
    Events *pstEvents;

    s8ReturnValue = InitEvents(&pstEvents);
    if (-1 == s8ReturnValue)
    {
        return s8ReturnValue;
    }

    while (pstRes->bThreadIsRunning)
    {
        #ifdef __ANDROID__
        pstEvents->s32WindowW = pstRes->pstVideo->s32LogicalWindowWidth;
        pstEvents->s32WindowH = pstRes->pstVideo->s32LogicalWindowHeight;
        #endif

        // Check if fullscreen state has been toggled.
        if (pstEvents->bToggleFullscreen)
        {
            ToggleFullscreen(pstRes->pstVideo);
            pstEvents->bToggleFullscreen = 0;
        }

        // Lock / Unlock camera.
        if (pstEvents->bIsCameraUnlocked)
        {
            UnlockCamera(pstRes->pstCamera);
        }
        else
        {
            LockCamera(pstRes->pstCamera);
        }

        UpdateEntities(pstEvents, pstRes);
        UpdateCamera(pstRes);

        SDL_Delay(APPROX_TIME_PER_FRAME);
    }

    FreeEvents(pstEvents);
    return 0;
}

void DetectCollisions(Events *pstEvents, Res *pstRes)
{
    if (IsOnTileOfType(
            "Platform", pstRes->pstEntity[0]->dPosX, pstRes->pstEntity[0]->dPosY,
            pstRes->pstEntity[0]->u16Height, pstRes->pstMap))
    {
        pstEvents->bIsOnPlatform = 1;
    }
    else
    {
        pstEvents->bIsOnPlatform = 0;
    }

    if (! pstEvents->bIsOnPlatform)
    {
        SetFrameOffset(0, 1, pstRes->pstEntity[0]);
        if (IsEntityRising(pstRes->pstEntity[0]))
        {
            SetAnimation(12, 15, 12.5f, pstRes->pstEntity[0]);
        }
        else
        {
            SetAnimation(15, 15, 12.5f, pstRes->pstEntity[0]);
        }
        DropEntity(pstRes->pstEntity[0]);
    }
}

void UpdateCamera(Res *pstRes)
{
    // Follow player entity and set camera boudnaries to map size.
    SetCameraTargetEntity(
        pstRes->pstVideo->s32LogicalWindowWidth,
        pstRes->pstVideo->s32LogicalWindowHeight,
        pstRes->pstEntity[0],
        pstRes->pstCamera);

    if (SetCameraBoundariesToMapSize(
            pstRes->pstVideo->s32LogicalWindowWidth,
            pstRes->pstVideo->s32LogicalWindowHeight,
            pstRes->pstMap->u16Width,
            pstRes->pstMap->u16Height,
            pstRes->pstCamera))
    {
        // Do not move background when camera hits boundaries.
        pstRes->dBgVelocityX = 0;
    }
    else
    {
        pstRes->dBgVelocityX = pstRes->pstEntity[0]->dVelocityX;
    }

    // Do not move background when camera is locked.
    if (! IsCameraLocked(pstRes->pstCamera))
    {
        pstRes->dBgVelocityX = 0;
    }
}

void UpdateEntities(Events *pstEvents, Res *pstRes)
{
    // Reset entity flags.
    ResetEntity(pstRes->pstEntity[0]);
    if (! UpdateEvents(pstEvents))
    {
        pstRes->bGameIsRunning   = 0;
        pstRes->bThreadIsRunning = 0;
    }

    // Set the player's idle animation.
    if (! IsEntityMoving(pstRes->pstEntity[0]))
    {
        AnimateEntity(1, pstRes->pstEntity[0]);
        SetFrameOffset(0, 1, pstRes->pstEntity[0]);
        SetAnimation(0, 3, 5.f, pstRes->pstEntity[0]);
    }

    // Process events.
    if (pstEvents->bIsCrouching)
    {
        AnimateEntity(0, pstRes->pstEntity[0]);
        SetFrameOffset(0, 0, pstRes->pstEntity[0]);
        SetDirection(pstEvents->eDirection, pstRes->pstEntity[0]);
        SetAnimation(22, 22, 0.f, pstRes->pstEntity[0]);
    }

    if (pstEvents->bIsMoving && ! pstEvents->bIsCrouching)
    {
        AnimateEntity(1, pstRes->pstEntity[0]);
        MoveEntityFull(
            pstEvents->eDirection, 5.0f, 2.5f, 0, 15,
            24.f,
            0,
            pstRes->pstEntity[0]);
    }

    if (pstEvents->bIsJumping)
    {
        if (! pstEvents->bIsCrouching)
        {
            JumpEntity(4.f, pstRes->pstEntity[0]);
            pstEvents->bIsJumping = 0;
        }
        else
        {
            pstRes->pstEntity[0]->dPosY += 8.f;
            pstEvents->bIsJumping = 0;
        }
    }

    // Collision detecion.
    DetectCollisions(pstEvents, pstRes);

    // Update map objects.
    UpdateMapObjects(pstRes);

    // Update entity state.
    UpdateEntity(
        pstRes->pstVideo->dDeltaTime,
        pstRes->pstMap->dGravitation,
        pstRes->pstMap->u8MeterInPixel,
        pstRes->pstEntity[0]);

    for (Uint8 u8Index = 1; u8Index <= 3; u8Index++)
    {
        UpdateEntity(
            pstRes->pstVideo->dDeltaTime,
            0,
            pstRes->pstMap->u8MeterInPixel,
            pstRes->pstEntity[u8Index]);
    }

    if (pstRes->pstEntity[0]->dPosY > (pstRes->pstMap->u16Height + pstRes->pstEntity[0]->u16Height))
    {
        ResetEntityToSpawnPosition(pstRes->pstEntity[0]);
    }

    for (Uint8 u8Index = 0; u8Index <= 3; u8Index++)
    {
        ConnectHorizontalMapEndsForEntity(pstRes->pstMap->u16Width, pstRes->pstEntity[u8Index]);
    }
}

void UpdateMapObjects(Res *pstRes)
{
    for (Uint16 u16Index = 0; u16Index < pstRes->pstMap->u16ObjectCount; u16Index++)
    {
        Object *pstObject = &pstRes->pstMap->astObject[u16Index];
        AABB    stBB      = pstObject->stBB;

        #ifdef __ANDROID__
        if (IsObjectOfType("AndroidMsg", pstObject))
        {
            if (BoxesDoIntersect(pstRes->pstEntity[0]->stBB, stBB))
            {
                SDL_strlcpy(pstRes->acMessage, GetObjectName(pstObject), OBJECT_NAME_LEN - 1);
                pstRes->bShowMessage = 1;
                break;
            }
            else
            {
                pstRes->bShowMessage = 0;
            }
        }
        #endif
        if (IsObjectOfType("GenericMsg", pstObject))
        {
            if (BoxesDoIntersect(pstRes->pstEntity[0]->stBB, stBB))
            {
                SDL_strlcpy(pstRes->acMessage, GetObjectName(pstObject), OBJECT_NAME_LEN - 1);
                pstRes->bShowMessage = 1;
                break;
            }
            else
            {
                pstRes->bShowMessage = 0;
            }
        }
    }
}
