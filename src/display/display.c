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
    free(display_handle->pixel_buffer);
    free(display_handle);
    display_handle = nullptr;
}

// Takes the display handle and sets all pixels to 0
void clearDisplay(DisplayHandle display_handle) {
    memset(display_handle->pixel_buffer, 0, DISPLAY_WIDTH * DISPLAY_HEIGHT);
}

void setPixel(DisplayHandle display_handle, const int x, const int y, const bool state) {
    display_handle->pixel_buffer[x + y * DISPLAY_WIDTH] = state ? WHITE : BLACK;
}

pixel_t* getPixelBuffer(DisplayHandle display_handle) {
    return display_handle->pixel_buffer;
}

bool updateDisplay(DisplayHandle display_handle) {
    if (!SDL_UpdateTexture(display_handle->sdl_texture, NULL, display_handle->pixel_buffer, DISPLAY_WIDTH))
        return false;

    if (!SDL_RenderClear(display_handle->sdl_renderer))
        return false;

    if (!SDL_RenderTexture(display_handle->sdl_renderer, display_handle->sdl_texture,NULL,NULL))
        return false;

    if (!SDL_RenderPresent(display_handle->sdl_renderer))
        return false;

    return true;
}