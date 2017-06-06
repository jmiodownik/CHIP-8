#include "../include/system.h"
#include <stdlib.h>
#include "../include/debug.h"

struct chip8_t {
	//Willl store the 35 opcodes for the chip-8
	unsigned short opcode;

	//4KiB of memory
	unsigned char* memory;

	//Register array, the chip 8 has 15 named V0...V15
	unsigned char* V;

	//Index register (0x000 - 0xFFF)
	unsigned short I;

	//The program counter (0x000 - 0xFFF)
	unsigned short pc;

	//Graphics display - 2048 pixels
	unsigned char* graphics;

	//Timer registers
	unsigned char delay_timer;
	unsigned char sound_timer;

	//the stack on the chip-8 is 17 elements deep
	unsigned short* stack;

	//the stack pointer
	unsigned short* sp;

	//array containing input keys (0x0 - 0xF)
	unsigned char* key;
};

chip8* create_chip8()
{
	chip8* chip8 = calloc(1, sizeof(struct chip8_t));
	check_mem(chip8 !=NULL);

	return chip8;
	
error:
	return NULL;
}

int c8_initialize(chip8* chip8)
{
	debug("Initializing chip8 processor...");

	chip8->pc = 0x200;
	chip8->opcode = 0;
	chip8->I = 0;
	
	chip8->memory = calloc(4096, sizeof(unsigned char));
	check_mem(chip8->memory);

	chip8->V = calloc(16, sizeof(unsigned char));
	check_mem(chip8->V);

	chip8->stack =  calloc(16, sizeof(short));
	check_mem(chip8->stack);

	chip8->sp = chip8->stack;

	chip8->key = calloc(16, sizeof(unsigned char));
	check_mem(chip8->key);

	chip8->graphics = calloc(64*32, sizeof(unsigned char));	
	check_mem(chip8->graphics);


	//load_fontset
	//reset_timers
	return 0;

error:
	if(chip8->memory){
		free(chip8->memory);
	}
	if(chip8->V){
		free(chip8->V);
	}
	if(chip8->stack){
		free(chip8->stack);
	}
	if(chip8->key){
		free(chip8->key);
	}
	if(chip8->graphics){
		free(chip8->graphics);
	}

	if(chip8) {
		free(chip8);
	}
	return 1;
}

unsigned char* c8_get_memory(chip8* chip8)
{
	return chip8->memory;
}


void c8_emulate_cycle(chip8* chip8)
{
	//Fetch
	/*
	* opcodes are 2 bytes, so get the first portion of it from the program counter, 
	* bitshift right by 8 bits (1 byte), and concatinate the LSB of the opcode found at pc+1
	*/
	chip8->opcode = chip8->memory[chip8->pc] << 8 | chip8->memory[chip8->pc+1];

	//Decode
	
	//Execute
	
	//Update Timers
}
