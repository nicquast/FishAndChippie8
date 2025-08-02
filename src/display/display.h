#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL3/SDL_render.h>
#include <stdbool.h>

#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32
#define DISPLAY_HZ 60

typedef struct {
    bool pixelmap[DISPLAY_WIDTH][DISPLAY_HEIGHT]; 
    SDL_Renderer *sdl_renderer;
} Display;

typedef Display* DisplayHandle;

// Initialise a display and return a handler for it
DisplayHandle createDisplay();

// Clear the contents of a display
void clearDisplay(DisplayHandle);

// Delete the display and free its memory
void deleteDisplay(DisplayHandle);
#endif
