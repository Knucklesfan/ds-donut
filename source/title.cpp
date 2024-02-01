#include "title.h"
#include <stdio.h>
#include <nds.h>
#include <maxmod9.h>
#include <math.h>
#include "star.h"
#include "starcrawl.h"
#include "finalcrawl.h"
#include "donut.h"
#include "starbgmain.h"
#include "pressstart.h"
// #include "chris.h"
#include "soundbank.h"
#define DISTANCE 300
#define SPEED 6

PrintConsole Title::bottomScreen;
int Title::angle = 0;
int Title::brightness = 0;
bool Title::active = true;
bool Title::explode = false;
bool Title::godown = false;
bool Title::goup = true;
int Title::lifetime = 0;
int scrollY = 0;
int Title::bg3 = 0;
u16* Title::starValue = 0;
u16* pressValue = 0;
double Title::scale = 1.0;
float starX[127];
float starY[127];
float starZ[127];
int Title::firstTex = 0;
int Title::secondTex = 0;
bool donut = false;
mm_word stream(mm_word length, mm_addr dest, mm_stream_formats format);
FILE* file;

void drawTri(int poly) {	
	u32 f1 = donut_faces[poly * 3] ;
	u32 f2 = donut_faces[poly * 3 + 1] ;
	u32 f3 = donut_faces[poly * 3 + 2] ;
	
	glColor3b(255,0,0);
	glVertex3v16(donut_vert[f1*3], donut_vert[f1*3 + 1], donut_vert[f1*3 +  2] );
	glColor3b(0,255,0);
	glVertex3v16(donut_vert[f2*3], donut_vert[f2*3 + 1], donut_vert[f2*3 + 2] );
	glColor3b(0,0,255);
	glVertex3v16(donut_vert[f3*3], donut_vert[f3*3 + 1], donut_vert[f3*3 + 2] );

}
void playWav() {
	//load audio file
	file = fopen("starwars.wav", "rb");
	//create a stream
	//https://maxmod.devkitpro.org/ref/functions/mm_stream.html
	mm_stream mystream;
    mystream.sampling_rate = 16000;
    mystream.buffer_length = 16000;
    mystream.callback = stream;
    mystream.format = MM_STREAM_16BIT_STEREO;
    mystream.timer = MM_TIMER2;
    mystream.manual = 1;
    mmStreamOpen(&mystream);
}
//this fills mm_stream mystream buffer
//https://maxmod.devkitpro.org/ref/functions/mm_stream_func.html
mm_word stream(mm_word length, mm_addr dest, mm_stream_formats format) {
	if (file) {
		int res = fread(dest, 4, length, file);
		if (res != length) {
			mmStreamClose();
			fclose(file);
			length = res - (res % 4);
		} else {
			length = res;
		}
	}
	return length;
}
void Title::clean() {
	mmStreamClose();
}
void Title::load() {
	playWav();

    memset(starX, 0, sizeof(starX));
    memset(starY, 0, sizeof(starY)); //clear out the coordinates for the stars
    memset(starZ, 0, sizeof(starZ));
    for(int i = 0; i < 127; i++) {
        starX[i] = (rand() % 256) - 127;
        starY[i] = (rand() % 192) - 96;
        starZ[i] = (rand() % 1700) - 100;
    }
    videoSetMode(MODE_0_3D);
	videoSetModeSub(MODE_0_2D);

    vramSetBankA(VRAM_A_TEXTURE);
	vramSetBankB(VRAM_B_TEXTURE);
	vramSetBankC(VRAM_C_MAIN_BG_0x06000000);
	vramSetBankD(VRAM_D_SUB_SPRITE);
 	oamInit(&oamSub, SpriteMapping_1D_128, false);
	
  	// bgExtPaletteEnable();
	//initialize the memory for our two backgrounds
	bg3 = bgInit(1, BgType_Text4bpp, BgSize_T_256x256, 16, 0);
	dmaCopy(starbgmainTiles,  bgGetGfxPtr(bg3), starbgmainTilesLen);
	dmaCopy(starbgmainMap, bgGetMapPtr(bg3), starbgmainMapLen);
	dmaCopy(starbgmainPal, BG_PALETTE, 256);

	consoleInit(&bottomScreen, 3, BgType_Text4bpp, BgSize_T_256x256, 31, 0, false, true);
	consoleSelect(&bottomScreen);
	printf("          PRESS START");
	// dmaCopy(chrisBitmap, bgGetGfxPtr(bg3), 256*256);
	// dmaCopy(chrisPal, BG_PALETTE, 256*2);

	u8* gfx = (u8*)starTiles; 
	starValue = oamAllocateGfx(&oamSub, SpriteSize_8x8, SpriteColorFormat_16Color);
	dmaCopy(gfx, starValue, 8*8);
	dmaCopy(starPal, SPRITE_PALETTE_SUB, 32);
	gfx = (u8*)pressstartTiles; 
	pressValue = oamAllocateGfx(&oamSub, SpriteSize_64x32, SpriteColorFormat_16Color);
	dmaCopy(gfx, pressValue, 64*32);
	dmaCopy(pressstartPal, SPRITE_PALETTE_SUB+32, 32);

    glInit();

    glEnable(GL_TEXTURE_2D);

    // The background must be fully opaque and have a unique polygon ID
    // (different from the polygons that are going to be drawn) so that
    // antialias works.
    glClearColor(0, 0, 0, 0);

    glClearDepth(0x7FFF);

    glViewport(0, 0, 255, 191);

    // Setup some VRAM as memory for textures

    // Load texture
    glGenTextures(1, &firstTex);
	glGenTextures(1, &secondTex); //should just use an array, but this is enough

    glBindTexture(0, firstTex);
    glTexImage2D(0, 0, GL_RGBA, TEXTURE_SIZE_256 , TEXTURE_SIZE_256, 0,
                 TEXGEN_TEXCOORD, (u8*)starcrawlBitmap);
    glBindTexture(0, secondTex);
    glTexImage2D(0, 0, GL_RGBA, TEXTURE_SIZE_256 , TEXTURE_SIZE_256, 0,
                 TEXGEN_TEXCOORD, (u8*)finalcrawlBitmap);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // gluPerspective(70, 256.0 / 192.0, 0.1, 40);

    // gluLookAt(0.0, 0.0, 2.0,  // Position
    //           0.0, 0.0, 0.0,  // Look at
    //           0.0, 1.0, 0.0); // Up
	gluPerspective(70, 256.0 / 192.0, 0.1, 40);
	
	gluLookAt(	0.0, 0.0, 2.0,		//camera possition 
				0.0, 0.0, 0.0,		//look at
				0.0, 1.0, 0.0);		//up
  	// consoleInit(&bottomScreen, 3, BgType_Text4bpp, BgSize_T_256x256, 31, 0, false, true);
	
	// consoleInit(&topScreen, 3, BgType_Text4bpp, BgSize_T_256x256, 30, 31, true, true);

    // consoleSelect(&topScreen);
	// iprintf("\x1b[2J");
	// iprintf("\x1b[0m");

    // iprintf("DSDonut!");
	// consoleSelect(&bottomScreen);
	// iprintf("Press start...");



}
int Title::logic() {

	//ALL OF THIS LOGIC IS STOLEN FROM KNUXFANPONG/KNUXFANTETRIMINOS
	//because im lazy

        //std::cout << lifetime << "\n";
    scanKeys();
    u16 keys = keysDown();
    if (keys & KEY_START) {
		lifetime = 0;
		explode = true;
	}




	//BEGIN RENDERING!!!
    // oamRotateScale(&oamMain, 0, degreesToAngle(knRot), intToFixed(1, 8), intToFixed(1, 8));
	// bgSetScroll(bg[1], 0, 0);

	// bgSetScroll(bgS, angle, angle);

	// bgSetScroll(checkerboard,-angle,-angle);
	// bgSetScroll(bg3,-(4.0-4.0/scale*4)-10,-(4-4.0/scale*4)+(float)lifetime/5.0);
	
	// bgSetScale(bg3,((int)((scale)*(1<<8))),((int)((scale)*(1<<8))));
	// bgUpdate();
	lifetime++;
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

	//implementation reference https://samme.github.io/phaser-examples-mirror/demoscene/starfield.html
	for(int i = 0; i < 127; i++) {
    float perspective = 300.0 / (300.0 - starZ[i]);
    int x = 256/2 + starX[i] * perspective;
    int y = 192/2 + starY[i] * perspective;

    starZ[i] += SPEED;
    if (starZ[i] > 192/2) {
        starZ[i] -= 600;
    }
    
	oamSet(&oamSub, i+1, x, y, 0, 0, SpriteSize_8x8, SpriteColorFormat_16Color, 
    starValue, -1, false, false, false, false, false);
	}
	oamSetPalette(&oamSub,0,1);
	oamSet(&oamSub, 0, 96, 80, 0, 0, SpriteSize_64x32, SpriteColorFormat_16Color, 
    pressValue, -1, false, false, false, false, false);

	oamUpdate(&oamSub);
	glPushMatrix();
	if(!donut) {
		
		glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE);

		glScalef(5,20,1);
		glRotateZ(0);
		glRotateX(-85);
		if(explode) {
			glTranslatef32(0,scrollY,0-lifetime*6);
			if(lifetime > 200) {
				donut = true;
				explode = false;
				lifetime = 0;
			}
		}
		else {
			scrollY = -6015+lifetime;
			glTranslatef32(0,scrollY,0);
			if(scrollY == 0) {
				lifetime = 0;
				explode = true;
			}
		}
		glPolyFmt(POLY_ALPHA(1) | POLY_CULL_NONE | POLY_ID(2));		glMatrixMode(GL_MODELVIEW);

		glBindTexture(0, firstTex);

		glColor3f(1, 1, 1);

		glBegin(GL_QUADS);

			GFX_TEX_COORD = (TEXTURE_PACK(0, inttot16(256)));
			glVertex3v16(floattov16(-0.5), floattov16(0), 0);

			GFX_TEX_COORD = (TEXTURE_PACK(inttot16(256),inttot16(256)));
			glVertex3v16(floattov16(0.5), floattov16(0), 0);

			GFX_TEX_COORD = (TEXTURE_PACK(inttot16(256), 0));
			glVertex3v16(floattov16(0.5), floattov16(1), 0);

			GFX_TEX_COORD = (TEXTURE_PACK(0,0));
			glVertex3v16(floattov16(-0.5), floattov16(1), 0);

		glEnd();
		
		glBindTexture(0, secondTex);

		glColor3f(1, 1, 1);

		glBegin(GL_QUADS);

			GFX_TEX_COORD = (TEXTURE_PACK(0, inttot16(256)));
			glVertex3v16(floattov16(-0.5), floattov16(-1), 0);

			GFX_TEX_COORD = (TEXTURE_PACK(inttot16(256),inttot16(256)));
			glVertex3v16(floattov16(0.5), floattov16(-1), 0);

			GFX_TEX_COORD = (TEXTURE_PACK(inttot16(256), 0));
			glVertex3v16(floattov16(0.5), floattov16(0), 0);

			GFX_TEX_COORD = (TEXTURE_PACK(0,0));
			glVertex3v16(floattov16(-0.5), floattov16(0), 0);

			glEnd();
	}
	else {
	
		//move it away from the camera
		// glTranslatef32(0, floattof32((float)lifetime/100), floattof32(-3));
		if(5-lifetime/50 > 0) {
			glTranslatef32(0,floattof32(5-(float)lifetime/50.0),floattof32(-3));
		}
		else {
			glTranslatef32(0,0,floattof32(-3));
		}
		glRotateY(lifetime/2);
		glRotateX(-lifetime/2);

		glBindTexture(0, -1);

		glSetOutlineColor(3,0);
		glPolyFmt(POLY_ALPHA(0) | POLY_CULL_NONE | POLY_ID(1));		glMatrixMode(GL_MODELVIEW);

		glBegin(GL_TRIANGLE);

			for(int i = 0; i < 74; i++) {
				drawTri(i);
			}
			
		glEnd();
	}
	glPopMatrix(1);
			
	glFlush(0);


	//not a real gl function and will likely change

	setBrightness(3,-16+16*(brightness/100.0));
	mmStreamUpdate();

	lifetime++;
	angle++;
	return 0;
}
