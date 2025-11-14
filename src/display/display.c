#include "display.h"
#include <SDL3/SDL_render.h>
#include <stdlib.h>

DisplayHandle createDisplay() {	
    DisplayHandle display_handle = malloc(sizeof(Display));

    clearDisplay(display_handle);

    return display_handle;
}

void deleteDisplay(DisplayHandle display_handle) {
    SDL_DestroyRenderer(display_handle->sdl_renderer);
    free(display_handle);
    display_handle = nullptr;
}

// Takes the display handle and sets all pixels to 0
void clearDisplay(DisplayHandle display_handle) {
    for (int x = 0; x < DISPLAY_WIDTH; x++) {
	    for (int y = 0; y < DISPLAY_HEIGHT; y++) {
	        display_handle->pixelmap[x][y] = 0;
	    }
    }
}
