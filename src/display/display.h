#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdbool.h>

#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32
#define DISPLAY_HZ 60

typedef struct {
	bool** pixelmap;
} Display;

typedef Display* DisplayHandle;

// Initialise a display and return a handler for it
DisplayHandle createDisplay();

// Delete the display and free its memory
void freeDisplay(DisplayHandle);
#endif
