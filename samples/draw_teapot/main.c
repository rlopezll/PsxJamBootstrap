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

    SVECTOR rotation = {0};
    VECTOR translation = {0, 0, 0, 0};
    MATRIX transform;

    while (1) {
        rotation.vy += 16;

        RotMatrix(&rotation, &transform);
        TransMatrix(&transform, &translation);
        dcCamera_ApplyCameraTransform(&camera, &transform, &transform);

        FntPrint("GameDev Challenge Teapot Demo\n");

        dcRender_DrawMesh(&render, &Teapot001_Mesh, &transform, NULL, NULL);  

        dcRender_SwapBuffers(&render);  
    }

    return 0;
}