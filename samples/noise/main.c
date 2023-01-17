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

static SDC_Vertex piramid_vertices[] = {
    { {0, CUBESIZE / 2, 0 } }, 
    { {-CUBESIZE/2, -CUBESIZE / 2, -CUBESIZE / 2 } }, { {CUBESIZE/2, -CUBESIZE / 2, -CUBESIZE / 2 } }, { {0, -CUBESIZE / 2, CUBESIZE / 2 } }
};

static u_short piramid_indices[] = {
    0, 1, 2,
    0, 2, 3,
    0, 3, 1,
    1, 3, 2
};

static SDC_Mesh3D piramidMesh = { piramid_vertices, piramid_indices, NULL, 12, 4, POLIGON_VERTEX };


SDC_Mesh3D* GenerateProceduralTerrain();
long GetHeightAtXZ(long x, long z);


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
    long cameraHeight = 600;
    int  width = 640;
    int  height = 240;

    SDC_Mesh3D* terrainMesh = GenerateProceduralTerrain();

    CVECTOR bgColor = {60, 120, 120};
    dcRender_Init(&render, width, height, bgColor, 4096, 65536, RENDER_MODE_PAL);
    dcCamera_SetScreenResolution(&camera, width, height);
    dcCamera_SetCameraPosition(&camera, 0, cameraHeight, distance);
    dcCamera_LookAt(&camera, &VECTOR_ZERO);

    SVECTOR rotation = {0};
    VECTOR translation = {-512, 600, -512, 0};
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
        //long prevTerrainOffset = GetHeightAtXZ(transform.t[0], transform.t[2]);

        transform.t[0] += translation.vx;
        transform.t[2] += translation.vz;
        
        transform.t[1] += translation.vy; //GetHeightAtXZ(transform.t[0], transform.t[2]);



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

        FntPrint("GameDev Challenge Noise Demo\n");

        // Draw the axis
        dcMisc_DrawAxis(&render, &camera);

        // Draw terrain
        MATRIX terrainMVP = {0};
        SVECTOR rot0 = {0};
        RotMatrix(&rot0, &terrainMVP);
        terrainMVP.t[1] = -512;

        dcCamera_ApplyCameraTransform(&camera, &terrainMVP, &terrainMVP);
        SDC_DrawParams terrainParams = { 0 };

        dcRender_DrawMesh(&render, terrainMesh, &terrainMVP, &terrainParams );
        
        // Draw the cube
        dcRender_DrawMesh(&render, &piramidMesh, &MVP, &drawParams );
        
        // Draw a line along the cube's movement direction
        SVECTOR cubeCenter = {0};
        SVECTOR frontPoint = {0, 0, CUBESIZE << 2};
        dcRender_DrawLine(&render, &cubeCenter, &frontPoint, &MVP, &drawParams.constantColor, 4 );

        dcRender_SwapBuffers(&render);
    }

    return 0;
}

const long maxHeight = ONE;
const long minHeight = -ONE >> 2;
const long size = 2 << 12;

long GetHeightAtXZ(long x, long z)
{
    const long dist = SquareRoot12(DC_MUL64(x, x) + DC_MUL64(z, z));
    const long distFactor = DC_DIV(dist, size);

    const long amplitude = maxHeight - minHeight;

    const long tmpMinHeight = DC_LERP( maxHeight - (amplitude >> 2), minHeight, DC_CLAMP(distFactor, 0, ONE));
    const long tmpMaxHeight = DC_LERP( maxHeight, minHeight + ( amplitude >> 2), DC_CLAMP(distFactor, 0, ONE));

    const long tmpAmplitude = tmpMaxHeight - tmpMinHeight;
    return tmpMinHeight + DC_MUL64( DC_ONE + dcMisc_Fbm(x >> 2, (x^z) >> 2, z >> 2), tmpAmplitude >> 1 );
}

SDC_Mesh3D* GenerateProceduralTerrain()
{

    // colors in abgr format
    const u_long seaBottomColor = 0x00000000;
    const u_long seaColor = 0x00AA0000;
    const u_long beachColor = 0x006BD1F2;
    const u_long forestColor = 0x00228B22;
    const u_long snowColor = 0x00FAFBFF;

    CVECTOR* snowRgb = (CVECTOR*)&snowColor;
    printf("Snow r: %x g: %x b: %x", snowRgb->r, snowRgb->g, snowRgb->b );

    const u_long columns = 20;
    const u_long rows = 20;

    SDC_Mesh3D* mesh = dcMisc_GeneratePlaneMeshXZ( size, columns, rows, POLIGON_VERTEX_COLOR );
    SDC_VertexColor* vert = (SDC_VertexColor*) mesh->vertexs;

    CVECTOR vertexColor;
    long height;

    for(int i = 0; i < mesh->numVertices; ++i)
    {
        height = GetHeightAtXZ(vert[i].position.vx, vert[i].position.vz );
        
        vert[i].position.vy = height >> 2;

        DC_LERP_COLOR((CVECTOR*)&seaBottomColor, (CVECTOR*)&seaColor, DC_CLAMP(DC_DIV(height - minHeight, 0 - minHeight), 0, DC_ONE), &vertexColor );
        DC_LERP_COLOR(&vertexColor, (CVECTOR*)&beachColor, DC_CLAMP(DC_DIV(height - 0, 256 - 0), 0, DC_ONE), &vertexColor );
        DC_LERP_COLOR(&vertexColor, (CVECTOR*)&forestColor, DC_CLAMP(DC_DIV(height - 256, (maxHeight - 1000) - 256), 0, DC_ONE), &vertexColor );
        DC_LERP_COLOR(&vertexColor, (CVECTOR*)&snowColor, DC_CLAMP(DC_DIV(height - (maxHeight - 1000), maxHeight - (maxHeight - 1000)), 0, DC_ONE), &vertexColor );
        
        vert[i].color = vertexColor;

    }
    
    return mesh;
}
