#pragma once
#include <nds.h>
namespace Intro {
    extern PrintConsole topScreen;
    extern PrintConsole bottomScreen;
    extern bool active;
    extern int bg;
    extern int angle;
    extern bool godown;
    void load();
    void clean();
    int logic();
    void render();
}