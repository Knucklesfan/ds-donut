#include "intro.h"
#include <nds.h>
#include <stdio.h>
#include "lakepng.h"
#include "surprisingly.h"
PrintConsole Intro::bottomScreen;
PrintConsole Intro::topScreen;
int Intro::bg[2];
int Intro::angle = 0;
int Intro::knuxfanx = 0;
void Intro::load() {
    videoSetModeSub(MODE_0_2D);
    videoSetMode(MODE_0_2D);
	//using mode 0 for both for MAXIMUM BACKGROUNDS!!!

	vramSetBankA(VRAM_A_MAIN_BG);
  	vramSetBankC(VRAM_C_SUB_BG);

  	bgExtPaletteEnable();

	//initialize the memory for our two backgrounds
	bg[0] = bgInit   (0, BgType_Text8bpp, BgSize_T_256x256, 31, 0);
	bg[1] = bgInit   (1, BgType_Text8bpp, BgSize_T_256x256, 30, 4);

	dmaCopy(lakepngTiles,  bgGetGfxPtr(bg[1]), lakepngTilesLen);
	dmaCopy(surprisinglyTiles, bgGetGfxPtr(bg[0]), surprisinglyTilesLen);

	dmaCopy(lakepngMap,  bgGetMapPtr(bg[1]), lakepngMapLen);
	dmaCopy(surprisinglyMap, bgGetMapPtr(bg[0]), surprisinglyMapLen);

 	vramSetBankE(VRAM_E_LCD); // for main engine
	dmaCopy(lakepngPal,  &VRAM_E_EXT_PALETTE[1][0],  lakepngPalLen);  // bg 1, slot 0
    dmaCopy(surprisinglyPal,  &VRAM_E_EXT_PALETTE[0][0],  surprisinglyPalLen);  // bg 0, slot 0
 	vramSetBankE(VRAM_E_BG_EXT_PALETTE);     // for main engine

	consoleInit(&bottomScreen, 3, BgType_Text4bpp, BgSize_T_256x256, 31, 0, false, true);
	
	// consoleInit(&topScreen, 3, BgType_Text4bpp, BgSize_T_256x256, 30, 31, true, true);

    // consoleSelect(&topScreen);
	// iprintf("\x1b[2J");
	// iprintf("\x1b[0m");

    // iprintf("DSDonut!");
	consoleSelect(&bottomScreen);
	iprintf("Press start...");
	
}
int Intro::logic() {
	// bgSetScroll(bg[1], 0, 0);
	bgSetScroll(bg[0], angle, angle);

	bgUpdate();
	    scanKeys();
    u16 keys = keysDown();
    if (keys & KEY_L)
    {
        if (knuxfanx > -16)
        {
            knuxfanx--;
        }
    };
    if (keys & KEY_R)
    {
        if (knuxfanx < 0)
        {
            knuxfanx++;
        }
    }
	setBrightness(1,knuxfanx);
	angle++;
	return 0;
}