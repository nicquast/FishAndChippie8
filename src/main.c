#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include "display/display.h"
#include "system/system.h"
#include "memory/memory.h"
#include "cpu/cpu.h"
#include "keypad/keypad.h"

#define SDL_FLAGS SDL_INIT_VIDEO

#define WINDOW_TITLE "Fish and Chippie 8"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 400

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

	// Initialise Chip8 System
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

	int timing_counter = 0;
	while (!quit) {
		constexpr int cycle_ms = 1;
		constexpr int timing_tick_ms = 16;
		// Handle SDL Events
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT)
				quit = true;
		}

		//Handle timing logic
		if (timing_counter > timing_tick_ms) {
			// Decrement timing registers down to 0
			chip8_system.register_store.delay_timer -= (chip8_system.register_store.delay_timer > 0 ? 1 : 0);
			chip8_system.register_store.sound_timer -= (chip8_system.register_store.sound_timer > 0 ? 1 : 0);
			timing_counter = 0;
		}
		timing_counter ++;

		//Get Keyboard Input
		updateKeypad(chip8_system.keypad);

		// Handle system instructions
		if (!instructionTick(&chip8_system, display_handle)) {
			quit = true;
			printf("undefined instruction encountered, exiting");
		}
		updateDisplay(display_handle);
		SDL_Delay(cycle_ms);
	}

	freeChip8SystemMemory(chip8_system);
	deleteDisplay(display_handle);

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);

	SDL_Quit();

	return EXIT_SUCCESS;
}