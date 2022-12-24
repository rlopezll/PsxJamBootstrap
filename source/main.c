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
#include "dcMisc.h"
#include "dcCollision.h"

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

static SDC_Mesh3D cubeMesh = { cube_vertices, cube_indices, NULL, 36, POLIGON_VERTEX };

int main(void) 
{
    dcMemory_Init();
    PadInit(0);

    SDC_Mesh3D* sphereMesh = dcMisc_generateSphereMesh(CUBESIZE, 7, 7);

    SDC_Render render;
    SDC_Camera camera;
    long distance = 800;
    int  width = 640;
    int  height = 240;

    CVECTOR meshColor = {255, 0, 0};
    CVECTOR bgColor = {60, 120, 120};
    dcRender_Init(&render, width, height, bgColor, 4096, 8192, RENDER_MODE_PAL);
    dcCamera_SetScreenResolution(&camera, width, height);
    dcCamera_SetCameraPosition(&camera, 0, 0, distance);
    dcCamera_LookAt(&camera, &VECTOR_ZERO);

    SVECTOR rotation = {0};
    VECTOR translation = {0, 0, 0, 0};
    MATRIX transform;

    while (1) {

        // Read pad state and move primitive
        u_long padState = PadRead(0);

        if( _PAD(0,PADLup ) & padState )
        {
            translation.vy -= 32;
        }
        if( _PAD(0,PADLdown ) & padState )
        {
            translation.vy += 32;
        }
        if( _PAD(0,PADLright ) & padState )
        {
            translation.vx -= 32;
        }
        if( _PAD(0,PADLleft ) & padState )
        {
            translation.vx += 32;
        }

        rotation.vy += 16;

        RotMatrix(&rotation, &transform);
        TransMatrix(&transform, &translation);
        dcCamera_ApplyCameraTransform(&camera, &transform, &transform);

        FntPrint("GameDev Challenge Sphere Demo\n");

        SVECTOR rayDir = { camera.viewMatrix.m[2][0], camera.viewMatrix.m[2][1], camera.viewMatrix.m[2][2] };
        VectorNormalSS(&rayDir, &rayDir);
        if( dcCollision_RaySphereInteresct(&camera.position, &rayDir, &translation, CUBESIZE ) > 0 )
        {
            dcRender_DrawMesh(&render, sphereMesh, &transform, NULL);
        }
        else
        {
            dcRender_DrawMesh(&render, &cubeMesh, &transform, &meshColor );
        }

        dcRender_SwapBuffers(&render);
    }

    return 0;
}