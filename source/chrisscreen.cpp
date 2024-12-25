#include "chrisscreen.h"
#include <nds.h>
#include <stdio.h>
#include <maxmod9.h>

#include "chris.h"
#include "soundbank.h"

PrintConsole ChrisScreen::bottomScreen;
int ChrisScreen::angle = 0;
int ChrisScreen::brightness = 0;
bool ChrisScreen::active = true;
bool ChrisScreen::explode = false;
bool ChrisScreen::godown = false;
bool ChrisScreen::goup = true;
int ChrisScreen::lifetime = 0;
int ChrisScreen::bg3 = 0;
double ChrisScreen::scale = 1.0;

void ChrisScreen::clean() {
	mmStop();
}
void ChrisScreen::load() {
	mmLoad(MOD_WHISTLE);

	mmStart(MOD_WHISTLE, MM_PLAY_LOOP);

	videoSetMode(MODE_5_2D);
	videoSetModeSub(MODE_0_2D);
    vramSetBankA(VRAM_A_MAIN_BG_0x06000000);
	vramSetBankB(VRAM_B_MAIN_SPRITE);
	vramSetBankC(VRAM_C_SUB_BG);
 	oamInit(&oamMain, SpriteMapping_1D_128, false);
	
  	// bgExtPaletteEnable();
  	bgExtPaletteEnableSub();
	//initialize the memory for our two backgrounds
	bg3 = bgInit(3, BgType_Bmp8, BgSize_B8_256x256, 3,0);

	consoleInit(&bottomScreen, 3, BgType_Text4bpp, BgSize_T_256x256, 31, 4, false, true);
	consoleSelect(&bottomScreen);

	dmaCopy(chrisBitmap, bgGetGfxPtr(bg3), 256*256);
	dmaCopy(chrisPal, BG_PALETTE, 256*2);

  	// consoleInit(&bottomScreen, 3, BgType_Text4bpp, BgSize_T_256x256, 31, 0, false, true);
	
	// consoleInit(&topScreen, 3, BgType_Text4bpp, BgSize_T_256x256, 30, 31, true, true);

    // consoleSelect(&topScreen);
	// iprintf("\x1b[2J");
	// iprintf("\x1b[0m");

    // iprintf("DSDonut!");
	// consoleSelect(&bottomScreen);
	// iprintf("Press start...");
	setBackdropColor(0);



}
int ChrisScreen::logic() {

	//ALL OF THIS LOGIC IS STOLEN FROM KNUXFANPONG/KNUXFANTETRIMINOS
	//because im lazy

        if (lifetime > 250) {
            active = false;
			godown = true;
        }
        //std::cout << lifetime << "\n";


    scanKeys();
    u16 keys = keysDown();
    if (keys & KEY_START) {
		return 1;
	}




	//BEGIN RENDERING!!!
    // oamRotateScale(&oamMain, 0, degreesToAngle(knRot), intToFixed(1, 8), intToFixed(1, 8));
	// bgSetScroll(bg[1], 0, 0);

	// bgSetScroll(bgS, angle, angle);

	// bgSetScroll(checkerboard,-angle,-angle);
	bgSetScroll(bg3,-(4.0-4.0/scale*4)-10,-(4-4.0/scale*4)+(float)lifetime/5.0);
	
	bgSetScale(bg3,((int)((scale)*(1<<8))),((int)((scale)*(1<<8))));
	bgUpdate();
	if(!godown && active && brightness < 100) {
		brightness++;
	}
	if(!active && scale < 0x100000) {
		scale -= 0.01;
	}
	if(!active && scale <= 0 && brightness > 0) {
		brightness--;
	}
	if(brightness <= 0 && !active) {
		return 1;
	}

	setBrightness(3,-16+16*(brightness/100.0));
	lifetime++;
	angle++;
	return 0;
}
