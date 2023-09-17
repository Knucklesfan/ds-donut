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
volatile int frame = 0;

void Vblank() {

	frame++;
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
	defaultExceptionHandler();
	//---------------------------------------------------------------------------------
	irqSet(IRQ_VBLANK, Vblank);
	mmInitDefaultMem((mm_addr)soundbank_bin);
	mmSetEventHandler(myEventHandler);

	Intro::load();
	int gamemode = 0;
	while (true)
	{	
		switch(gamemode) {
			case 0: {
				int logic = Intro::logic();
				if(logic == 1) {
					Intro::clean();
					Game::load();
					gamemode = 1;
				};
				break;

			}
			case 1: {
				Game::logic();
				break;
			}
		}
		swiWaitForVBlank();

	}
	return 0;
}
