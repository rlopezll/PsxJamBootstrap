#ifndef _RUEDA_T_D001_MESH_
#define _RUEDA_T_D001_MESH_

#include <types.h>
#include "dcRender.h"

static SDC_VertexTexturedNormal rueda_t_d001_Vertices[] = {
    { 28, 0, -39, 0, 1358, -3856, -239, 0, 2, 53 },
    { 15, 1, -32, 0, -1110, -3560, 1693, 0, 3, 50 },
    { 14, 0, -37, 0, -1667, -3729, 294, 0, 2, 50 },
    { 29, 1, -34, 0, 1622, -3560, 1211, 0, 3, 53 },
    { 30, 3, -30, 0, 1832, -2727, 2446, 0, 3, 53 },
    { 16, 3, -27, 0, -1209, -2637, 2891, 0, 3, 50 },
    { 17, 13, -23, 0, -1019, 0, 3966, 0, 4, 50 },
    { 31, 8, -27, 0, 1984, -1474, 3265, 0, 4, 53 },
    { 31, 13, -26, 0, 2028, 0, 3558, 0, 4, 53 },
    { 16, 8, -25, 0, -748, -1474, 3747, 0, 4, 50 },
    { 16, 23, -27, 0, -1209, 2637, 2891, 0, 5, 50 },
    { 31, 18, -27, 0, 1984, 1474, 3265, 0, 5, 53 },
    { 30, 23, -30, 0, 1832, 2727, 2446, 0, 5, 53 },
    { 16, 18, -25, 0, -748, 1474, 3747, 0, 5, 50 },
    { 15, 25, -32, 0, -1110, 3560, 1693, 0, 6, 50 },
    { 29, 25, -34, 0, 1622, 3560, 1211, 0, 6, 53 },
    { 28, 27, -39, 0, 1358, 3856, -239, 0, 6, 53 },
    { 14, 27, -37, 0, -1667, 3729, 294, 0, 6, 50 },
    { 13, 25, -42, 0, -1622, 3560, -1211, 0, 7, 50 },
    { 28, 25, -44, 0, 1110, 3560, -1693, 0, 7, 53 },
    { 27, 23, -49, 0, 885, 2727, -2925, 0, 8, 53 },
    { 13, 23, -46, 0, -2125, 2637, -2303, 0, 8, 50 },
    { 12, 18, -49, 0, -1984, 1474, -3265, 0, 8, 50 },
    { 26, 18, -52, 0, 748, 1474, -3747, 0, 8, 53 },
    { 26, 13, -53, 0, 688, 0, -4037, 0, 9, 53 },
    { 12, 13, -50, 0, -2315, 0, -3378, 0, 9, 50 },
    { 12, 8, -49, 0, -1984, -1474, -3265, 0, 9, 50 },
    { 26, 8, -52, 0, 748, -1474, -3747, 0, 9, 53 },
    { 27, 3, -49, 0, 885, -2727, -2925, 0, 10, 53 },
    { 13, 3, -46, 0, -2125, -2637, -2303, 0, 10, 50 },
    { 13, 2, -32, 0, -3426, -1787, 1356, 0, 34, 47 },
    { 12, 2, -41, 0, -3683, -1787, -102, 0, 17, 47 },
    { 14, 0, -37, 0, -1667, -3729, 294, 0, 26, 53 },
    { 29, 1, -34, 0, 1622, -3560, 1211, 0, 16, 50 },
    { 32, 4, -31, 0, 3821, -1315, 662, 0, 9, 43 },
    { 30, 3, -30, 0, 1832, -2727, 2446, 0, 7, 45 },
    { 31, 2, -35, 0, 3744, -1658, 37, 0, 17, 48 },
    { 14, 8, -26, 0, -3244, -740, 2387, 0, 47, 35 },
    { 16, 3, -27, 0, -1209, -2637, 2891, 0, 44, 45 },
    { 31, 13, -26, 0, 2028, 0, 3558, 0, 0, 27 },
    { 32, 8, -29, 0, 3913, -687, 994, 0, 5, 35 },
    { 32, 13, -28, 0, 3916, 0, 1199, 0, 3, 27 },
    { 31, 8, -27, 0, 1984, -1474, 3265, 0, 2, 36 },
    { 14, 18, -26, 0, -3244, 740, 2387, 0, 47, 18 },
    { 17, 13, -23, 0, -1019, 0, 3966, 0, 52, 27 },
    { 32, 22, -31, 0, 3821, 1315, 662, 0, 9, 10 },
    { 31, 18, -27, 0, 1984, 1474, 3265, 0, 2, 17 },
    { 32, 18, -29, 0, 3913, 687, 994, 0, 5, 18 },
    { 30, 23, -30, 0, 1832, 2727, 2446, 0, 7, 8 },
    { 13, 24, -32, 0, -3426, 1787, 1356, 0, 34, 6 },
    { 16, 23, -27, 0, -1209, 2637, 2891, 0, 44, 8 },
    { 28, 27, -39, 0, 1358, 3856, -239, 0, 26, 1 },
    { 29, 25, -34, 0, 1622, 3560, 1211, 0, 16, 3 },
    { 31, 24, -35, 0, 3744, 1658, 37, 0, 17, 6 },
    { 30, 25, -40, 0, 3593, 1861, -633, 0, 26, 4 },
    { 12, 24, -41, 0, -3683, 1787, -102, 0, 17, 6 },
    { 14, 27, -37, 0, -1667, 3729, 294, 0, 26, 1 },
    { 28, 25, -44, 0, 1110, 3560, -1693, 0, 35, 3 },
    { 29, 22, -48, 0, 3364, 1315, -1929, 0, 42, 10 },
    { 27, 23, -49, 0, 885, 2727, -2925, 0, 44, 8 },
    { 29, 24, -44, 0, 3506, 1658, -1315, 0, 34, 6 },
    { 11, 18, -47, 0, -3865, 740, -1133, 0, 5, 18 },
    { 13, 23, -46, 0, -2125, 2637, -2303, 0, 7, 8 },
    { 26, 13, -53, 0, 688, 0, -4037, 0, 52, 27 },
    { 26, 18, -52, 0, 748, 1474, -3747, 0, 49, 17 },
    { 28, 18, -51, 0, 3337, 687, -2272, 0, 47, 18 },
    { 28, 13, -52, 0, 3270, 0, -2466, 0, 49, 27 },
    { 11, 8, -47, 0, -3865, -740, -1133, 0, 5, 35 },
    { 12, 13, -50, 0, -2315, 0, -3378, 0, 0, 27 },
    { 26, 8, -52, 0, 748, -1474, -3747, 0, 49, 36 },
    { 29, 4, -48, 0, 3364, -1315, -1929, 0, 42, 43 },
    { 27, 3, -49, 0, 885, -2727, -2925, 0, 44, 45 },
    { 28, 8, -51, 0, 3337, -687, -2272, 0, 47, 35 },
    { 13, 3, -46, 0, -2125, -2637, -2303, 0, 7, 45 },
    { 28, 0, -39, 0, 1358, -3856, -239, 0, 26, 53 },
    { 28, 1, -44, 0, 1110, -3560, -1693, 0, 35, 50 },
    { 29, 2, -44, 0, 3506, -1658, -1315, 0, 34, 48 },
    { 30, 1, -40, 0, 3593, -1861, -633, 0, 26, 50 },
    { 29, 5, -43, 0, 3922, 1163, -202, 0, 32, 43 },
    { 31, 5, -36, 0, 3754, 1163, -1151, 0, 19, 43 },
    { 29, 10, -48, 0, 4040, 481, 468, 0, 42, 33 },
    { 29, 17, -48, 0, 4040, -481, 468, 0, 42, 20 },
    { 29, 22, -43, 0, 3922, -1163, -202, 0, 32, 10 },
    { 31, 22, -36, 0, 3754, -1163, -1151, 0, 19, 10 },
    { 32, 17, -31, 0, 3636, -481, -1822, 0, 10, 20 },
    { 32, 10, -31, 0, 3636, 481, -1822, 0, 10, 33 },
    { 28, 7, -37, 0, 2658, 2676, -1594, 0, 21, 39 },
    { 27, 7, -42, 0, 3043, 2676, 589, 0, 31, 39 },
    { 27, 10, -46, 0, 3315, 1108, 2133, 0, 38, 32 },
    { 27, 16, -46, 0, 3315, -1108, 2133, 0, 38, 22 },
    { 27, 20, -42, 0, 3043, -2676, 589, 0, 31, 14 },
    { 28, 20, -37, 0, 2658, -2676, -1594, 0, 21, 14 },
    { 29, 16, -33, 0, 2386, -1108, -3139, 0, 13, 22 },
    { 29, 10, -33, 0, 2386, 1108, -3139, 0, 13, 32 },
    { 28, 7, -37, 0, 3922, 666, -971, 0, 21, 39 },
    { 27, 7, -42, 0, 4018, 666, -428, 0, 31, 39 },
    { 27, 13, -39, 0, 4033, 0, -711, 0, 26, 27 },
    { 27, 10, -46, 0, 4086, 276, -43, 0, 38, 32 },
    { 27, 16, -46, 0, 4086, -276, -43, 0, 38, 22 },
    { 27, 20, -42, 0, 4018, -666, -428, 0, 31, 14 },
    { 28, 20, -37, 0, 3922, -666, -971, 0, 21, 14 },
    { 29, 16, -33, 0, 3855, -276, -1356, 0, 13, 22 },
    { 29, 10, -33, 0, 3855, 276, -1356, 0, 13, 32 },
    { 12, 13, -36, 0, -4033, 0, 711, 0, 26, 27 },
    { 13, 1, -42, 0, -1622, -3560, -1211, 0, 1, 50 },
    { 28, 1, -44, 0, 1110, -3560, -1693, 0, 1, 53 },
    { 28, 1, -44, 0, 1110, -3560, -1693, 0, 10, 53 },
    { 13, 1, -42, 0, -1622, -3560, -1211, 0, 10, 50 },
    { 12, 18, -49, 0, -1984, 1474, -3265, 0, 2, 17 },
    { 12, 8, -49, 0, -1984, -1474, -3265, 0, 2, 36 },
    { 13, 1, -42, 0, -1622, -3560, -1211, 0, 16, 50 },
    { 15, 1, -32, 0, -1110, -3560, 1693, 0, 36, 50 },
    { 16, 8, -25, 0, -748, -1474, 3747, 0, 49, 36 },
    { 16, 18, -25, 0, -748, 1474, 3747, 0, 49, 17 },
    { 15, 25, -32, 0, -1110, 3560, 1693, 0, 36, 3 },
    { 13, 25, -42, 0, -1622, 3560, -1211, 0, 16, 3 }
};
static u_short rueda_t_d001_Indices[] = {
    0, 2, 1, 1, 3, 0, 1, 4, 3, 1, 5, 4, 6, 8, 7, 7, 9, 6, 10, 12, 11, 11, 13, 10, 14, 16, 15, 14, 17, 16, 18, 20, 19, 
    18, 21, 20, 22, 24, 23, 22, 25, 24, 26, 28, 27, 26, 29, 28, 30, 32, 31, 33, 35, 34, 34, 36, 33, 37, 38, 30, 39, 41, 40, 40, 42, 
    39, 43, 44, 37, 45, 47, 46, 46, 48, 45, 49, 50, 43, 51, 53, 52, 53, 51, 54, 55, 56, 49, 57, 59, 58, 58, 60, 57, 61, 62, 55, 63, 
    65, 64, 65, 63, 66, 67, 68, 61, 69, 71, 70, 70, 72, 69, 31, 73, 67, 74, 76, 75, 76, 74, 77, 77, 79, 78, 70, 78, 80, 66, 80, 81, 
    58, 81, 82, 54, 82, 83, 45, 83, 84, 41, 84, 85, 34, 85, 79, 79, 86, 78, 86, 87, 78, 78, 87, 88, 88, 80, 78, 89, 81, 80, 80, 88, 
    89, 82, 81, 89, 89, 90, 82, 82, 90, 83, 90, 91, 83, 92, 84, 83, 83, 91, 92, 84, 92, 93, 93, 85, 84, 93, 86, 79, 85, 93, 79, 94, 
    96, 95, 95, 96, 97, 97, 96, 98, 98, 96, 99, 99, 96, 100, 100, 96, 101, 101, 96, 102, 102, 96, 94, 30, 103, 37, 37, 103, 43, 43, 103, 49, 
    49, 103, 55, 55, 103, 61, 61, 103, 67, 67, 103, 31, 31, 103, 30, 0, 105, 104, 104, 2, 0, 9, 7, 4, 4, 5, 9, 13, 11, 8, 8, 6, 
    13, 10, 15, 12, 10, 14, 15, 17, 19, 16, 17, 18, 19, 21, 23, 20, 21, 22, 23, 25, 27, 24, 25, 26, 27, 29, 106, 28, 29, 107, 106, 42, 
    40, 34, 34, 35, 42, 46, 47, 39, 47, 41, 39, 45, 48, 52, 52, 53, 45, 54, 51, 60, 51, 57, 60, 58, 59, 64, 64, 65, 58, 66, 63, 72, 
    63, 69, 72, 70, 71, 75, 75, 76, 70, 77, 74, 36, 74, 33, 36, 62, 61, 108, 61, 68, 108, 68, 67, 109, 67, 73, 109, 73, 31, 110, 31, 32, 
    110, 32, 30, 111, 30, 38, 111, 38, 37, 112, 37, 44, 112, 44, 43, 113, 43, 50, 113, 50, 49, 114, 49, 56, 114, 56, 55, 115, 55, 62, 115, 82, 
    54, 60, 54, 83, 53, 83, 45, 53, 45, 84, 47, 84, 41, 47, 41, 85, 40, 85, 34, 40, 34, 79, 36, 79, 77, 36, 77, 78, 76, 78, 70, 76, 
    70, 80, 72, 80, 66, 72, 66, 81, 65, 81, 58, 65, 58, 82, 60
};
static SDC_Mesh3D rueda_t_d001_Mesh = {rueda_t_d001_Vertices, rueda_t_d001_Indices, NULL, 432, 116, POLIGON_VERTEX_TEXTURED_NORMAL};
#endif