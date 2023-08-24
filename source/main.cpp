/*---------------------------------------------------------------------------------

	$Id: main.cpp,v 1.13 2008-12-02 20:21:20 dovoto Exp $

	Simple console print demo
	-- dovoto


---------------------------------------------------------------------------------*/
#include <nds.h>
#include <maxmod9.h>
#include <cmath>
#include <stdio.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "soundbank.h"
#include "soundbank_bin.h"
#include "fortniteballs.h"

volatile int frame = 0;

#define BUFFER_WIDTH 32
#define BUFFER_HEIGHT 21
#define BUFFER_SIZE (BUFFER_WIDTH*BUFFER_HEIGHT)

#define IMAGE_SCALE 30
#define TWO_PI 6.28

// donut point traversal increment (inverse of density)
#define DELTA_J 0.07
#define DELTAJSIZE 90
#define DELTA_I 0.02
#define DELTAISIZE 314
static int color;
#define R(mul,shift,x,y) \
  _=x; \
  x -= mul*y>>shift; \
  y += mul*_>>shift; \
  _ = 3145728-x*x-y*y>>11; \
  x = x*_>>10; \
  y = y*_>>10;

// rotation increment around each axis

int8_t b[1760], z[1760];
	  int sA=1024,cA=0,sB=1024,cB=0,_;

//---------------------------------------------------------------------------------
void Vblank() {


	frame++;
//---------------------------------------------------------------------------------
}
	
//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------

	touchPosition touchXY;

	PrintConsole topScreen;
	PrintConsole bottomScreen;
	videoSetMode(MODE_5_2D);
	videoSetModeSub(MODE_0_2D);
	vramSetBankA(VRAM_A_MAIN_BG);
	vramSetBankC(VRAM_C_SUB_BG);

	consoleInit(&bottomScreen, 3,BgType_Text4bpp, BgSize_T_256x256, 31, 0, false, true);
	bgInit(3, BgType_Bmp16, BgSize_B16_256x256, 0,0);
	
	decompress(fortniteballsBitmap, BG_GFX,  LZ77Vram);
	consoleSelect(&bottomScreen);
	lcdMainOnBottom();
	irqSet(IRQ_VBLANK, Vblank);
	mmInitDefaultMem((mm_addr)soundbank_bin);
	
	// load the module
	mmLoad( MOD_FLATOUTLIES );

	mmStart( MOD_FLATOUTLIES, MM_PLAY_LOOP );
	while(true) {
		scanKeys();
		
		u16 keys = keysHeld();
		if (keys & KEY_LEFT) {
			if(color > 0) {
				color--;
			}
		};
		if(keys & KEY_RIGHT) {
			if(color < 9) {
				color++;
			}
		}
		printf("\x1b[40;37m");
		// render();
		memset(b, 32, 1760);  // text buffer
		memset(z, 127, 1760);   // z buffer
		int sj=0, cj=1024;
		for (int j = 0; j < 90; j++) {
		int si = 0, ci = 1024;  // sine and cosine of angle i
		for (int i = 0; i < 324; i++) {
			int R1 = 1, R2 = 2048, K2 = 5120*1024;

			int x0 = R1*cj + R2,
				x1 = ci*x0 >> 10,
				x2 = cA*sj >> 10,
				x3 = si*x0 >> 10,
				x4 = R1*x2 - (sA*x3 >> 10),
				x5 = sA*sj >> 10,
				x6 = K2 + R1*1024*x5 + cA*x3,
				x7 = cj*si >> 10,
				x = BUFFER_WIDTH/2 + IMAGE_SCALE/2*(cB*x1 - sB*x4)/x6,
				y = 12 + (IMAGE_SCALE/2)*(cB*x4 + sB*x1)/x6,
				N = (-cA*x7 - cB*((-sA*x7>>10) + x2) - ci*(cj*sB >> 10) >> 10) - x5 >> 7;

			int o = x + BUFFER_WIDTH * y;
			int8_t zz = (x6-K2)>>15;
			if (22 > y && y > 0 && x > 0 && BUFFER_WIDTH > x && zz < z[o]) {
			z[o] = zz;
			b[o] = ".,-~:;=!*#$@"[N > 0 ? N : 0];
			}
			R(5, 8, ci, si)  // rotate i
		}
		R(9, 7, cj, sj)  // rotate j
		}
		printf("\x1b[2J");
		printf("\x1b[0m");
		printf("LEFT/RIGHT CHANGE COLOR %d",color);
		switch(color) {
			default: printf("\x1b[37;5m");break;
			case 1: printf("\x1b[31;3m");break;
			case 2: printf("\x1b[32;3m");break;
			case 3: printf("\x1b[33;3m");break;
			case 4: printf("\x1b[34;3m");break;
			case 5: printf("\x1b[35;3m");break;
			case 6: printf("\x1b[36;3m");break;
			case 7: printf("\x1b[29;3m");break;
			case 8: printf("\x1b[38;3m");break;
			case 9: {
				int raindo = frame/10;
				switch(raindo%8) {
					case 0: printf("\x1b[31;3m");break;
					case 1: printf("\x1b[32;3m");break;
					case 2: printf("\x1b[33;3m");break;
					case 3: printf("\x1b[34;3m");break;
					case 4: printf("\x1b[35;3m");break;
					case 5: printf("\x1b[36;3m");break;
					case 6: printf("\x1b[29;3m");break;
					case 7: printf("\x1b[38;3m");break;
				}
			}

		}
			// printf("\x1b[H");

		for (int k = 0; BUFFER_WIDTH*BUFFER_HEIGHT > k; k++)
		
		putchar(k % BUFFER_WIDTH ? b[k] : 10);
		R(5, 7, cA, sA);
		R(5, 8, cB, sB);
		printf("\x1b[23A");

	}
	return 0;
}

