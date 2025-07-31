#include "display.h"
#include <stdlib.h>

DisplayHandle createDisplay() {
		// Malloc contiguous block of memory for our rows of "pixels"
		void* display_memory = malloc(sizeof(bool) * DISPLAY_WIDTH	* DISPLAY_HEIGHT);
		int offset = 0;
		

		bool** display_array = display_memory;
		for (int i; i < DISPLAY_WIDTH; i++) {
				display_array[i] = display_memory + offset;
				offset += sizeof(bool) * DISPLAY_HEIGHT;
		}
		
		DisplayHandle display_handle = malloc(sizeof(Display));
		
		display_handle->pixelmap = display_array;

		return display_handle;
}

void deleteDisplay(DisplayHandle display_handle) {
		//Free allocated memory
		for (int i; i < DISPLAY_WIDTH; i++) {
				free(display_handle->pixelmap[i]);
				display_handle->pixelmap[i] = NULL;
		}
		display_handle->pixelmap = NULL;
}
