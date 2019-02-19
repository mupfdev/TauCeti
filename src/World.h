/**
 * @file World.h
 * @ingroup World
 */

#ifndef _WORLD_H_
#define _WORLD_H_

#include "EventHandler.h"
#include "Resources.h"

int UpdateWorld(void *pData); // Main thread.
void DetectCollisions(Events *pstEvents, Res *pstRes);
void UpdateEntities(Events *pstEvents, Res *pstR);

#endif // _WORLD_H_
