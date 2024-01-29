#include "title.h"
#include <nds.h>
#include <stdio.h>
#include <maxmod9.h>
#include "star.h"
#include "chris.h"
#include "soundbank.h"
#include "soundbank_bin.h"

PrintConsole Title::bottomScreen;
int Title::angle = 0;
int Title::brightness = 0;
bool Title::active = true;
bool Title::explode = false;
bool Title::godown = false;
bool Title::goup = true;
int Title::lifetime = 0;
int Title::bg3 = 0;
u16* Title::starValue = 0;
double Title::scale = 1.0;

void Title::clean() {

}
void Title::load() {
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

	u8* gfx = (u8*)starTiles; 
	starValue = oamAllocateGfx(&oamMain, SpriteSize_8x8, SpriteColorFormat_16Color);
	dmaCopy(gfx, starValue, 8*8);
	dmaCopy(starPal, SPRITE_PALETTE, 32);

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
int Title::logic() {

	//ALL OF THIS LOGIC IS STOLEN FROM KNUXFANPONG/KNUXFANTETRIMINOS
	//because im lazy

        //std::cout << lifetime << "\n";






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
	for(int i = 0; i < 128; i++) {
		    oamSet(&oamMain, i, ((i)%32)*8, ((i)/32)*8, 0, 0, SpriteSize_8x8, SpriteColorFormat_16Color, 
    starValue, -1, false, false, false, false, false);
	}
	oamUpdate(&oamMain);

	setBrightness(3,-16+16*(brightness/100.0));
	lifetime++;
	angle++;
	return 0;
}
