#pragma once
#include <nds.h>
namespace Title {
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
    extern int bg3;
    extern int bgS;
    extern int checkerboard;
    extern int border;
    extern int brightness;
    extern int angle;
    extern double scale;
    extern bool godown;
    extern bool goup;
    extern bool explode;
    void load();
    void clean();
    int logic();
    void render();
}