#include <nds.h>
#include <maxmod9.h>
#include <stdio.h>

#include "soundbank.h"
#include "soundbank_bin.h"
#include "zuodehao.h"

int main(void) {
        // set the mode for 2 text layers and two extended background layers

        videoSetMode(MODE_5_2D);
        vramSetBankA(VRAM_A_MAIN_BG_0x06000000);

	consoleDemoInit();
        int bg3 = bgInit(3, BgType_Bmp8, BgSize_B8_256x256, 0,0);

	dmaCopy(zuodehaoBitmap, bgGetGfxPtr(bg3), 256*256);
	dmaCopy(zuodehaoPal, BG_PALETTE, 256*2);

	mmInitDefaultMem((mm_addr)soundbank_bin);

	// load sound effects
	// unable to change variable name due to preset name
	// being loaded into memory
	mmLoadEffect( SFX_RED );

	mm_sound_effect red = {
		{ SFX_RED } ,			// id
		(int)(1.0f * (1<<10)),	// rate
		0,		// handle
		255,	// volume
		128,		// panning
	};

	// ansi escape sequence to clear screen and home cursor
	// /x1b[line;columnH
	iprintf("\x1b[2J");

	// ansi escape sequence to set print co-ordinates
	// /x1b[line;columnH
	iprintf("\x1b[0;0Htrolled lol");
	iprintf("\x1b[2;0HHold A for Red Sun in the Sky,");
        iprintf("\x1b[3;0Hor press X ONCE for 1 loop.");
        iprintf("\x1b[4;0HPress start and A,");
        iprintf("\x1b[5;0Hto play once without pressing.");
        iprintf("\x1b[6;0HPress start without A,");
        iprintf("\x1b[7;0Hto disable buttons.");
        iprintf("\x1b[9;0HEnjoy!");
	
	// sound effect handle (for cancelling it later)
	mm_sfxhand sound_eff = 0;


	do {

		int keys_pressed, keys_released;
		
		swiWaitForVBlank();
		scanKeys();

		keys_pressed = keysDown();
		keys_released = keysUp();

		// Play if A button is pressed
		if ( keys_pressed & KEY_A ) {
			sound_eff = mmEffectEx(&red);
		}

		// stop when A button is released
		if ( keys_released & KEY_A ) {
			mmEffectCancel(sound_eff);
		}
                
		// just play
                if ( keys_pressed & KEY_X ) {
			sound_eff = mmEffectEx(&red);
		}

		if( keys_pressed & KEY_START) break;

	} while(1);
	return 0;
}