// screw you code formatter, this one needs to stay on top
#include <stdlib.h>
// now we need to have libgte.h
#include <libgte.h>
// then, these can work; the psyq library is badly written and won't include what it uses
#include <libetc.h>
#include <libgpu.h>
#include <stdio.h>
#include <types.h>

#include "dcMemory.h"
#include "dcMath.h"
#include "dcCamera.h"
#include "dcRender.h"
#include "dcMisc.h"

#define CUBESIZE 196 

static SDC_Vertex cube_vertices[] = {
    { {-CUBESIZE / 2, -CUBESIZE / 2, -CUBESIZE / 2, 0} }, { {CUBESIZE / 2, -CUBESIZE / 2, -CUBESIZE / 2, 0} },
    { {CUBESIZE / 2, CUBESIZE / 2, -CUBESIZE / 2, 0  } }, { {-CUBESIZE / 2, CUBESIZE / 2, -CUBESIZE / 2, 0  } },
    { {-CUBESIZE / 2, -CUBESIZE / 2, CUBESIZE / 2, 0 } }, { {CUBESIZE / 2, -CUBESIZE / 2, CUBESIZE / 2, 0   } },
    { {CUBESIZE / 2, CUBESIZE / 2, CUBESIZE / 2, 0   } }, { {-CUBESIZE / 2, CUBESIZE / 2, CUBESIZE / 2, 0   } },
};

static u_short cube_indices[] = {
    0, 2, 1, 2, 0, 3,  1, 6, 5, 6, 1, 2,  5, 7, 4, 7, 5, 6,  4, 3, 0, 3, 4, 7,  4, 1, 5, 1, 4, 0,  6, 3, 7, 3, 6, 2,
};

static SDC_Mesh3D cubeMesh = { cube_vertices, cube_indices, NULL, 36, 8, POLIGON_VERTEX };

/**
 * README: MATH TUTORIAL
 * 
 * In this sample we will look at fixed point match by implementing a basic controller which moves front/back with the Up/Down keys 
 * and rotates Right/Left with the respective keys, we well then position the camera to back and up of the object to implement a third person camera.
*/

int main(void) 
{
    dcMemory_Init();
    PadInit(0);

    /**
     * WARNING: Remember to always call InitGeom before any calls to gte functions or it will not work.
    */
    InitGeom();

    SDC_Render render;
    SDC_Camera camera;
    long distance = 800;
    long cameraHeight = 400;
    int  width = 640;
    int  height = 240;

    CVECTOR bgColor = {60, 120, 120};
    dcRender_Init(&render, width, height, bgColor, 4096, 8192, RENDER_MODE_PAL);
    dcCamera_SetScreenResolution(&camera, width, height);
    dcCamera_SetCameraPosition(&camera, 0, cameraHeight, distance);
    dcCamera_LookAt(&camera, &VECTOR_ZERO);

    SVECTOR rotation = {0};
    VECTOR translation = {0, 0, 0, 0};
    MATRIX transform;

    RotMatrix(&rotation, &transform);
    TransMatrix(&transform, &translation);

    SDC_DrawParams drawParams = {
        .tim = NULL,
        .constantColor = {255, 255, 255},
        .bLighting = 0,
        .bUseConstantColor = 1
    };

    while (1) {

        // Read pad state and move primitive
        u_long padState = PadRead(0);

        short rotationAroundY = 0;
        long movementFront = 0;

        // input up/down will determine movement along the front vector of the cube
        if( _PAD(0,PADLup ) & padState )
        {
            movementFront = 32;
        }
        if( _PAD(0,PADLdown ) & padState )
        {
            movementFront = -32;
        }

        // Input right/left will determine rotation aroung Y axis.
        if( _PAD(0,PADLright ) & padState )
        {
            rotationAroundY = -32;
        }
        if( _PAD(0,PADLleft ) & padState )
        {
            rotationAroundY = 32;
        }

        /**
         * We can user gtelib function RotMatrix to create a rotation matrix from a rotation vector.
         * Rotation values are in "revolutions" where 1 rev = 4096 = 360 degrees.
         * So in this sample we are rotating 360*32/4096 = 2.8 degrees each frame
        */
        
        rotation.vx = 0;
        rotation.vy = rotationAroundY;
        rotation.vz = 0;

        MATRIX frameRotation = {0};
        RotMatrix(&rotation, &frameRotation);

        // now we need to accumulate the current rotation with absolute transform matrix
        CompMatrixLV( &transform, &frameRotation, &transform);

        /**
         * We could have computed the translation with the previous matrix operations but we'll do it now using
         * vector math to see how to operate with fixed point math. In this case we want to move 32 along the Z axis (front)
         * of the object, 32 in fixed point math is representing a decimal number wich is equal to 32/4096 (0.0078).
         * 
         * So, we need to take the front vector from the transform matrix and scale it by 32. Then we'll apply this result back to the transform.
        */ 
        
        SVECTOR frontVector = {transform.m[0][2], transform.m[1][2], transform.m[2][2] }; // transform.m[2] is Z vector
        VectorNormalSS(&frontVector, &frontVector); // normalize a vector of shorts to a vector of shorts
        
        /**
         * Now front vector is normalized, where 4096 is 1. So we need to multiply each component by our scaling factor.
         * When multiplying fixed point decimal numbers by each other we need to shift right bits to get the correct result.
         * 
         * For instance, in this sample we want the vector to be of length 32, if we just multiply the normalized vector we would get much larger results:
         * Normalized Z vector would be (0, 0, 4096), because 4096 is our 1 in fixed point decimal numbers, so:
         * 
         * 32 * (0, 0, 4096) = (0, 0, 131072 ) which is not what we want when multiplying something by ONE(4096), but if we divide this number
         * by 4096 we get out 32 lenght. Obviously we don't want to divide if we can just do a shift right by 12 (2^12 = 4096) which is much faster. So:
        */
        translation.vx = (frontVector.vx * movementFront) >> 12;
        translation.vy = (frontVector.vy * movementFront) >> 12;
        // Or using dcMath macros:
        translation.vz = DC_MUL(frontVector.vz, movementFront);
        
        // Accumulate this translation with our transform
        transform.t[0] += translation.vx;
        transform.t[1] += translation.vy;
        transform.t[2] += translation.vz;

        /**
         * To have a third-person camera, we can compute the new camera position by applying an offset in -Forward and Y axis.
         * So we can take the negative frontVector and multiply it by distance then add this offset to the object position.
         * 
         * Note that if the result of the multiplication is big enough it could overflow the 32bit word and we would end up losing the most
         * significant bits of the result. To avoid that we added the function DC_MUL64 in dcMath, which takes advantage of a MIPS r3000
         * feature where the result of a multiply is split between 2 32 bit register mfhi and mflo. Then we combine those two registers to get
         * a more precise 32 bit result.
        */ 
        long cameraX = transform.t[0] - DC_MUL64(frontVector.vx, distance);
        long cameraY = transform.t[1] - DC_MUL64(frontVector.vy, distance) + cameraHeight; // Add a height offset to the Y component
        long cameraZ = transform.t[2] - DC_MUL64(frontVector.vz, distance);

        dcCamera_SetCameraPosition(&camera, cameraX, cameraY, cameraZ );

        // After setting the camera back and up we need to recalculate it's viewProjection by a LookAt at current object position
        VECTOR objPos = {transform.t[0], transform.t[1], transform.t[2] };
        dcCamera_LookAt( &camera, &objPos );

        // Compute our ModelViewProjection matrix by applying camera ViewProjection
        MATRIX MVP;
        dcCamera_ApplyCameraTransform(&camera, &transform, &MVP);

        FntPrint("GameDev Challenge Math Demo\n");

        dcMisc_DrawAxis(&render, &camera);
        dcRender_DrawMesh(&render, &cubeMesh, &MVP, &drawParams );

        dcRender_SwapBuffers(&render);
    }

    return 0;
}
