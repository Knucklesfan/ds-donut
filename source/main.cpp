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
#include <stdio.h>

#include <filesystem.h>
#include <nds.h>
#include <maxmod9.h>

#include "soundbank.h"
#include "marioheadbang.h"
#include "dsconcert.h"
#include "game.h"
#include "intro.h"
#include "title.h"
#include "chrisscreen.h"

mm_word stream(mm_word length, mm_addr dest, mm_stream_formats format);
FILE* file;

void playWav() {
	//load audio file
	file = fopen("rap.wav", "rb");
	//create a stream
	//https://maxmod.devkitpro.org/ref/functions/mm_stream.html
	mm_stream mystream;
    mystream.sampling_rate = 44100;
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
volatile int frame = 0;
volatile int scanline = 0;
volatile int gamemode = 0;
void Vblank() {
	scanline = 0;
	frame++;
}
void Hblank() {
	if(gamemode == 2) {
		setBackdropColorSub(scanline/24);
		scanline++;

	}
}

mm_word myEventHandler(mm_word msg, mm_word param) {
	switch (msg)
	{
	
	case MMCB_SONGMESSAGE: // process song messages

		break;

	case MMCB_SONGFINISHED: // process song finish message (only triggered in songs played with MM_PLAY_ONCE)

		break;
	}

	return 0;
}
//---------------------------------------------------------------------------------
int main(void)
{
	if(nitroFSInit(NULL)) {

	defaultExceptionHandler();

    mmInitDefault("nitro:/soundbank.bin");
	mmSetEventHandler(myEventHandler);
	playWav();
	//---------------------------------------------------------------------------------
	irqSet(IRQ_VBLANK, Vblank); //Do some vblank, cuz we need to for hblank
	irqSet(IRQ_HBLANK, Hblank); //..and hblank for cool snes-like (HDMA ftw) hblank fun

	irqEnable(IRQ_HBLANK|IRQ_VBLANK);   
	
	Title::load();
	while (true)
	{	
		mmStreamUpdate();
		switch(gamemode) {
			case 0: {
				int logic = Title::logic();
				if(logic == 1) {
					Title::clean();
					ChrisScreen::load();
					gamemode = 1;
				};
				break;

			}
			case 1:{
				int logic = ChrisScreen::logic();
				if(logic == 1) {
					ChrisScreen::clean();
					Title::load();
					gamemode = 2;
				};
			}break;
			case 2:{
				int logic = Title::logic();
				if(logic == 1) {
					Title::clean();
					Game::load();
					gamemode = 3;
				};
			}break;

			case 3: {
				Game::logic();

			}break;
		}
		swiWaitForVBlank();

	}
	}
	return 0;

}
