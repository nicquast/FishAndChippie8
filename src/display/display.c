#include "display.h"
#include <stdlib.h>

DisplayHandle createDisplay() {
		// Allocate contiguous block of memory for our rows of "pixels"
		void* display_memory = malloc(sizeof(bool) * DISPLAY_WIDTH	* DISPLAY_HEIGHT + sizeof(bool*) * DISPLAY_WIDTH);
		int offset = 0;
		

		bool** display_array = display_memory;
		offset += sizeof(bool*) * DISPLAY_WIDTH;

		for (int i = 0; i < DISPLAY_WIDTH; i++) {
				display_array[i] = display_memory + offset;
				offset += sizeof(bool) * DISPLAY_HEIGHT;
		}
		
		DisplayHandle display_handle = malloc(sizeof(Display));
		
		display_handle->pixelmap = display_array;

		return display_handle;
}

void deleteDisplay(DisplayHandle display_handle) {
		//Free allocated memory
		for (int i = 0; i < DISPLAY_WIDTH; i++) {
				free(display_handle->pixelmap[i]);
				display_handle->pixelmap[i] = NULL;
		}
		free(display_handle->pixelmap);
		display_handle->pixelmap = NULL;

		free(display_handle);
		display_handle = NULL;
}

// Takes the display handle and sets all pixels to 0
void clearDisplay(DisplayHandle display_handle) {
		for (int x = 0; x < DISPLAY_WIDTH; x++) {
				for (int y = 0; y < DISPLAY_HEIGHT; y++) {
						display_handle->pixelmap[x][y] = 0;
				}
		}
}
