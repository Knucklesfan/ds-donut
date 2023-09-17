#include "intro.h"
#include <nds.h>
#include <stdio.h>
#include "lakepng.h"
#include "surprisingly.h"
#include "border.h"
#include "checkerboard.h"
#include "knuxfan.h"
#include "presents.h"
PrintConsole Intro::bottomScreen;
PrintConsole Intro::topScreen;
int Intro::bg[4];
int Intro::angle = 0;
int Intro::brightness = 0;
bool Intro::active = true;
bool Intro::explode = false;
bool Intro::godown = false;
bool Intro::goup = true;
float Intro::knCoord[4] = {16,0,80,0}; //xy pieces
float Intro::presCoord[4] = {80+32+16,192,80+32+16+64,192}; //xy pieces
float Intro::knRot = 0;
u16* Intro::knuxfanScreen[2]= {0,0};
u16* Intro::presents[2] = {0,0};

void Intro::load() {
	
  videoSetMode(MODE_0_2D);
  videoSetModeSub(MODE_0_2D);
	vramSetBankA(VRAM_A_MAIN_BG);
	vramSetBankB(VRAM_B_MAIN_SPRITE);
	vramSetBankC(VRAM_C_SUB_BG);
 	oamInit(&oamMain, SpriteMapping_1D_128, false);
	
  	bgExtPaletteEnable();
  	bgExtPaletteEnableSub();
	//initialize the memory for our two backgrounds
	bg[0] = bgInit   (0, BgType_Text8bpp, BgSize_T_256x256, 31, 0);
	bg[1] = bgInit   (1, BgType_Text8bpp, BgSize_T_256x256, 30, 4);
  	bg[2] = bgInitSub(1, BgType_Text8bpp, BgSize_T_256x256, 31, 0);
  	bg[3] = bgInitSub(0, BgType_Text8bpp, BgSize_T_256x256, 30, 4);
	dmaCopy(lakepngTiles,  bgGetGfxPtr(bg[1]), lakepngTilesLen);
	dmaCopy(surprisinglyTiles, bgGetGfxPtr(bg[0]), surprisinglyTilesLen);
	dmaCopy(checkerboardTiles,  bgGetGfxPtr(bg[2]), checkerboardTilesLen);
	dmaCopy(borderTiles, bgGetGfxPtr(bg[3]), borderTilesLen);

	dmaCopy(lakepngMap,  bgGetMapPtr(bg[1]), lakepngMapLen);
	dmaCopy(surprisinglyMap, bgGetMapPtr(bg[0]), surprisinglyMapLen);
	dmaCopy(checkerboardMap,  bgGetMapPtr(bg[2]), checkerboardMapLen);
	dmaCopy(borderMap, bgGetMapPtr(bg[3]), borderMapLen);

 	vramSetBankE(VRAM_E_LCD); // for main engine
	vramSetBankH(VRAM_H_LCD); // for sub engine
	dmaCopy(lakepngPal,  &VRAM_E_EXT_PALETTE[1][0],  lakepngPalLen);  // bg 1, slot 0
    dmaCopy(surprisinglyPal,  &VRAM_E_EXT_PALETTE[0][0],  surprisinglyPalLen);  // bg 0, slot 0
	dmaCopy(checkerboardPal,  &VRAM_H_EXT_PALETTE[1][0],  checkerboardPalLen);  // bg 0, slot 0
	dmaCopy(borderPal, &VRAM_H_EXT_PALETTE[0][0], borderPalLen); // bg 1, slot 12
  	vramSetBankE(VRAM_E_BG_EXT_PALETTE);     // for main engine
	vramSetBankH(VRAM_H_SUB_BG_EXT_PALETTE); // for sub engine
  	// consoleInit(&bottomScreen, 3, BgType_Text4bpp, BgSize_T_256x256, 31, 0, false, true);
	
	// consoleInit(&topScreen, 3, BgType_Text4bpp, BgSize_T_256x256, 30, 31, true, true);

    // consoleSelect(&topScreen);
	// iprintf("\x1b[2J");
	// iprintf("\x1b[0m");

    // iprintf("DSDonut!");
	// consoleSelect(&bottomScreen);
	// iprintf("Press start...");
	bgSetPriority(bg[2],3);
	bgSetPriority(bg[3],0);

	dmaCopy(knuxfanPal, SPRITE_PALETTE, 512);
	u8* gfx = (u8*)knuxfanTiles; 
	knuxfanScreen[0] = oamAllocateGfx(&oamMain, SpriteSize_64x64, SpriteColorFormat_256Color);
	dmaCopy(gfx, knuxfanScreen[0], 64*64);
	gfx+=64*64;
	knuxfanScreen[1] = oamAllocateGfx(&oamMain, SpriteSize_64x64, SpriteColorFormat_256Color);
	dmaCopy(gfx, knuxfanScreen[1], 64*64);
	
	dmaCopy(presentsPal, SPRITE_PALETTE+512, 512);
	
	gfx = (u8*)presentsTiles; 
	presents[0] = oamAllocateGfx(&oamMain, SpriteSize_64x64, SpriteColorFormat_256Color);
	dmaCopy(gfx, presents[0], 64*64);
	gfx+=64*64;
	presents[1] = oamAllocateGfx(&oamMain, SpriteSize_64x64, SpriteColorFormat_256Color);
	dmaCopy(gfx, presents[1], 64*64);


}
int Intro::logic() {

	//ALL OF THIS LOGIC IS STOLEN FROM KNUXFANPONG/KNUXFANTETRIMINOS
	//because im lazy


        if (lifetime > 1000) {
            active = false;
        }
        if(lifetime > 650) {
            explode = true;
        }
        //std::cout << lifetime << "\n";
        if(!explode) {
            knCoord[1] = lerp(knCoord[1],96,0.05);
			knCoord[3] = lerp(knCoord[3],96,0.05);
            presCoord[1] = lerp(presCoord[1],96,0.05);
			presCoord[3] = lerp(presCoord[3],96,0.05);
        }
        else {
            knRot++;
			
            knCoord[2] -= 0.5;
			knCoord[0] --;

            presCoord[2] += 0.5;
			presCoord[0]++;
            
			knCoord[2] -= 0.5;
			knCoord[0] --;

            knCoord[1] += yVel*0.5;
			knCoord[3] += yVel*0.5;

            presentsy += yvelocity*deltatime*0.5;

            yVel += 0.0025;
        }






	//BEGIN RENDERING!!!

	// bgSetScroll(bg[1], 0, 0);
	bgSetScroll(bg[0], angle, angle);

	bgSetScroll(bg[2],-angle,-angle);
	bgUpdate();
	if(!godown && active && brightness < 100) {
		brightness++;
	}
    oamSet(&oamMain, 0, 16, 8, 0, 0, SpriteSize_64x64, SpriteColorFormat_256Color, 
    knuxfanScreen[0], 0, false, false, false, false, false);
    oamSet(&oamMain, 1, 80, 8, 0, 0, SpriteSize_64x64, SpriteColorFormat_256Color, 
    knuxfanScreen[1], 0, false, false, false, false, false);

	//center of circle is 80+32+16+64 = 192
	
    oamSet(&oamMain, 2, 80+32+16, 8, 0, 0, SpriteSize_64x64, SpriteColorFormat_256Color, 
    presents[0], 0, false, false, false, false, false);
    oamSet(&oamMain, 3, 80+32+16+64, 8, 0, 0, SpriteSize_64x64, SpriteColorFormat_256Color, 
    presents[1], 0, false, false, false, false, false);

	oamUpdate(&oamMain);

	setBrightness(3,-16+16*(brightness/100.0));
	angle++;
	return 0;
}
float lerp(double a, double b, double t)    {
        if (t <= 0.5)
            return a+(b-a)*t;
        else
            return b-(b-a)*(1.0-t);
    }