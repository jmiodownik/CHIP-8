#include "../include/debug.h"
#include "../include/system.h"

#include "/usr/include/GL/glut.h"
/* 
 * System Memory Map
 *
 * 0x000-0x1FF - Chip 8 interpreter (contains font set in emu)
 * 0x050-0x0A0 - Used for the built in 4x5 pixel font set (0-F)
 * 0x200-0xFFF - Program ROM and work RAM
 * 
 */

chip8* mychip8 = NULL;
int modifier = 1;
void display();

int main(int argc, char** argv)
{

	if(argc != 2)
	{
		errno = 5;
		log_err("Input file needs to be specified as command line argument");
		return 1;
	}
	
	

	//this needs to be checked better
	mychip8 = create_chip8();

	int ret = c8_initialize(mychip8);
	if(ret) return 1;

	//Test the stack
#ifdef DEBUG
	test_stack(mychip8);
#endif

	char* fName = argv[1];

	ret = c8_load_program(mychip8, fName);
	if(ret) return 1;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	
	glutInitWindowSize(64,32);
	glutInitWindowPosition(320,320);
	glutCreateWindow("Snoogie's Chip8 Emulator");

	glutDisplayFunc(display);
	glutIdleFunc(display);

	c8_shutdown(mychip8);
	
}
void drawPixel(int x, int y)
{
	glBegin(GL_QUADS);
		glVertex3f((x * modifier) + 0.0f,     (y * modifier) + 0.0f,	 0.0f);
		glVertex3f((x * modifier) + 0.0f,     (y * modifier) + modifier, 0.0f);
		glVertex3f((x * modifier) + modifier, (y * modifier) + modifier, 0.0f);
		glVertex3f((x * modifier) + modifier, (y * modifier) + 0.0f,	 0.0f);
	glEnd();
}

void updateQuads(const chip8* c8)
{
	// Draw
	for(int y = 0; y < 32; ++y)		
		for(int x = 0; x < 64; ++x)
		{
			if(mychip8->graphics[(y*64) + x] == 0) 
				glColor3f(0.0f,0.0f,0.0f);			
			else 
				glColor3f(1.0f,1.0f,1.0f);

			drawPixel(x, y);
		}
}

void display()
{

	c8_emulate_cycle(mychip8);
	
	if(mychip8->draw_flag)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		updateQuads(mychip8);
		glutSwapBuffers();

		mychip8->draw_flag= 0;
	}	
}


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
