#include "display.h"
#include <SDL2/SDL.h>

#define WINDOW_SIZE 10

// Global variables :(
// I'm very sorry
SDL_Window *window;
SDL_Surface *window_surface;

void initWindow() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("Failed to initialise the SDL2 library\n");
		exit(-1);
	}

	window = SDL_CreateWindow("SDL2 Window",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			DISPLAY_WIDTH * WINDOW_SIZE, DISPLAY_HEIGHT * WINDOW_SIZE,0);

	if (!window) {
		printf("failed to create window\n");
		exit(-1);
	}

	window_surface = SDL_GetWindowSurface(window);

	if (!window_surface) {
		printf("Failed to get surface from window");
		exit(-1);
	}
}

void drawScreen() {
	SDL_UpdateWindowSurface(window);
}

void quitWindow() {
	SDL_DestroyWindow(window);
}
