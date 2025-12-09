#include "display.h"

#include <stdio.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_pixels.h>
#include <stdlib.h>

#define ON_COLOUR 0xFFD7DEDC
#define OFF_COLOUR 0xFF4B475C

#define PIXEL_FORMAT SDL_PIXELFORMAT_ARGB8888

DisplayHandle createDisplay(SDL_Renderer* renderer) {
    DisplayHandle display_handle = malloc(sizeof(Display));
    clearDisplay(display_handle);
    display_handle->sdl_renderer = renderer;
    display_handle->sdl_texture = SDL_CreateTexture(
        renderer,
        PIXEL_FORMAT,
        SDL_TEXTUREACCESS_STATIC,
        DISPLAY_WIDTH,
        DISPLAY_HEIGHT);
    SDL_SetTextureScaleMode(display_handle->sdl_texture,SDL_SCALEMODE_NEAREST);
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
    for (int i = 0; i < DISPLAY_WIDTH * DISPLAY_HEIGHT; i++) {
        display_handle->pixel_buffer[i] = OFF_COLOUR;
    }
}

void setPixel(DisplayHandle display_handle, const int x, const int y, const bool state) {
    display_handle->pixel_buffer[x + y * DISPLAY_WIDTH] = state ? ON_COLOUR : OFF_COLOUR;
}

int flipPixel(DisplayHandle display_handle, const int x, const int y) {
    switch (display_handle->pixel_buffer[x + y * DISPLAY_WIDTH]) {
    case ON_COLOUR:
        display_handle->pixel_buffer[x + y * DISPLAY_WIDTH] = OFF_COLOUR;
        return 1;
    case OFF_COLOUR:
        display_handle->pixel_buffer[x + y * DISPLAY_WIDTH] = ON_COLOUR;
        return 0;
    default:
        return -1;
    }
}


pixel_t* getPixelBuffer(DisplayHandle display_handle) {
    return display_handle->pixel_buffer;
}

bool updateDisplay(DisplayHandle display_handle) {
    int pitch = DISPLAY_WIDTH * SDL_BYTESPERPIXEL(PIXEL_FORMAT);
    if (!SDL_UpdateTexture(display_handle->sdl_texture, NULL, display_handle->pixel_buffer, pitch))
        return false;

    if (!SDL_RenderClear(display_handle->sdl_renderer))
        return false;

    if (!SDL_RenderTexture(display_handle->sdl_renderer, display_handle->sdl_texture,nullptr,nullptr))
        return false;

    if (!SDL_RenderPresent(display_handle->sdl_renderer))
        return false;

    return true;
}