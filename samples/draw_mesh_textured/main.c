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

#include "Teapot001.h"
#include "Box001.h"

extern unsigned long _binary_smile_tim_start[];

int main(void) 
{
    dcMemory_Init();

    SDC_Render render;
    SDC_Camera camera;
    long distanceY = -300;
    long distanceZ = 300;
    int  width = 640;
    int  height = 240;

    CVECTOR bgColor = {60, 120, 120}; 
    dcRender_Init(&render, width, height, bgColor, 4096, 8192, RENDER_MODE_PAL);
    dcCamera_SetScreenResolution(&camera, width, height);
    dcCamera_SetCameraPosition(&camera, 0, distanceY, distanceZ);
    dcCamera_LookAt(&camera, &VECTOR_ZERO);

    SDC_TIM_IMAGE tim_smile;
    dcRender_LoadTexture(&tim_smile, _binary_smile_tim_start);  

    SVECTOR rotation = {0};
    VECTOR translationTeapot = {150, 0, 0, 0}; 
    VECTOR translationCube = {-150, 0, 0, 0}; 
    MATRIX transform;

    SDC_DrawParams draw_params;
    draw_params.tim = &tim_smile;
    while (1) {
        rotation.vy += 16; 

        RotMatrix(&rotation, &transform);
        TransMatrix(&transform, &translationCube);
        dcCamera_ApplyCameraTransform(&camera, &transform, &transform);
        dcRender_DrawMesh(&render, &Teapot001_Mesh, &transform, &draw_params);  

        RotMatrix(&rotation, &transform);
        TransMatrix(&transform, &translationTeapot);
        dcCamera_ApplyCameraTransform(&camera, &transform, &transform);
        dcRender_DrawMesh(&render, &Box001_Mesh, &transform, &draw_params);  

        FntPrint("GameDev Challenge Texture Demo\n");
        dcRender_SwapBuffers(&render);  
    }

    return 0;
}