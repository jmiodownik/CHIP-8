#ifndef SYSTEM_H 
#define SYSTEM_H

typedef struct chip8_t chip8;

chip8* create_chip8(void);
int c8_initialize(chip8*);
void c8_emulate_cycle(chip8*);
int c8_load_game(chip8*);


//Chip-8 getters

//this return value may need to be fixed
unsigned char* c8_get_memory(chip8*);
unsigned short* getPC(chip8*);
void test_stack(chip8*);
#endif
