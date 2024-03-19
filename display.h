#ifndef DISPLAY_H
#define DISPLAY_H
/*
 * This is a really shitty c file that probably
 * does not adhere to any good conventions...
 *
 * Basically it abstracts SDL into the absolute most basic
 * functionality needed to simulate the chip8 screen.
 *
 * If you actually know what you're doing and have the displeasure of 
 * reading through this code... I'm very sorry.
 */
#include <stdbool.h>

#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32

// Toggles the specified pixel on or off
void setPixel(int x, int y);

// Draws the current screen matrix to the screen
void drawScreen();

typedef bool display_t[DISPLAY_WIDTH][DISPLAY_HEIGHT];

// Initialises the SDL window
void initWindow();

// Destroys the window
void quitWindow();

#endif
