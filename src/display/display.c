#include "display.h"
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_pixels.h>
#include <stdlib.h>

#define WHITE 0xFFFFFFFF
#define BLACK 0xFF000000

DisplayHandle createDisplay(SDL_Renderer* renderer) {
    DisplayHandle display_handle = malloc(sizeof(Display));
    clearDisplay(display_handle);
    display_handle->sdl_renderer = renderer;
    display_handle->sdl_texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STATIC,
        DISPLAY_WIDTH,
        DISPLAY_HEIGHT);

    return display_handle;
}

void deleteDisplay(DisplayHandle display_handle) {
    SDL_DestroyRenderer(display_handle->sdl_renderer);
    SDL_DestroyTexture(display_handle->sdl_texture);
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

void setPixel(DisplayHandle display_handle, const int x, const int y, const bool state) {
    display_handle->pixelmap[x][y] = state;
}

bool updateDisplay(DisplayHandle display_handle) {
    u_int32_t* pixelBuffer = malloc(sizeof(u_int32_t) * DISPLAY_WIDTH * DISPLAY_HEIGHT);
    bool success = true;

    for (int y = 0; y < DISPLAY_HEIGHT; y++) {
        for (int x = 0; x < DISPLAY_WIDTH; x++) {
            pixelBuffer[x + y * DISPLAY_WIDTH] = display_handle->pixelmap[x][y]
                ? WHITE
                : BLACK;
        }
    }

    if (!SDL_UpdateTexture(display_handle->sdl_texture, NULL, pixelBuffer, DISPLAY_WIDTH))
        success = false;

    free(pixelBuffer);

    return success;
}