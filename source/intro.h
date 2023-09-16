#pragma once
#include <nds.h>
namespace Intro {
    extern PrintConsole topScreen;
    extern PrintConsole bottomScreen;
    extern bool active;
    extern int bg[2];
    extern int knuxfan[2];
    extern int presents[2];
    extern int knuxfany;
    extern int knuxfanx;
    extern int presentsy;
    extern int presentsx;
    extern int knuxfanangle;
    extern int presentsangle;
    extern int angle;
    extern bool godown;
    void load();
    void clean();
    int logic();
    void render();
}