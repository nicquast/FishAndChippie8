#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "display/display.h"
#include "system/system.h"
#include "memory/memory.h"
#include "cpu/cpu.h"

#define SDL_FLAGS SDL_INIT_VIDEO

#define WINDOW_TITLE "Fish and Chippie 8"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 400

void instructionTick(Chip8System *system);

int main(int argc, char **argv) {

	if (argc < 2) {
		printf("Usage: %s [ROM file]\n", argv[0]);
		return EXIT_FAILURE;
	}
	// Initialise SDL video components
	if (!SDL_Init(SDL_FLAGS)) {
		fprintf(stderr, "Error initialising SDL3: %s\n", SDL_GetError());
		SDL_Quit();
		return EXIT_FAILURE;
	}
	
	SDL_Window *window = SDL_CreateWindow(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	if (!window) {
		fprintf(stderr, "Error initialising SDL3 Window: %s\n", SDL_GetError());
		SDL_Quit();
		return EXIT_FAILURE;
	}
	
	SDL_Renderer *renderer = SDL_CreateRenderer(window, nullptr);
	if (!renderer) {
		fprintf(stderr, "Error initialising SDL3 Renderer: %s\n", SDL_GetError());
		SDL_Quit();
		return EXIT_FAILURE;
	}

	DisplayHandle display_handle = createDisplay(renderer);
	Chip8System chip8_system = initChip8System();

	printf("Chip8 system initialised\n");

	//Load ROM
	FILE *rom_fp = fopen(argv[1], "rb");
	if (!rom_fp) {
		fprintf(stderr, "Error opening ROM file: %s\n", argv[1]);
		SDL_Quit();
		return EXIT_FAILURE;
	}
	loadRom(rom_fp, chip8_system.memory);

	printf("Loaded ROM\n");

	instructionTick(&chip8_system);

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);

	SDL_Quit();

	return EXIT_SUCCESS;
}


void instructionTick(Chip8System *system) {
	//Fetch next instruction (also increments PC)
	instruction_t c_inst = fetchInstruction(system);

	//Deconstruct instruction
	int x = getX(c_inst);
	int y = getY(c_inst);
	byte_t nn = getNN(c_inst);
	mem_addr_t nnn = getNNN(c_inst);

	printf("Instruction: %x\n", c_inst);

	// Choose behaviour based on first nibble
	switch (getNibble1(c_inst)) {
	case 0x0:
		// Differentiate between 00E0 and 00EE and execute
		break;
	case 0x1:
		// Jump
		break;
	case 0x2:
		// Call subroutine
		break;
	case 0x3:
		// Skip conditionally
		break;
	case 0x4:
		//Skip conditionally
		break;
	case 0x5:
		//Skip conditionally
		break;
	case 0x9:
		//Skip conditionally
		break;
	default:
		break;
	}

}