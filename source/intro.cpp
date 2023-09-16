#include "intro.h"
#include <nds.h>
#include <stdio.h>
#include "thugbob.h"
PrintConsole Intro::bottomScreen;
PrintConsole Intro::topScreen;
int Intro::bg = 0;
int Intro::angle = 0;
void Intro::load() {
    videoSetModeSub(MODE_0_2D);
    videoSetMode(MODE_5_2D);
    vramSetBankA(VRAM_A_MAIN_BG);
	vramSetBankB(VRAM_B_MAIN_BG);
	vramSetBankC(VRAM_C_SUB_BG);
	bg = bgInit(3, BgType_Bmp16, BgSize_B16_256x256, 0, 0);
	consoleInit(&bottomScreen, 3, BgType_Text4bpp, BgSize_T_256x256, 31, 0, false, true);
	consoleInit(&topScreen, 0, BgType_Text4bpp, BgSize_T_256x256, 9606+128, 9606, true, true);

	decompress(thugbobBitmap, BG_GFX, LZ77Vram);

    consoleSelect(&topScreen);
    iprintf("DSDonut!");
	consoleSelect(&bottomScreen);
	iprintf("Press start...");
	
}
int Intro::logic() {
	bgSetCenter(bg, 0, 0);
	bgSetRotate(bg, angle);
	bgUpdate();
	return 0;
}