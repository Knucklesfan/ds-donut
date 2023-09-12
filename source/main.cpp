/*---------------------------------------------------------------------------------

ITS DS DONUT BABY!!!!!

    OOOOOO
   O      O
  O   __   O
  O  |  |  O
  O  |  |  O
  O   --   O
   O      O
    OOOOOO

By Knuxfan
originally taken from donut.c by Andy Sloane (https://www.a1k0n.net/2021/01/13/optimizing-donut.html in particular!)
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
#include "dsconcert.h"
#include "FINALLY.h"
#include "thugbob.h"
#include "DSDONUT.h"
volatile int frame = 0;

#define BUFFER_WIDTH 32
#define BUFFER_HEIGHT 21
#define BUFFER_SIZE (BUFFER_WIDTH * BUFFER_HEIGHT)

#define IMAGE_SCALE 30
#define TWO_PI 6.28

// donut point traversal increment (inverse of density)
#define DELTA_J 0.07
#define DELTAJSIZE 90
#define DELTA_I 0.02
#define DELTAISIZE 314
static int color;
static bool cube = false;
void lineDraw(int8_t *buffer, int x, int y, int x2, int y2);
void bresenham(int8_t *buffer, int x1, int y1, int x2, int y2);

#define R(mul, shift, x, y)            \
	_ = x;                             \
	x -= mul * y >> shift;             \
	y += mul * _ >> shift;             \
	_ = 3145728 - x * x - y * y >> 11; \
	x = x * _ >> 10;                   \
	y = y * _ >> 10;

// rotation increment around each axis

int8_t b[1760], z[1760];
int8_t cb[1760]; // cube buffer
int sA = 1024, cA = 0, sB = 1024, cB = 0, _;
uint8_t visualize;
int edges[12][2] = {
	{0, 1}, {1, 3}, {3, 2}, {2, 0}, {4, 5}, {5, 7}, {7, 6}, {6, 4}, {0, 4}, {1, 5}, {2, 6}, {3, 7}};
float nodes[8][3] = {
	{-100, -100, -100},
	{-100, -100, +100},
	{-100, +100, -100},
	{-100, +100, +100},
	{+100, -100, -100},
	{+100, -100, +100},
	{+100, +100, -100},
	{+100, +100, +100}};
float rad(float deg);
void rotateZ3D(float t);
void rotateX3D(float t);
void rotateY3D(float t);
int song = 0;
//---------------------------------------------------------------------------------
void Vblank()
{

	frame++;
	if (visualize > 0)
	{
		visualize--;
	}
	//---------------------------------------------------------------------------------
}
mm_word myEventHandler(mm_word msg, mm_word param)
{
	//---------------------------------------------------------------------------------
	switch (msg)
	{

	case MMCB_SONGMESSAGE: // process song messages

		// if song event 1 is triggered, set sprite's y velocity to make it jump
		if (param == 1)
			visualize = 10;

		break;

	case MMCB_SONGFINISHED: // process song finish message (only triggered in songs played with MM_PLAY_ONCE)

		break;
	}

	return 0;
}
//---------------------------------------------------------------------------------
int main(void)
{
	//---------------------------------------------------------------------------------


	PrintConsole bottomScreen;
	videoSetMode(MODE_5_2D);
	videoSetModeSub(MODE_0_2D);
	vramSetBankA(VRAM_A_MAIN_BG);
	vramSetBankC(VRAM_C_SUB_BG);

	consoleInit(&bottomScreen, 3, BgType_Text4bpp, BgSize_T_256x256, 31, 0, false, true);
	bgInit(3, BgType_Bmp16, BgSize_B16_256x256, 0, 0);

	decompress(dsconcertBitmap, BG_GFX, LZ77Vram);

	consoleSelect(&bottomScreen);
	lcdMainOnBottom();
	irqSet(IRQ_VBLANK, Vblank);
	mmInitDefaultMem((mm_addr)soundbank_bin);
	mmSetEventHandler(myEventHandler);
	// load the module
	mmLoad(MOD_ADDICTI);

	mmStart(MOD_ADDICTI, MM_PLAY_LOOP);
	while (true)
	{
		scanKeys();

		u16 keys = keysDown();
		if (keys & KEY_L)
		{
			if (color > 0)
			{
				color--;
			}
		};
		if (keys & KEY_R)
		{
			if (color < 9)
			{
				color++;
			}
		}
		if (keys & KEY_SELECT) {
			cube = !cube;
		}
		if (keys & KEY_START) {
			if(song < 4) {
				song++;
			}
			else {
				song = 0;
			}
			
			mmStop();

			switch(song) {
				default: {
					decompress(dsconcertBitmap, BG_GFX, LZ77Vram);
					mmUnload(MOD_FLATOUTLIES);
					mmLoad(MOD_ADDICTI);
					mmStart(MOD_ADDICTI, MM_PLAY_LOOP);
					break;
				}
				case 1: {
					decompress(FINALLYBitmap, BG_GFX, LZ77Vram);
					mmUnload(MOD_ADDICTI);
					mmLoad(MOD_2ND_PM);
					mmStart(MOD_2ND_PM, MM_PLAY_LOOP);
					break;

				}
				case 2: {
					decompress(DSDONUTBitmap, BG_GFX, LZ77Vram);
					mmUnload(MOD_2ND_PM);
					mmLoad(MOD_VRC6N001);
					mmStart(MOD_VRC6N001, MM_PLAY_LOOP);
					break;
				}
				case 3: {
					decompress(fortniteballsBitmap, BG_GFX, LZ77Vram);
					mmUnload(MOD_VRC6N001);
					mmLoad(MOD_CANT_STOP_COMING);
					mmStart(MOD_CANT_STOP_COMING, MM_PLAY_LOOP);
					break;
				}
				case 4: {
					decompress(thugbobBitmap, BG_GFX, LZ77Vram);
					mmUnload(MOD_CANT_STOP_COMING);
					mmLoad(MOD_FLATOUTLIES);
					mmStart(MOD_FLATOUTLIES, MM_PLAY_LOOP);
					break;
				}

			}
		}
		if (cube)
		{
			iprintf("\x1b[2J");
			iprintf("\x1b[0m");
			iprintf("L/R: COLOR SEL: SHAPE ST: MUSIC\n");

			memset(cb, 32, 1760); // text buffer
			memset(b, 32, 1760);  // text buffer
			for (int *indices : edges)
			{
				float *n1 = nodes[indices[0]];
				float *n2 = nodes[indices[1]];
				int x1 = (int)(n1[0] * (0.05) + 16); // hardcoding this cuz this is a DS (and will always be), we dont need to be dynamic or something
				int y1 = (int)(n1[1] * (0.04375) + 11);
				int x2 = (int)(n2[0] * (0.05) + 16);
				int y2 = (int)(n2[1] * (0.04375) + 11);
				//printf("%.0lf:%.0lf:%.0lf:%.0lf\n",x1,y1,x2,y2);
				bresenham(cb, x1, y1, x2, y2);
				cb[(y1 * BUFFER_WIDTH) + x1] = '@';
				cb[(y2 * BUFFER_WIDTH) + x2] = '@';
			}
			if (visualize > 0)
			{
				if (color == 6)
				{ // white
					printf("\x1b[37;5m");
				}
				else
				{ // cyan
					printf("\x1b[36;3m");
				}
			}
			else
			{
				switch (color)
				{
				default:
					printf("\x1b[37;5m");
					break;
				case 1:
					printf("\x1b[31;3m");
					break;
				case 2:
					printf("\x1b[32;3m");
					break;
				case 3:
					printf("\x1b[33;3m");
					break;
				case 4:
					printf("\x1b[34;3m");
					break;
				case 5:
					printf("\x1b[35;3m");
					break;
				case 6:
					printf("\x1b[36;3m");
					break;
				case 7:
					printf("\x1b[29;3m");
					break;
				case 8:
					printf("\x1b[38;3m");
					break;
				case 9:
				{
					int raindo = frame / 10;
					switch (raindo % 8)
					{
					case 0:
						printf("\x1b[31;3m");
						break;
					case 1:
						printf("\x1b[32;3m");
						break;
					case 2:
						printf("\x1b[33;3m");
						break;
					case 3:
						printf("\x1b[34;3m");
						break;
					case 4:
						printf("\x1b[35;3m");
						break;
					case 5:
						printf("\x1b[36;3m");
						break;
					case 6:
						printf("\x1b[29;3m");
						break;
					case 7:
						printf("\x1b[38;3m");
						break;
					}
				}
				}
			}

			for (int k = 0; BUFFER_WIDTH * BUFFER_HEIGHT > k; k++) {
				putchar(k % BUFFER_WIDTH ? cb[k] : 10);
			}
			rotateZ3D(0.02);
			rotateY3D(0.01);
		}
		else
		{

			// render();
			memset(b, 32, 1760);  // text buffer
			memset(z, 127, 1760); // z buffer
			int sj = 0, cj = 1024;
			for (int j = 0; j < 90; j++)
			{
				int si = 0, ci = 1024; // sine and cosine of angle i
				for (int i = 0; i < 324; i++)
				{
					int R1 = 1, R2 = 2048, K2 = 5120 * 1024;

					int x0 = R1 * cj + R2,
						x1 = ci * x0 >> 10,
						x2 = cA * sj >> 10,
						x3 = si * x0 >> 10,
						x4 = R1 * x2 - (sA * x3 >> 10),
						x5 = sA * sj >> 10,
						x6 = K2 + R1 * 1024 * x5 + cA * x3,
						x7 = cj * si >> 10,
						x = BUFFER_WIDTH / 2 + IMAGE_SCALE / 2 * (cB * x1 - sB * x4) / x6,
						y = 12 + (IMAGE_SCALE / 2) * (cB * x4 + sB * x1) / x6,
						N = (-cA * x7 - cB * ((-sA * x7 >> 10) + x2) - ci * (cj * sB >> 10) >> 10) - x5 >> 7;

					int o = x + BUFFER_WIDTH * y;
					int8_t zz = (x6 - K2) >> 15;
					if (22 > y && y > 0 && x > 0 && BUFFER_WIDTH > x && zz < z[o])
					{
						z[o] = zz;
						b[o] = ".,-~:;=!*#$@"[N > 0 ? N : 0];
					}
					R(5, 8, ci, si) // rotate i
				}
				R(9, 7, cj, sj) // rotate j
			}
			printf("\x1b[2J");
			printf("\x1b[0m");
			iprintf("L/R: COLOR SEL: SHAPE ST: MUSIC\n");
			if (visualize > 0)
			{
				if (color == 6)
				{ // white
					printf("\x1b[37;5m");
				}
				else
				{ // cyan
					printf("\x1b[36;3m");
				}
			}
			else
			{
				switch (color)
				{
				default:
					printf("\x1b[37;5m");
					break;
				case 1:
					printf("\x1b[31;3m");
					break;
				case 2:
					printf("\x1b[32;3m");
					break;
				case 3:
					printf("\x1b[33;3m");
					break;
				case 4:
					printf("\x1b[34;3m");
					break;
				case 5:
					printf("\x1b[35;3m");
					break;
				case 6:
					printf("\x1b[36;3m");
					break;
				case 7:
					printf("\x1b[29;3m");
					break;
				case 8:
					printf("\x1b[38;3m");
					break;
				case 9:
				{
					int raindo = frame / 10;
					switch (raindo % 8)
					{
					case 0:
						printf("\x1b[31;3m");
						break;
					case 1:
						printf("\x1b[32;3m");
						break;
					case 2:
						printf("\x1b[33;3m");
						break;
					case 3:
						printf("\x1b[34;3m");
						break;
					case 4:
						printf("\x1b[35;3m");
						break;
					case 5:
						printf("\x1b[36;3m");
						break;
					case 6:
						printf("\x1b[29;3m");
						break;
					case 7:
						printf("\x1b[38;3m");
						break;
					}
				}
				}
			}
			// printf("\x1b[H");

			for (int k = 0; BUFFER_WIDTH * BUFFER_HEIGHT > k; k++) {
				putchar(k % BUFFER_WIDTH ? b[k] : 10);
			}
			R(5, 7, cA, sA);
			R(5, 8, cB, sB);
			printf("\x1b[23A");
		}
		swiWaitForVBlank();
	}
	return 0;
}

void rotateZ3D(float t)
{
	float c = cos(t), s = sin(t);
	for (float *xyz : nodes)
	{
		float x = xyz[0], y = xyz[1];
		xyz[0] = x * c - y * s;
		xyz[1] = y * c + x * s;
	}
}

void rotateY3D(float t)
{
	float c = cos(t), s = sin(t);
	for (float *xyz : nodes)
	{
		float x = xyz[0], z = xyz[2];
		xyz[0] = x * c - z * s;
		xyz[2] = z * c + x * s;
	}
}

void rotateX3D(float t)
{
	float c = cos(t), s = sin(t);
	for (float *xyz : nodes)
	{
		float y = xyz[1], z = xyz[2];
		xyz[1] = y * c - z * s;
		xyz[2] = z * c + y * s;
	}
}

float rad(float deg)
{
	return deg * M_PI / 180;
}
void bresenham(int8_t *buffer, int xa, int ya, int xb, int yb)
{
	bool yLonger=false;
    int shortLen=yb-ya;
    int longLen=xb-xa;
	int temp = 0;
    if (abs(shortLen)>abs(longLen)) {
		temp = shortLen;
		shortLen = longLen;
		longLen = temp;

        yLonger=true;
    }
    int decInc = longLen==0 ?  decInc=0 : ((shortLen << 16) / longLen);

    if (yLonger) {
        ya*=BUFFER_WIDTH;
        yb*=BUFFER_WIDTH;
        if (longLen>0)
            for (int j=0x8000+(xa<<16);ya<=yb;ya+=BUFFER_WIDTH, j+=decInc)
                buffer[ya + (j >> 16)] = '#';  // or a call to your painting method
        else
            for (int j=0x8000+(xa<<16);ya>=yb;ya-=BUFFER_WIDTH, j-=decInc)
                buffer[ya + (j >> 16)] = '#';  // or a call to your painting method
    }
    else
    {
        if (longLen>0)
            for (int j=0x8000+(ya<<16);xa<=xb;++xa, j+=decInc)
                buffer[(j >> 16) * BUFFER_WIDTH + xa] = '#';  // or a call to your painting method
        else
            for (int j=0x8000+(ya<<16);xa>=xb;--xa, j-=decInc)
                buffer[(j >> 16) * BUFFER_WIDTH + xa] =  '#';  // or a call to your painting method
    }
}
