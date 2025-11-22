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

int main() {
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

	

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);

	SDL_Quit();

	return EXIT_SUCCESS;
}
