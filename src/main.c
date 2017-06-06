#include "../include/debug.h"
#include "../include/system.h"


/* 
 * System Memory Map
 *
 * 0x000-0x1FF - Chip 8 interpreter (contains font set in emu)
 * 0x050-0x0A0 - Used for the built in 4x5 pixel font set (0-F)
 * 0x200-0xFFF - Program ROM and work RAM
 * 
 */

int main(int argc, char** argv)
{

	//this needs to be checked better
	chip8* chip8 = create_chip8();
	
	int ret = c8_initialize(chip8);
	check(ret != 1, "Initialization failed");
	
//	setup_graphics();
//	setup_input();

//	initialize();
//	load_game();
//
	for(;;)
	{
		c8_emulate_cycle(chip8);

//		if(c8_get_draw_flag(chip8)){
//			draw_graphics();
//		}
//
//		set_keys(chip8);
	}
//
	return 0;
error:
	return 1;
}
