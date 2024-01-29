#pragma once
#include <nds.h>
namespace ChrisScreen {
    extern PrintConsole topScreen;
    extern PrintConsole bottomScreen;
    extern bool active;
    extern int bg[4];
    extern u16* topscreen;
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