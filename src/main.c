#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include "display/display.h"
#include "system/system.h"
#include "memory/memory.h"
#include "cpu/cpu.h"

#define SDL_FLAGS SDL_INIT_VIDEO

#define WINDOW_TITLE "Fish and Chippie 8"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 400


void instructionTick(Chip8System *system, DisplayHandle display_handle);

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

	SDL_SetRenderScale(renderer, (float)WINDOW_WIDTH/DISPLAY_WIDTH, (float)WINDOW_HEIGHT/DISPLAY_HEIGHT);

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

	bool quit = false;
	SDL_Event event;
	while (!quit) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT)
				quit = true;
		}

		instructionTick(&chip8_system, display_handle);
		updateDisplay(display_handle);
		SDL_Delay(1);
	}

	printDisplay(display_handle);

	free(chip8_system.memory);
	freeStack(&chip8_system.stack);
	deleteDisplay(display_handle);

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);

	SDL_Quit();

	return EXIT_SUCCESS;
}


void instructionTick(Chip8System *system, DisplayHandle display_handle) {
	//Fetch next instruction (also increments PC)
	instruction_t c_inst = fetchInstruction(system);

	//Deconstruct instruction
	int x = getX(c_inst);
	int y = getY(c_inst);
	int n = getN(c_inst);
	byte_t nn = getNN(c_inst);
	mem_addr_t nnn = getNNN(c_inst);

	printf("Instruction: %x\n", c_inst);

	// Choose behaviour based on first nibble
	switch (getNibble1(c_inst)) {
	case 0x0:
		// Differentiate between 00E0 and 00EE and execute
		//00E0 Clear Screen
		if (n == 0x0)
			clearDisplay(display_handle);
		//00EE Return from subroutine / Pop off stack
		break;
	case 0x1:
		// Jump to NNN
		system->register_store.program_counter = nnn;
		break;
	case 0x2:
		// Call subroutine at NNN
		stackPush(&system->stack, system->register_store.program_counter);
		system->register_store.program_counter = nnn;
		break;
	case 0x3:
		// Skip conditionally - Skip if VX = NN
		if (system->register_store.gp_registers[x] == nn)
			system->register_store.program_counter += 2;
		break;
	case 0x4:
		//Skip conditionally - Skip if VX !- NN
		if (system->register_store.gp_registers[x] != nn)
			system->register_store.program_counter += 2;
		break;
	case 0x5:
		//Skip conditionally - Skip if VX = VY
		if (system->register_store.gp_registers[x] == system->register_store.gp_registers[y])
			system->register_store.program_counter += 2;
		break;
	case 0x6:
		//Set VX to NN
		system->register_store.gp_registers[x] = nn;
		break;
	case 0x7:
		//Add NN to VX
		system->register_store.gp_registers[x] += nn;
		break;
	case 0x9:
		//Skip conditionally - Skip if VX != VY
		if (system->register_store.gp_registers[x] != system->register_store.gp_registers[y])
			system->register_store.program_counter += 2;
		break;
	case 0x8:
		// Arithmatic - further decoding required
		break;
	case 0xA:
		// Set index - set I to NNN
		system->register_store.index_register = nnn;
		break;
	case 0xB:
		// Jump with offset - jump to NNN + V0
		break;
	case 0xC:
		// Random - random number & NN and places it in VX
		break;
	case 0xD:
		// Display - DXYN - Draw an N pixel tall sprite from memory location at I index register
		// to the screen at X coord at VX and Y coord at VY, set pixels by flipping them. If we flip one from on to off
		// by doing this set VF to 1.

		// TODO: Need to make this more readable :(

		// Get coordinates
		int x_coord = system->register_store.gp_registers[x] % (DISPLAY_WIDTH - 1);
		int y_coord = system->register_store.gp_registers[y] % (DISPLAY_HEIGHT - 1);

		mem_addr_t sprite_address = system->register_store.index_register;

		// For each row
		int vf = 0;
		for (int i = 0; i < n; i++) {
			byte_t sprite_row = system->memory[sprite_address + i];
			// For each pixel in the sprite row
			for (int j = 7; j >= 0; j--) {
				//Skip to next row if we exceed the screen width
				if (x_coord + (7 - j) > DISPLAY_WIDTH)
					break;

				int bitmask = 0b1 << j;
				int pixel = (sprite_row & bitmask) >> j;

				// Skip if the pixel in the sprite is off
				if (pixel == 0)
					continue;

				// Flip the pixel on the display
				vf = flipPixel(display_handle, x_coord + (7 - j), y_coord) || vf;
			}
			y_coord++;
		}

		// Set VF to 1 if we flipped a pixel from on to off
		system->register_store.gp_registers[0xF] = vf;
		break;
	case 0xE:
		// Skip if key
		break;
	case 0xF:
		//Timers add to index, get key, font, binary-coded decimal conversion, store and load memory
		// Lots of stuff going on with this first nibble...
		break;
	default:
		//It shouldn't be possible to reach this
		break;
	}

}