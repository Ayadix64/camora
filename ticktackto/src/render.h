#ifndef TICK_RENDRER
#define TICK_RENDRER
#include "utils.h"
#include "../include/tick-tack-to/basics.h"

void  Genrate2DShapeAttribute(u32 vao, u32 vb, u32 ib);
void  GenrateTextureAttribute(u32 vao, u32 vb, u32 ib);
void  InitlizeRendrer(TickRendrerStruct* rendrer);
void  ResetRendrer(TickRendrerStruct* rendrer);
void  DeletRendrer(TickRendrerStruct* rendrer);
void  BatcheRendrerAdd2DShape(float* vetex , u32 vcount , u32* index, u32 icount ,u32 steps, TickRendrerStruct* render);
bool  regenRendrerData(TickRendrerStruct* rendr);
void  Render(TickRendrerStruct* rendrer);
void  RenderExtended(TickRendrerStruct* rendrer);
void  RenderTexture(TickTextureRendrerStruct*texture);

#endif
