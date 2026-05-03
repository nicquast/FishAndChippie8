#include "display.h"

#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <stdlib.h>


#define PIXEL_FORMAT SDL_PIXELFORMAT_ARGB8888

DisplayHandle createDisplay(SDL_Renderer *renderer, unsigned int on_colour, unsigned int off_colour) {
  DisplayHandle display_handle = malloc(sizeof(Display));
  display_handle->on_colour = on_colour;
  display_handle->off_colour = off_colour;
  clearDisplay(display_handle);

  display_handle->sdl_renderer = renderer;
  display_handle->sdl_texture =
      SDL_CreateTexture(renderer, PIXEL_FORMAT, SDL_TEXTUREACCESS_STATIC,
                        DISPLAY_WIDTH, DISPLAY_HEIGHT);
  SDL_SetTextureScaleMode(display_handle->sdl_texture, SDL_SCALEMODE_NEAREST);

  return display_handle;
}

void deleteDisplay(DisplayHandle display_handle) {
  SDL_DestroyRenderer(display_handle->sdl_renderer);
  SDL_DestroyTexture(display_handle->sdl_texture);
  free(display_handle);
  display_handle = NULL;
}

// Takes the display handle and sets all pixels to 0
void clearDisplay(DisplayHandle display_handle) {
  for (int i = 0; i < DISPLAY_WIDTH * DISPLAY_HEIGHT; i++) {
    display_handle->pixel_buffer[i] = display_handle->off_colour;
  }
}

void setPixel(DisplayHandle display_handle, const int x, const int y,
              const bool state) {
  display_handle->pixel_buffer[x + y * DISPLAY_WIDTH] =
      state ? display_handle->on_colour : display_handle->off_colour;
}

int flipPixel(DisplayHandle display_handle, const int x, const int y) {
  if (display_handle->pixel_buffer[x + y * DISPLAY_WIDTH] == display_handle->on_colour) {
    display_handle->pixel_buffer[x + y * DISPLAY_WIDTH] = display_handle->off_colour;
    return 1;
  } else {
    display_handle->pixel_buffer[x + y * DISPLAY_WIDTH] = display_handle->on_colour;
    return 0;
  }
}

pixel_t *getPixelBuffer(DisplayHandle display_handle) {
  return display_handle->pixel_buffer;
}

bool updateDisplay(DisplayHandle display_handle) {
  int pitch = DISPLAY_WIDTH * SDL_BYTESPERPIXEL(PIXEL_FORMAT);
  if (!SDL_UpdateTexture(display_handle->sdl_texture, NULL,
                         display_handle->pixel_buffer, pitch))
    return false;

  if (!SDL_RenderClear(display_handle->sdl_renderer))
    return false;

  if (!SDL_RenderTexture(display_handle->sdl_renderer,
                         display_handle->sdl_texture, NULL, NULL))
    return false;

  if (!SDL_RenderPresent(display_handle->sdl_renderer))
    return false;

  return true;
}
