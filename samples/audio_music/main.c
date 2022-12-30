#include <stdio.h>
#include <types.h>
#include <libetc.h>

#include "dcMath.h"
#include "dcMemory.h"
#include "dcRender.h"
#include "dcAudio.h"

extern unsigned long _binary_data_accept_vag_start[];
extern unsigned long _binary_data_beep_vag_start[];
extern unsigned long _binary_data_bell_vag_start[];

int main(void) 
{
    dcMemory_Init();

    SDC_Render render;
    SDC_Audio audio;
    SDC_Sfx acceptSfx;
    SDC_Sfx beepSfx;
    SDC_Sfx bellSfx;
    int  width = 640;
    int  height = 240;

    CVECTOR bgColor = {60, 120, 120};
    dcRender_Init(&render, width, height, bgColor, 4096, 32, RENDER_MODE_PAL);
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
        dcAudio_Update(&audio);
        dcRender_SwapBuffers(&render);
    }

    return 0;
}