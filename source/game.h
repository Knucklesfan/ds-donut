#pragma once

#include <nds.h>
#include <cmath>
#include "utils.h"

namespace Game {
    extern int frame;
    extern int8_t b[1760], z[1760];
    extern int8_t cb[1760]; // cube buffer
    extern int sA, cA, sB, cB;
    extern int underscore;
    extern uint8_t visualize;
    extern int edges[12][2];
    extern float nodes[8][3];
    extern int angle;
    extern int color;
    extern bool cube;
    extern int song;
    extern sprite spr;
    extern bool active;
    extern bool godown;
    extern PrintConsole bottomScreen;
     void load();
     void clean();
     int logic();
     void render();
     void drawCube();
     void drawDonut();
     float rad(float deg);
     void rotateZ3D(float t);
     void rotateX3D(float t);
     void rotateY3D(float t);

}