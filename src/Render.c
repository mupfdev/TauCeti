/**
 * @file      Render.c
 * @author    Michael Fitzmayer
 * @copyright "THE BEER-WARE LICENCE" (Revision 42)
 */

#include <SDL.h>
#include <eszFW.h>
#include "Render.h"
#include "Resources.h"

Sint8 Render(Res* pstRes)
{
    Sint8 s8ReturnValue = 0;

    s8ReturnValue = DrawBackground(
        pstRes->pstEntity[0]->eDirection,
        pstRes->pstVideo->s32LogicalWindowHeight,
        pstRes->pstCamera->dPosY,
        pstRes->dBgVelocityX,
        pstRes->pstVideo->pstRenderer,
        pstRes->pstBg);
    if (s8ReturnValue < 0)
    {
        return s8ReturnValue;
    }

    for (Uint8 u8Index = 1; u8Index <= 3; u8Index++)
    {
        s8ReturnValue = DrawEntity(
            pstRes->pstEntity[u8Index],
            pstRes->pstCamera,
            pstRes->pstSpVehicles,
            pstRes->pstVideo->pstRenderer);
    }

    s8ReturnValue = DrawMap(
        0,
        1,
        1,
        "BG",
        pstRes->pstCamera->dPosX,
        pstRes->pstCamera->dPosY,
        pstRes->pstMap,
        pstRes->pstVideo->pstRenderer);
    if (s8ReturnValue < 0)
    {
        return s8ReturnValue;
    }

    s8ReturnValue = DrawEntity(
        pstRes->pstEntity[0],
        pstRes->pstCamera,
        pstRes->pstSpPlayer,
        pstRes->pstVideo->pstRenderer);
    if (s8ReturnValue < 0)
    {
        return s8ReturnValue;
    }

    s8ReturnValue = DrawMap(
        1,
        0,
        0,
        "FG",
        pstRes->pstCamera->dPosX,
        pstRes->pstCamera->dPosY,
        pstRes->pstMap,
        pstRes->pstVideo->pstRenderer);
    if (s8ReturnValue < 0)
    {
        return s8ReturnValue;
    }

    if (pstRes->bShowMessage)
    {
        Sint32 s32PosX = (Sint32)(pstRes->pstEntity[0]->dPosX - pstRes->pstCamera->dPosX);
        Sint32 s32PosY = (Sint32)(pstRes->pstEntity[0]->dPosY - pstRes->pstCamera->dPosY);
        s32PosY += pstRes->pstEntity[0]->u16Height / 4;

        PrintText(
            pstRes->acMessage, s32PosX, s32PosY, pstRes->pstFont, pstRes->pstVideo->pstRenderer);
    }

    RenderScene(pstRes->pstVideo);
    return s8ReturnValue;
}

void UpdateZoomLevel(Res* pstRes)
{
    double dTime = (double)APPROX_TIME_PER_FRAME / (double)TIME_FACTOR;

    // Set zoom level dynamically in relation to vertical velocity.
    if (0.0 < pstRes->pstEntity[0]->dVelocityY)
    {
        pstRes->pstVideo->dZoomLevel -= dTime / 5.f;
        if (1.0 > pstRes->pstVideo->dZoomLevel)
        {
            pstRes->pstVideo->dZoomLevel = 1;
        }
    }
    else
    {
        pstRes->pstVideo->dZoomLevel += dTime / 1.75f;
        if (pstRes->pstVideo->dZoomLevel > pstRes->pstVideo->dInitialZoomLevel)
        {
            pstRes->pstVideo->dZoomLevel = pstRes->pstVideo->dInitialZoomLevel;
        }
    }
    SetZoomLevel(pstRes->pstVideo->dZoomLevel, pstRes->pstVideo);
}
