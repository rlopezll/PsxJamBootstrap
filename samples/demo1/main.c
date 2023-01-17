#include <stdio.h>
#include <types.h>
#include <libetc.h>

#include "dcMath.h"
#include "dcMemory.h"
#include "dcCamera.h"
#include "dcRender.h"
#include "dcAudio.h"

#include "meshes/carroceria.h"
#include "meshes/rueda_t_d001.h"
#include "meshes/rueda_t_d002.h"
#include "meshes/rueda_t_d003.h"
#include "meshes/rueda_t_d004.h"

#include "meshes/suspension_d_d.h"
#include "meshes/suspension_d_i.h"
#include "meshes/suspension_t_d.h"
#include "meshes/suspension_t_i.h"

extern unsigned long _binary_data_accept_vag_start[];
extern unsigned long _binary_data_beep_vag_start[];
extern unsigned long _binary_data_bell_vag_start[];

extern unsigned long _binary_meshes_FType_PSX_tim_start[];

int main(void) 
{
    dcMemory_Init();

    SDC_Render render;
    SDC_Camera camera;
    SDC_Audio audio;
    SDC_Sfx acceptSfx;
    SDC_Sfx beepSfx;
    SDC_Sfx bellSfx;
    int  width = 640;
    int  height = 240;
    long distanceY = 50;
    long distanceZ = 100;

    // long distanceY = 00;
    // long distanceZ = 600;

    CVECTOR bgColor = {60, 120, 120}; 
    dcRender_Init(&render, width, height, bgColor, 4096, 16192, RENDER_MODE_PAL);
    dcCamera_SetScreenResolution(&camera, width, height);
    dcCamera_SetCameraPosition(&camera, 0, distanceY, distanceZ);
    dcCamera_LookAt(&camera, &VECTOR_ZERO);
    
    SDC_TIM_IMAGE diffuse;
    dcRender_LoadTexture(&diffuse, _binary_meshes_FType_PSX_tim_start);  

    SVECTOR rotation = {0};
    VECTOR translationTeapot = {0, 0, 0, 0}; 
    MATRIX transform;

    SDC_DrawParams draw_params_teapot = {
        .tim = &diffuse,
        .constantColor = {255, 255, 255},
        .bLighting = 1,
        .bUseConstantColor = 0
    };

    CVECTOR ambientColor = {0, 0, 0};
    dcRender_SetAmbientColor(&render, &ambientColor );

    SVECTOR lightDir = {DC_ONE, 0, 0 };
    SVECTOR lightColor = {DC_ONE, DC_ONE, DC_ONE};
    dcRender_SetLight(&render, 0, &lightDir, &lightColor);

    SVECTOR lightDir1 = {0, -DC_ONE, 0 };
    SVECTOR lightColor1 = {DC_ONE/2, DC_ONE/2, DC_ONE/2};
    dcRender_SetLight(&render, 1, &lightDir1, &lightColor1);

    dcAudio_Init(&audio, 16);
    dcAudio_MusicPlay(&audio, 0);
    dcAudio_SfxLoad(&audio, &bellSfx, (u_char *)_binary_data_bell_vag_start);
    dcAudio_SfxLoad(&audio, &acceptSfx, (u_char *)_binary_data_accept_vag_start);
    dcAudio_SfxLoad(&audio, &beepSfx, (u_char *)_binary_data_beep_vag_start);

    // Init Pad
    PadInit(0);
    // Store input values
    int pad = 0, oldpad = 0;

    while (1) {

        // Read pad values
        pad = PadRead(0);
        if (pad & PADLleft && !(oldpad & PADLleft)){
            printf("Beep Sfx Play!\n");
            dcAudio_SfxPlay(&beepSfx);
            oldpad = pad;
        }
        if (pad & PADLright && !(oldpad & PADLright)){
            printf("Bell Sfx Play!\n");
            dcAudio_SfxPlay(&bellSfx);
            oldpad = pad;
        }
        if (pad & PADLup && !(oldpad & PADLup)){
            printf("Accept Sfx Play!\n");
            dcAudio_SfxPlay(&acceptSfx);
            oldpad = pad;
        }

        // Reset oldpad    
        if (!(pad & PADRdown) ||
            !(pad & PADRup)   ||
            !(pad & PADRleft) ||
            !(pad & PADRright) ||
            !(pad & PADLdown) ||
            !(pad & PADLup)   ||
            !(pad & PADLleft) ||
            !(pad & PADLright)
            ){ oldpad = pad; }

        FntPrint("GameDev Challenge Audio\n");

        rotation.vy += 16; 

        RotMatrix(&rotation, &transform);
        TransMatrix(&transform, &translationTeapot);
        dcCamera_ApplyCameraTransform(&camera, &transform, &transform);
        dcRender_DrawMesh(&render, &carroceria_Mesh, &transform, &draw_params_teapot);  

        dcRender_DrawMesh(&render, &rueda_t_d001_Mesh, &transform, &draw_params_teapot);  
        dcRender_DrawMesh(&render, &rueda_t_d002_Mesh, &transform, &draw_params_teapot);  
        dcRender_DrawMesh(&render, &rueda_t_d003_Mesh, &transform, &draw_params_teapot);  
        dcRender_DrawMesh(&render, &rueda_t_d004_Mesh, &transform, &draw_params_teapot);  

        // dcRender_DrawMesh(&render, &suspension_d_d_Mesh, &transform, &draw_params_teapot);  
        // dcRender_DrawMesh(&render, &suspension_d_i_Mesh, &transform, &draw_params_teapot);  
        // dcRender_DrawMesh(&render, &suspension_t_d_Mesh, &transform, &draw_params_teapot);  
        // dcRender_DrawMesh(&render, &suspension_t_i_Mesh, &transform, &draw_params_teapot);  


        dcAudio_Update(&audio);
        dcRender_SwapBuffers(&render);
    }

    return 0;
}