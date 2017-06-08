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

	if(argc != 2)
	{
		errno = 5;
		log_err("Input file needs to be specified as command line argument");
		return 1;
	}
	
	

	//this needs to be checked better
	chip8* chip8 = create_chip8();

	int ret = c8_initialize(chip8);
	if(ret) return 1;

	//Test the stack
#ifdef DEBUG
	test_stack(chip8);
#endif

	char* fName = argv[1];

	ret = c8_load_program(chip8, fName);
	if(ret) return 1;

	for(;;)
	{
		c8_emulate_cycle(chip8);
	
	}

	c8_shutdown(chip8);
	//	setup_graphics();
	//	setup_input();

	//	initialize();
	//	load_game();
	//




	/*
	   for(;;)
	   {
	   c8_emulate_cycle(chip8);

	   if(c8_get_draw_flag(chip8)){
	   draw_graphics();
	   }

	   set_keys(chip8);
	   }
	   */
	return 0;
}
