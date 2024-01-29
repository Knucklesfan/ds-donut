#include "game.h"
#include "soundbank.h"
#include "soundbank_bin.h"
#include "fortniteballs.h"
#include "dsconcert.h"
#include "FINALLY.h"
#include "thugbob.h"
#include "gangster.h"
#include "DSDONUT.h"
#include "marioheadbang.h"
#include <maxmod9.h>
#include <stdio.h>
    int Game::frame = 0;
	int Game::bg = 0;
     int8_t Game::b[1760], Game::z[1760];
     int8_t Game::cb[1760]; // cube buffer
     int Game::sA = 1024, Game::cA = 0, Game::sB = 1024, Game::cB = 0, Game::underscore=0;
     uint8_t Game::visualize;
     int Game::edges[12][2] = {
	{0, 1}, {1, 3}, {3, 2}, {2, 0}, {4, 5}, {5, 7}, {7, 6}, {6, 4}, {0, 4}, {1, 5}, {2, 6}, {3, 7}};
     float Game::nodes[8][3] = {
        {-100, -100, -100},
        {-100, -100, +100},
        {-100, +100, -100},
        {-100, +100, +100},
        {+100, -100, -100},
        {+100, -100, +100},
        {+100, +100, -100},
        {+100, +100, +100}};
     int Game::angle = 0;
     int Game::color = 0;
	 int Game::brightness = 0;
     bool Game::cube = false;
     int Game::song = 0;
     sprite Game::spr;
     bool Game::active = true;
     bool Game::godown = true;
    PrintConsole Game::bottomScreen;

void Game::load() {
    videoSetMode(MODE_5_2D);
	videoSetModeSub(MODE_0_2D);
	
	vramSetBankA(VRAM_A_MAIN_BG);
	vramSetBankB(VRAM_B_MAIN_SPRITE);
	vramSetBankC(VRAM_C_SUB_BG);

	oamInit(&oamMain, SpriteMapping_1D_128, false);
	consoleInit(&bottomScreen, 3, BgType_Text4bpp, BgSize_T_256x256, 31, 0, false, true);

	bg = bgInit(3, BgType_Bmp8, BgSize_B8_256x256, 0,0);
	dmaCopy(dsconcertBitmap, bgGetGfxPtr(bg), 256*256);
	dmaCopy(dsconcertPal, BG_PALETTE, 256*2);

	// decompress(dsconcertBitmap, BG_GFX, LZ77Vram);
    consoleSelect(&bottomScreen);
	lcdMainOnBottom();
	mmLoad(MOD_ADDICTI);
	// dmaFillHalfWords(0x0FFF,(u16*)0x0500041E,0x2); //commenting this, but if you ever wanna set individual palette colors with DMA, this is how you do it. (u16*)0x0500041E magic number corresponds to the location of the graphics + 0x1E
	mmStart(MOD_ADDICTI, MM_PLAY_LOOP);
	spr = {128,96};
	dmaCopy(marioheadbangPal, SPRITE_PALETTE, 512);
	int i;
	u8* gfx = (u8*)marioheadbangTiles; 
	
	for(i = 0; i < 5; i++)
	{
		spr.sprite_gfx_mem[i] = oamAllocateGfx(&oamMain, SpriteSize_64x64, SpriteColorFormat_256Color);
		dmaCopy(gfx, spr.sprite_gfx_mem[i], 64*64);
		gfx += 64*64;
	}
	int angle = 0;

}
int Game::logic() {
	if(brightness < 16) {
		brightness+=4;
		setBrightness(3,-16+brightness);
	}
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
				dmaCopy(dsconcertBitmap, bgGetGfxPtr(bg), 256*256);
				dmaCopy(dsconcertPal, BG_PALETTE, 256*2);

                mmUnload(MOD_FLATOUTLIES);
                mmLoad(MOD_ADDICTI);
                mmStart(MOD_ADDICTI, MM_PLAY_LOOP);
                break;
            }
            case 1: {
				dmaCopy(FINALLYBitmap, bgGetGfxPtr(bg), 256*256);
				dmaCopy(FINALLYPal, BG_PALETTE, 256*2);

                mmUnload(MOD_ADDICTI);
                mmLoad(MOD_2ND_PM);
                mmStart(MOD_2ND_PM, MM_PLAY_LOOP);
                break;

            }
            case 2: {
				dmaCopy(DSDONUTBitmap, bgGetGfxPtr(bg), 256*256);
				dmaCopy(DSDONUTPal, BG_PALETTE, 256*2);

                mmUnload(MOD_2ND_PM);
                mmLoad(MOD_VRC6N001);
                mmStart(MOD_VRC6N001, MM_PLAY_LOOP);
                break;
            }
            case 3: {
				dmaCopy(fortniteballsBitmap, bgGetGfxPtr(bg), 256*256);
				dmaCopy(fortniteballsPal, BG_PALETTE, 256*2);
                mmUnload(MOD_VRC6N001);
                mmLoad(MOD_CANT_STOP_COMING);
                mmStart(MOD_CANT_STOP_COMING, MM_PLAY_LOOP);
                break;
            }
            case 4: {
				dmaCopy(thugbobBitmap, bgGetGfxPtr(bg), 256*256);
				dmaCopy(thugbobPal, BG_PALETTE, 256*2);
                mmUnload(MOD_CANT_STOP_COMING);
                mmLoad(MOD_FLATOUTLIES);
                mmStart(MOD_FLATOUTLIES, MM_PLAY_LOOP);
                break;
            }

        }
    }
    if(cube) {
        drawCube();
    }
    else {
        drawDonut();
    }

    //update our funny lil sprite guys
    double sinVal = sin(angle/20.0)*45.0;
    double cosVal = cos(angle/20.0)*45.0;

    oamRotateScale(&oamMain, 0, degreesToAngle(sinVal), intToFixed(1, 8), intToFixed(1, 8));
    oamRotateScale(&oamMain, 1, degreesToAngle(1-sinVal), intToFixed(1, 8), intToFixed(1, 8));
    oamRotateScale(&oamMain, 2, degreesToAngle(1-cosVal), intToFixed(1, 8), intToFixed(1, 8));
    oamRotateScale(&oamMain, 3, degreesToAngle(cosVal), intToFixed(1, 8), intToFixed(1, 8));
    
    oamSet(&oamMain, 0, 256-96, -32, 0, 0, SpriteSize_64x64, SpriteColorFormat_256Color, 
    spr.sprite_gfx_mem[spr.gfx_frame%5], 3, true, false, false, false, false);

    oamSet(&oamMain, 1, -32, -32, 0, 0, SpriteSize_64x64, SpriteColorFormat_256Color, 
    spr.sprite_gfx_mem[spr.gfx_frame%5], 2, true, false, false, false, false);
    
    oamSet(&oamMain, 2, 256-96, 192-96, 0, 0, SpriteSize_64x64, SpriteColorFormat_256Color, 
    spr.sprite_gfx_mem[spr.gfx_frame%5], 1, true, false, false, false, false);

    oamSet(&oamMain, 3, -32, 192-96, 0, 0, SpriteSize_64x64, SpriteColorFormat_256Color, 
    spr.sprite_gfx_mem[spr.gfx_frame%5], 0, true, false, false, false, false);
    
    oamUpdate(&oamMain);
    spr.gfx_frame++;
    angle += (2);
    frame++;
    return 0;
}
void Game::drawCube() {
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
				Graphics::bresenham(cb, x1, y1, x2, y2);
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
void Game::drawDonut() {

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
					R(5, 8, ci, si,underscore) // rotate i
				}
				R(9, 7, cj, sj,underscore) // rotate j
			}
			printf("\x1b[2J");
			printf("\x1b[0m");
			printf("L/R: COLOR SEL: SHAPE ST: MUSIC\n");
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
			R(5, 7, cA, sA,underscore);
			R(5, 8, cB, sB,underscore);
			printf("\x1b[23A");
}
void Game::rotateZ3D(float t)
{
	float c = cos(t), s = sin(t);
	for (float *xyz : nodes)
	{
		float x = xyz[0], y = xyz[1];
		xyz[0] = x * c - y * s;
		xyz[1] = y * c + x * s;
	}
}

void Game::rotateY3D(float t)
{
	float c = cos(t), s = sin(t);
	for (float *xyz : nodes)
	{
		float x = xyz[0], z = xyz[2];
		xyz[0] = x * c - z * s;
		xyz[2] = z * c + x * s;
	}
}

void Game::rotateX3D(float t)
{
	float c = cos(t), s = sin(t);
	for (float *xyz : nodes)
	{
		float y = xyz[1], z = xyz[2];
		xyz[1] = y * c - z * s;
		xyz[2] = z * c + y * s;
	}
}

float Game::rad(float deg)
{
	return deg * M_PI / 180;
}
void Game::clean() {
	        switch(song) {
            default: {
                mmUnload(MOD_ADDICTI);
                break;
            }
            case 1: {
                mmUnload(MOD_2ND_PM);
                break;

            }
            case 2: {
                mmUnload(MOD_VRC6N001);
                break;
            }
            case 3: {
                mmUnload(MOD_CANT_STOP_COMING);
                break;
            }
            case 4: {
                mmUnload(MOD_FLATOUTLIES);
                break;
            }

        }

}