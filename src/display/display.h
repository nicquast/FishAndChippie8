#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL3/SDL_render.h>
#include <stdbool.h>

#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32
#define DISPLAY_HZ 60

typedef struct {
    bool pixelmap[DISPLAY_WIDTH][DISPLAY_HEIGHT]; //TODO: change to be an RGBA888 pixel buffer
    SDL_Renderer *sdl_renderer;
    SDL_Texture *sdl_texture;
} Display;

typedef Display* DisplayHandle;

// Initialise a display and return a handler for it
DisplayHandle createDisplay(SDL_Renderer*);

// Clear the contents of a display
void clearDisplay(DisplayHandle);

// Delete the display and free its memory
void deleteDisplay(DisplayHandle);

// Set the specified pixel to the value provided
void setPixel(DisplayHandle display_handle, int x, int y, bool state);

// Update renderer attached to display to match current pixel grid
// Returns true if successful, false if an SDL_Error has occurred
bool updateDisplay(DisplayHandle);
#endif
