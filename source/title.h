#pragma once
#include <nds.h>
namespace Title {
    extern PrintConsole bottomScreen;
    extern bool active;
    extern float starX[128];
    extern float starY[128];
    extern float starZ[128];

    extern int lifetime;
    extern int bg3;
    extern int firstTex;
    extern int secondTex;
    extern u16* starValue;
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