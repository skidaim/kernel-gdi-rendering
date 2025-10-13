#include "utils.h"
#define M_PI (float)3.1415926
#define XM_PIDIV2 ((float)M_PI/2)
#define XM_1DIV2PI ((float)1/2*M_PI)
#ifdef __cplusplus
extern "C" {
#endif
    int _fltused = 0; // it should be a single underscore since the double one is the mangled name
#ifdef __cplusplus
}
#endif


typedef struct {
    float m[4][4];

} viewmatrix;

viewmatrix viewm;
// Assuming 3D vector structure
typedef struct {
    float x, y, z;

} Vector3;

// Define the screen resolution


BOOLEAN WorldToScreen(Vector3 playerPos, int* screenX, int* screenY) {
    float clipX = playerPos.x * viewm.m[0][0] + playerPos.y * viewm.m[0][1] + playerPos.z * viewm.m[0][2] + viewm.m[0][3];
    float clipY = playerPos.x * viewm.m[1][0] + playerPos.y * viewm.m[1][1] + playerPos.z * viewm.m[1][2] + viewm.m[1][3];
    float clipW = playerPos.x * viewm.m[3][0] + playerPos.y * viewm.m[3][1] + playerPos.z * viewm.m[3][2] + viewm.m[3][3];

    if (clipW < 0.1f) return FALSE;

    float ndcX = clipX / clipW;
    float ndcY = clipY / clipW;

    *screenX = (int)((ndcX + 1.0f) * 0.5f * SCREEN_WIDTH);
    *screenY = (int)((1.0f - ndcY) * 0.5f * SCREEN_HEIGHT);

    return TRUE;
}
