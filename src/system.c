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
	//reset timers
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

	unsigned short opcode = chip8->opcode; //for ease of reference
	//Decode
	
	/*
	 * Opcode Table
	 *   00E0 - CLS   		clear the screen
         *   00EE - RET			return from subroutine
         *   0nnn - SYS addr		jump to machine code subroutine at nnn
         *   1nnn - JP addr		jump to location nnn
         *   2nnn - CALL addr		call subroutine at nnn
         *   3xkk - SE Vx, byte		skip next instruction if Vx == kk
         *   4xkk - SNE Vx, byte	skip next instruction if Vx != kk
         *   5xy0 - SE Vx, Vy		skip the next instruction if Vx == Vy
         *   6xkk - LD Vx, byte		Set Vx = kk
         *   7xkk - ADD Vx, byte	Set Vx = Vx + kk
         *   8xy0 - LD Vx, Vy		Set Vx = Vy
         *   8xy1 - OR Vx, Vy		Set Vx = Vx or Vy
         *   8xy2 - AND Vx, Vy		Set Vx = Vx and Vy
         *   8xy3 - XOR Vx, Vy		Set Vx = Vx xor Vy
         *   8xy4 - ADD Vx, Vy		Set Vx = Vx + Vy
         *   8xy5 - SUB Vx, Vy		Set Vx = Vx - Vy, set Vf = carry if needed
         *   8xy6 - SHR Vx {, Vy}	Set Vx = Vx shift right 1
         *   8xy7 - SUBN Vx, Vy		Set Vx = Vy - Vx, set Vf = NOT borrow
         *   8xyE - SHL Vx {, Vy}	Set Vx = Vx shift left 1
         *   9xy0 - SNE Vx, Vy		Skip the next instruction if Vx != Vy
         *   Annn - LD I, addr		Set I = nnn
         *   Bnnn - JP V0, addr		Jump to location V0 + nnn
         *   Cxkk - RND Vx, byte	Set Vx = a random byte + kk  (random = 0 - 255)
         *   Dxyn - DRW Vx, Vy, nibble	Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
         *   Ex9E - SKP Vx		Skip next instruction if key with the value of Vx is pressed
         *   ExA1 - SKNP Vx		Skip next instruciton if key with a value NOT in Vx in pressed (key up)
         *   Fx07 - LD Vx, DT		Set Vx = delay timer value
         *   Fx0A - LD Vx, K		wait for key press, store the value in Vx (no timeout)
         *   Fx15 - LD DT, Vx		Set Delay timer = Vx
         *   Fx18 - LD ST, Vx		Set sound timer = Vx
         *   Fx1E - ADD I, Vx		Set I = I + Vx
         *   Fx29 - LD F, Vx		Set I = location of sprite for digit Vx.
         *   Fx33 - LD B, Vx		Store BCD representation of Vx in memory locations I, I+1, and I+2.
         *   Fx55 - LD [I], Vx		Store registers V0 through Vx in memory starting at location I.
         *   Fx65 - LD Vx, [I]		Read registers V0 through Vx from memory starting at location I.
	 */

	//Begin by reading the most significant four bits
	switch(opcode & 0xF000){
		case 0x0000:
			switch(opcode & 0x000F){
				case 0x0000:
					break;
				case 0x000E:
					break;

			}

		case 0x2000:
		      //save off the address of the pc because we're about to jump to a new instruction
		      chip8->stack[*(chip8->sp)] = chip8->pc;
		      (chip8->sp)++;

		      chip8->pc = opcode & 0x0FFF;
		      break;
		case 0xA000:
	      		chip8->I = opcode & 0x0FFF;
			chip8->pc +=2;
	  		break;		
		default:
			log_err("Invalid opcode found: 0x%X", opcode);
			break;

	}
	//Execute
	
	//Update Timers
}
