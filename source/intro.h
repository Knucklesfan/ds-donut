#pragma once
#include <nds.h>
namespace Intro {
    extern PrintConsole topScreen;
    extern PrintConsole bottomScreen;
    extern bool active;
    extern int bg[4];
    extern u16* knuxfanScreen[2];
    extern u16* presents[2];
    extern float knCoord[4]; //x,y, and rot
    extern float presCoord[4]; //x,y, and rot
    extern float presRot;
    extern float knRot;
    extern float yVel;
    extern int lifetime;
    extern int brightness;
    extern int angle;
    extern bool godown;
    extern bool goup;
    extern bool explode;
    void load();
    void clean();
    int logic();
    void render();
}