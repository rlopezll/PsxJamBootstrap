// screw you code formatter, this one needs to stay on top
#include <stdlib.h>
// now we need to have libgte.h
#include <libgte.h>
// then, these can work; the psyq library is badly written and won't include what it uses
#include <libetc.h>
#include <libgpu.h>
#include <stdio.h>
#include <types.h>

#include "dcMath.h"
#include "dcCamera.h"
#include "dcRender.h"
#include "dcMemory.h"
#include "dcSprite.h"

extern unsigned long _binary_sonic_tim_start[];

SDC_SpriteFrame sonic_run_frames[] = {
    {29-13,0,28,42}, {58-13,0,37,42}, {96-13,0,38,42}, {135-13,0,26,42},
    {162-13,0,27,42}, {190-13,0,39,42}, {217,0,39,42}
};

SDC_SpriteAnimation sonic_run = {sonic_run_frames, NULL, 3, 7, 0, 0};

int main(void) 
{
    dcMemory_Init();

    SDC_Render render;
    SDC_Camera camera;
    int  width = 320;
    int  height = 240;

    CVECTOR bgColor = {60, 120, 120};
    dcRender_Init(&render, width, height, bgColor, 4096, 32, RENDER_MODE_PAL);

    SDC_Sprite sonic_sprite;
    dcSprite_LoadAnimationTex(&sonic_run, _binary_sonic_tim_start);
    dcSprite_SetAnimation(&sonic_sprite, &sonic_run);
 
    DVECTOR UVs = {58,0};
    CVECTOR Color = {128, 128, 128};
    while (1) {
        FntPrint("GameDev Challenge Draw Sprite\n");

        dcSprite_Update(&sonic_sprite);
        dcSprite_Render(&render, &sonic_sprite, width/2, height - 60, &Color);
        dcRender_SwapBuffers(&render);
    }

    return 0;
}