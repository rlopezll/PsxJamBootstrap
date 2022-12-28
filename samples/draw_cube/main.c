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

#define CUBESIZE 196 

static SDC_Vertex cube_vertices[] = {
    {-CUBESIZE / 2, -CUBESIZE / 2, -CUBESIZE / 2, 0}, {CUBESIZE / 2, -CUBESIZE / 2, -CUBESIZE / 2, 0},
    {CUBESIZE / 2, CUBESIZE / 2, -CUBESIZE / 2, 0},   {-CUBESIZE / 2, CUBESIZE / 2, -CUBESIZE / 2, 0},
    {-CUBESIZE / 2, -CUBESIZE / 2, CUBESIZE / 2, 0},  {CUBESIZE / 2, -CUBESIZE / 2, CUBESIZE / 2, 0},
    {CUBESIZE / 2, CUBESIZE / 2, CUBESIZE / 2, 0},    {-CUBESIZE / 2, CUBESIZE / 2, CUBESIZE / 2, 0},
};

static u_short cube_indices[] = {
    0, 2, 1, 2, 0, 3,  1, 6, 5, 6, 1, 2,  5, 7, 4, 7, 5, 6,  4, 3, 0, 3, 4, 7,  4, 1, 5, 1, 4, 0,  6, 3, 7, 3, 6, 2,
};

static SDC_Mesh3D cubeMesh = { cube_vertices, cube_indices, NULL, 36, 8, POLIGON_VERTEX };

int main(void) 
{
    dcMemory_Init();

    SDC_Render render;
    SDC_Camera camera;
    long distanceY = -300;
    long distanceZ = 300;
    int  width = 640;
    int  height = 240;

    CVECTOR meshColor = {255, 0, 0};
    CVECTOR bgColor = {60, 120, 120};
    dcRender_Init(&render, width, height, bgColor, 4096, 32, RENDER_MODE_PAL);
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

        FntPrint("GameDev Challenge Cube Demo\n");

        dcRender_DrawMesh(&render, &cubeMesh, &transform, &meshColor, NULL);

        dcRender_SwapBuffers(&render);
    }

    return 0;
}