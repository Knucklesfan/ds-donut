#include "intro.h"
#include <stdio.h>
#include <nds.h>

#include "lakepng.h"
#include "surprisingly.h"
#include "border.h"
#include "checkerboard.h"
#include "knuxfan.h"
#include "presents.h"
PrintConsole Intro::bottomScreen;
PrintConsole Intro::topScreen;
int Intro::angle = 0;
int Intro::brightness = 0;
bool Intro::active = true;
bool Intro::explode = false;
bool Intro::godown = false;
bool Intro::goup = true;
float Intro::knCoord[4] = {16,0,80,0}; //xy pieces
float Intro::presCoord[4] = {80+32+16,192,80+32+16+64,192}; //xy pieces
float Intro::knRot = 0;
float Intro::yVel = 0;
int Intro::lifetime = 0;
int Intro::bg3 = 0;
int Intro::bgS = 0;
double Intro::scale = 1.0;
int Intro::border = 0;
int Intro::checkerboard = 0;

u16* Intro::knuxfanScreen[2]= {0,0};
u16* Intro::presents[2] = {0,0};
float lerp(double a, double b, double t)    {
        if (t <= 0.5)
            return a+(b-a)*t;
        else
            return b-(b-a)*(1.0-t);
    }
void Intro::clean() {

}
void Intro::load() {
	
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
	bgS = bgInit(0, BgType_Text4bpp, BgSize_T_256x256, 16, 0);

	checkerboard = bgInitSub(1, BgType_Text8bpp, BgSize_T_256x256, 5, 0);
  	border = bgInitSub(0, BgType_Text8bpp, BgSize_T_256x256, 4, 1);
	consoleInit(&bottomScreen, 3, BgType_Text4bpp, BgSize_T_256x256, 31, 4, false, true);
	consoleSelect(&bottomScreen);
	// dmaCopy(surprisinglyBitmap, bgGetGfxPtr(bg[1]), 256*256);
	dmaCopy(surprisinglyTiles,  bgGetGfxPtr(bgS), surprisinglyTilesLen);
	dmaCopy(surprisinglyMap, bgGetMapPtr(bgS), surprisinglyMapLen);
	dmaCopy(surprisinglyPal, BG_PALETTE, 256*2);

	dmaCopy(lakepngBitmap, bgGetGfxPtr(bg3), 256*256);
	dmaCopy(lakepngPal, BG_PALETTE, 256*2);

	dmaCopy(checkerboardTiles,  bgGetGfxPtr(checkerboard), checkerboardTilesLen);
	dmaCopy(borderTiles, bgGetGfxPtr(border), borderTilesLen);

	dmaCopy(checkerboardMap,  bgGetMapPtr(checkerboard), checkerboardMapLen);
	dmaCopy(borderMap, bgGetMapPtr(border), borderMapLen);

	vramSetBankH(VRAM_H_LCD); // for sub engine
	// dmaCopy(lakepngPal,  &VRAM_E_EXT_PALETTE[3][0],  lakepngPalLen);  // bg 1, slot 0
	dmaCopy(checkerboardPal,  &VRAM_H_EXT_PALETTE[1][0],  checkerboardPalLen);  // bg 0, slot 0
	dmaCopy(borderPal, &VRAM_H_EXT_PALETTE[0][0], borderPalLen); // bg 1, slot 12
	vramSetBankH(VRAM_H_SUB_BG_EXT_PALETTE); // for sub engine
  	// consoleInit(&bottomScreen, 3, BgType_Text4bpp, BgSize_T_256x256, 31, 0, false, true);
	
	// consoleInit(&topScreen, 3, BgType_Text4bpp, BgSize_T_256x256, 30, 31, true, true);

    // consoleSelect(&topScreen);
	// iprintf("\x1b[2J");
	// iprintf("\x1b[0m");

    // iprintf("DSDonut!");
	// consoleSelect(&bottomScreen);
	// iprintf("Press start...");
	bgSetPriority(checkerboard,3);
	bgSetPriority(border,1);
	setBackdropColor(0);
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

        if (lifetime > 250) {
            active = false;
			godown = true;
        }
        if(lifetime > 200 && !explode) {
            explode = true;
			yVel=-2;
        }
        //std::cout << lifetime << "\n";
        if(!explode) {
            knCoord[1] = lerp(knCoord[1],80,0.05);
			knCoord[3] = lerp(knCoord[3],80,0.05);
            presCoord[1] = lerp(presCoord[1],80,0.05);
			presCoord[3] = lerp(presCoord[3],80,0.05);
        }
        else if(active) {
            knRot++;
			
            knCoord[2] --;
			knCoord[0] -=2;

            presCoord[2]++;
			presCoord[0]+=2;
            
			knCoord[2] --;
			knCoord[0] -=2;

            knCoord[1] += yVel;
			knCoord[3] += yVel;

            presCoord[1] += yVel;
			presCoord[3] += yVel;

            yVel += 0.25;
        }






	//BEGIN RENDERING!!!
    oamRotateScale(&oamMain, 0, degreesToAngle(knRot), intToFixed(1, 8), intToFixed(1, 8));
	// bgSetScroll(bg[1], 0, 0);

	bgSetScroll(bgS, angle, angle);

	bgSetScroll(checkerboard,-angle,-angle);
	bgSetScroll(bg3,-(4.0-4.0/scale),-(4-4.0/scale));
	
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
    oamSet(&oamMain, 0, knCoord[0], knCoord[1], 0, 0, SpriteSize_64x64, SpriteColorFormat_256Color, 
    knuxfanScreen[0], 0, false, knCoord[1]>=193, false, false, false);
    oamSet(&oamMain, 1, knCoord[2], knCoord[3], 0, 0, SpriteSize_64x64, SpriteColorFormat_256Color, 
    knuxfanScreen[1], 0, false, knCoord[3]>=193, false, false, false);

	//center of circle is 80+32+16+64 = 192
	
    oamSet(&oamMain, 2, presCoord[0], presCoord[1], 0, 0, SpriteSize_64x64, SpriteColorFormat_256Color, 
    presents[0], 0, false, presCoord[1]>=193, false, false, false);
    oamSet(&oamMain, 3, presCoord[2], presCoord[3], 0, 0, SpriteSize_64x64, SpriteColorFormat_256Color, 
    presents[1], 0, false, presCoord[3]>=193, false, false, false);

	oamUpdate(&oamMain);

	setBrightness(3,-16+16*(brightness/100.0));
	lifetime++;
	angle++;
	return 0;
}
