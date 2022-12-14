#ifndef _SUSPENSION_D_I_MESH_
#define _SUSPENSION_D_I_MESH_

#include <types.h>
#include "dcRender.h"

static SDC_VertexTexturedNormal suspension_d_i_Vertices[] = {
    { 11, 13, 18, 0, -847, -3826, 1190, 0, 17, 61 },
    { 24, 11, 22, 0, -847, -3826, 1190, 0, 14, 60 },
    { 11, 14, 21, 0, -847, -3826, 1190, 0, 18, 60 },
    { 24, 10, 19, 0, -847, -3826, 1190, 0, 13, 60 },
    { 24, 12, 21, 0, 1112, 3768, -1156, 0, 14, 60 },
    { 11, 15, 18, 0, 1112, 3768, -1156, 0, 17, 61 },
    { 11, 16, 21, 0, 1112, 3768, -1156, 0, 18, 60 },
    { 24, 12, 19, 0, 1112, 3768, -1156, 0, 13, 60 },
    { 24, 12, 21, 0, -106, 391, 4075, 0, 14, 60 },
    { 11, 14, 21, 0, -106, 391, 4075, 0, 18, 59 },
    { 24, 11, 22, 0, -106, 391, 4075, 0, 14, 60 },
    { 11, 16, 21, 0, -106, 391, 4075, 0, 18, 58 },
    { 24, 12, 19, 0, 120, -391, -4075, 0, 13, 59 },
    { 11, 13, 18, 0, 120, -391, -4075, 0, 17, 59 },
    { 11, 15, 18, 0, 120, -391, -4075, 0, 17, 58 },
    { 24, 10, 19, 0, 120, -391, -4075, 0, 13, 60 },
    { 24, 11, 22, 0, -908, -3879, -951, 0, 14, 60 },
    { 13, 12, 29, 0, -908, -3879, -951, 0, 17, 61 },
    { 12, 13, 27, 0, -908, -3879, -951, 0, 18, 60 },
    { 25, 10, 24, 0, -908, -3879, -951, 0, 13, 60 },
    { 13, 14, 30, 0, 1158, 3840, 828, 0, 17, 58 },
    { 24, 12, 22, 0, 1158, 3840, 828, 0, 14, 59 },
    { 12, 15, 27, 0, 1158, 3840, 828, 0, 18, 58 },
    { 25, 12, 25, 0, 1158, 3840, 828, 0, 13, 59 },
    { 25, 12, 25, 0, 1520, -375, 3784, 0, 13, 60 },
    { 13, 12, 29, 0, 1520, -375, 3784, 0, 17, 61 },
    { 25, 10, 24, 0, 1520, -375, 3784, 0, 13, 60 },
    { 13, 14, 30, 0, 1520, -375, 3784, 0, 17, 60 },
    { 24, 12, 22, 0, -1508, 375, -3789, 0, 14, 60 },
    { 12, 13, 27, 0, -1508, 375, -3789, 0, 18, 60 },
    { 12, 15, 27, 0, -1508, 375, -3789, 0, 18, 59 },
    { 24, 11, 22, 0, -1508, 375, -3789, 0, 14, 60 }
};
static u_short suspension_d_i_Indices[] = {
    0, 2, 1, 1, 3, 0, 4, 6, 5, 5, 7, 4, 8, 10, 9, 9, 11, 8, 12, 14, 13, 15, 12, 13, 16, 18, 17, 17, 19, 16, 20, 22, 21, 
    21, 23, 20, 24, 26, 25, 25, 27, 24, 28, 30, 29, 31, 28, 29
};
static SDC_Mesh3D suspension_d_i_Mesh = {suspension_d_i_Vertices, suspension_d_i_Indices, NULL, 48, 32, POLIGON_VERTEX_TEXTURED_NORMAL};
#endif