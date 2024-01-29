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
#include "marioheadbang.h"
#include "dsconcert.h"
#include "game.h"
#include "intro.h"
#include "chrisscreen.h"

volatile int frame = 0;
volatile int scanline = 0;

void Vblank() {
	scanline = 0;
	frame++;
	mmFrame();
}
void Hblank() {
	setBackdropColorSub(scanline/24);
	scanline++;
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
	irqInit(); //init the irq
	defaultExceptionHandler();

	mmInitDefaultMem((mm_addr)soundbank_bin);
	mmSetEventHandler(myEventHandler);

	//---------------------------------------------------------------------------------
	
	irqSet(IRQ_HBLANK, Hblank); //..and hblank for cool snes-like hblank fun

	irqEnable(IRQ_ALL);   

	Intro::load();
	int gamemode = 0;
	while (true)
	{	
		switch(gamemode) {
			case 0: {
				int logic = Intro::logic();
				if(logic == 1) {
					Intro::clean();
					ChrisScreen::load();
					gamemode = 1;
				};
				break;

			}
			case 1:{
				int logic = ChrisScreen::logic();
				if(logic == 1) {
					ChrisScreen::clean();
					Game::load();
					gamemode = 2;
				};
			}break;
			case 2: {
				Game::logic();

			}break;
		}
		swiWaitForVBlank();

	}
	return 0;
}
