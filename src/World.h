/**
 * @file World.h
 * @ingroup World
 */

#pragma once
#include "EventHandler.h"
#include "Resources.h"

int UpdateWorld(void *pData); // Main thread.
void DetectCollisions(Events *pstEvents, Res *pstRes);
void UpdateCamera(Res *pstRes);
void UpdateEntities(Events *pstEvents, Res *pstRes);
void UpdateMapObjects(Res *pstRes);
