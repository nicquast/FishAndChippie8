//
// Created by nic on 16/11/25.
//

#include "system.h"

#include <stdlib.h>
#include <math.h>
#include "../audio/audio.h"

Chip8System initChip8System() {
    Chip8System system = {
        .memory = initMemory(),
        .register_store = initRegisterStore(),
        .stack = {nullptr},
        .keypad = malloc(sizeof(bool) * KEYPAD_ARRAY_SIZE)
    };
    system.register_store.program_counter = ROM_START_ADDRESS;
    memset(system.keypad, false, KEYPAD_ARRAY_SIZE * sizeof(bool));
	initSound();
    return system;
}

void freeChip8SystemMemory(Chip8System system) {
    free(system.memory);
    freeStack(&system.stack);
    free(system.keypad);
}

instruction_t fetchInstruction(Chip8System *system) {
    mem_addr_t pc = system->register_store.program_counter;
    byte_t byte1 = system->memory[pc];
    byte_t byte2 = system->memory[pc + 1];

    // Concatenate the two bytes making up an instruction
    instruction_t instruction = ((instruction_t)byte1 << 8) | byte2;

    system->register_store.program_counter += 2;

    return instruction;
}

// Fills audio buffer and plays beep sound if audio register is > 0
void audioTick(Chip8System *system) {
	updateSound();

	if (system->register_store.sound_timer > 0)
		toggleSound(true);
	else
		toggleSound(false);
}

// Gets the next instruction and executes it.
// returns false if an unknown instruction is encountered
bool instructionTick(Chip8System *system, DisplayHandle display_handle) {
	//Fetch next instruction (also increments PC)
	instruction_t c_inst = fetchInstruction(system);

	//Deconstruct instruction
	int x = getX(c_inst);
	int y = getY(c_inst);
	int n = getN(c_inst);
	byte_t nn = getNN(c_inst);
	mem_addr_t nnn = getNNN(c_inst);
	const byte_t vx = system->register_store.gp_registers[x];
	const byte_t vy = system->register_store.gp_registers[y];

	printf("Instruction: %x\n", c_inst);

	// Choose behaviour based on first nibble
	switch (getNibble1(c_inst)) {
	case 0x0:
		// Differentiate between 00E0 and 00EE and execute
		//00E0 Clear Screen
		if (n == 0x0)
			clearDisplay(display_handle);
		//00EE Return from subroutine / Pop off stack
		if (n == 0xE) {
			mem_addr_t return_address = stackPop(&system->stack);
			system->register_store.program_counter = return_address;
		}
		break;
	case 0x1:
		// Jump to NNN
		system->register_store.program_counter = nnn;
		break;
	case 0x2:
		// Call subroutine at NNN
		stackPush(&system->stack, system->register_store.program_counter);
		system->register_store.program_counter = nnn;
		break;
	case 0x3:
		// Skip conditionally - Skip if VX = NN
		if (vx == nn)
			system->register_store.program_counter += 2;
		break;
	case 0x4:
		//Skip conditionally - Skip if VX !- NN
		if (vx != nn)
			system->register_store.program_counter += 2;
		break;
	case 0x5:
		//Skip conditionally - Skip if VX = VY
		if (vx == vy)
			system->register_store.program_counter += 2;
		break;
	case 0x6:
		//Set VX to NN
		system->register_store.gp_registers[x] = nn;
		break;
	case 0x7:
		//Add NN to VX
		system->register_store.gp_registers[x] += nn;
		break;
	case 0x9:
		//Skip conditionally - Skip if VX != VY
		if (vx != vy)
			system->register_store.program_counter += 2;
		break;
	case 0x8:
		// Arithmatic - further decoding required
		int result;
		switch (n) {
		case 0x0:
			// Set VX to value of VY
			system->register_store.gp_registers[x] = vy;
			break;
		case 0x1:
			// Binary OR
			system->register_store.gp_registers[x] = vx | vy;
			break;
		case 0x2:
			// Binary AND
			system->register_store.gp_registers[x] = vx & vy;
			break;
		case 0x3:
			// Binary XOR
			system->register_store.gp_registers[x] = vx ^ vy;
			break;
		case 0x4:
			// Add VX to VY
			result = (int)vx + (int)vy;

			// Set flag register to 1 if it overflows and 0 if it doesn't
			if (result > 255)
				system->register_store.gp_registers[0xF] = 1;
			else
				system->register_store.gp_registers[0xF] = 0;

			// set VX to result
			system->register_store.gp_registers[x] = (byte_t) result;
			break;
		case 0x5:
			// VX - VY
			result = vx - vy;

			// Set VF to 0 if we underflow and 1 if we don't
			if (result < 0)
				system->register_store.gp_registers[0xF] = 0;
			else
				system->register_store.gp_registers[0xF] = 1;

			system->register_store.gp_registers[x] = (byte_t) result;
			break;
		case 0x7:
			// VY - VX
			result = vy - vx;

			// Set VF to 0 if we underflow and 1 if we don't
			if (result < 0)
				system->register_store.gp_registers[0xF] = 0;
			else
				system->register_store.gp_registers[0xF] = 1;

			system->register_store.gp_registers[x] = (byte_t) result;
			break;
		case 0x6:
			// Shift right
			const int right_bit = vx & 0b00000001;
			system->register_store.gp_registers[0xF] = right_bit;
			system->register_store.gp_registers[x] = (byte_t) (vx >> 1);
			break;
		case 0xE:
			// Shift left
			const int left_bit = vx >> 7;
			system->register_store.gp_registers[0xF] = left_bit;
			system->register_store.gp_registers[x] = (byte_t) (vx << 1);
			break;
		default:
			//Undefined instruction
			return false;
		}
		break;
	case 0xA:
		// Set index - set I to NNN
		system->register_store.index_register = nnn;
		break;
	case 0xB:
		// Jump with offset - jump to NNN + V0
		system->register_store.program_counter = nnn + system->register_store.gp_registers[0];
		break;
	case 0xC:
		// Random - random number & NN and places it in VX
		byte_t random_number = rand();
		system->register_store.gp_registers[x] = random_number & nn;
		break;
	case 0xD:
		// Display - DXYN - Draw an N pixel tall sprite from memory location at I index register
		// to the screen at X coord at VX and Y coord at VY, set pixels by flipping them. If we flip one from on to off
		// by doing this set VF to 1.

		// TODO: Need to make this more readable :(

		// Get coordinates
		int x_coord = vx % (DISPLAY_WIDTH - 1);
		int y_coord = vy % (DISPLAY_HEIGHT - 1);

		mem_addr_t sprite_address = system->register_store.index_register;

		// For each row
		int vf = 0;
		for (int i = 0; i < n; i++) {
			byte_t sprite_row = system->memory[sprite_address + i];
			// For each pixel in the sprite row
			for (int j = 7; j >= 0; j--) {
				//Skip to next row if we exceed the screen width
				if (x_coord + (7 - j) > DISPLAY_WIDTH)
					break;

				int bitmask = 0b1 << j;
				int pixel = (sprite_row & bitmask) >> j;

				// Skip if the pixel in the sprite is off
				if (pixel == 0)
					continue;

				// Flip the pixel on the display
				vf = flipPixel(display_handle, x_coord + (7 - j), y_coord) || vf;
			}
			y_coord++;
		}

		// Set VF to 1 if we flipped a pixel from on to off
		system->register_store.gp_registers[0xF] = vf;
		break;
	case 0xE:
		// Skip if key
		switch (nn) {
		case 0x9E:
			// Skip if key in VX pressed
			if (system->keypad[vx]) //TODO: protect against overflow
				system->register_store.program_counter += 2;
			break;
		case 0xA1:
			// Skip if key in VX is not pressed
			if (!system->keypad[vx]) //TODO: protect against overflow
				system->register_store.program_counter += 2;
			break;
		default:
			//Undefined instruction
			return false;
		}
		break;
	case 0xF:
		//Timers add to index, get key, font, binary-coded decimal conversion, store and load memory
		// Lots of stuff going on with this first nibble...
		switch (nn) {
		case 0x07:
			system->register_store.gp_registers[x] = system->register_store.delay_timer;
			break;
		case 0x15:
			system->register_store.delay_timer = system->register_store.gp_registers[x];
			break;
		case 0x18:
			system->register_store.sound_timer = system->register_store.gp_registers[x];
			break;
		case 0x1E:
			// Add VX to Index register
			// Implementing Amiga interpreter to retain compatability wit Spaceflight 2091!
			result = system->register_store.index_register + vx;
			system->register_store.index_register = result;
			if (result > 1000)
				system->register_store.gp_registers[0xF] = 1;
			break;
		case 0x0A:
			// Get Key
			// TODO: Only detect key pressed on key release
			bool key_pressed = false;
			for (int key = 0; key < KEYPAD_ARRAY_SIZE; key++) {
				if (system->keypad[key]) {
					key_pressed = true;
					system->register_store.gp_registers[x] = key;
				}
			}

			// Block if no key is pressed
			if (!key_pressed) {
				system->register_store.program_counter -= 2;
			}
			break;
		case 0x29:
			// Set Index register to the address of the hex digit in vx
			system->register_store.index_register = vx * FONT_CHARACTER_OFFSET;
			break;
		case 0x33:
			// Store binary decimal representation of VX to i, i+1, i+2
			const mem_addr_t index = system->register_store.index_register;
			for (int i = 0; i <= 2; i++) {
				const int divisor = i == 0 ? 1 : (int) pow(10, i);
				system->memory[index + 2 - i] = vx / divisor % 10;
			}
			break;
		case 0x55:
			// Store successive registers up to x into memory starting at the index register
			for (int i = 0; i <= x; i++) {
				const mem_addr_t save_address = system->register_store.index_register + i;
				system->memory[save_address] = system->register_store.gp_registers[i];
			}
			break;
		case 0x65:
			// Load memory into registers from the index register up to I + x
			// TODO: Add configurable option on whether or not to increment I
			// NOTE: Original COSMAC interpreter did increment, modern interpreters do not. default behaviour should be
			// the more modern approach
			for (int i = 0; i <= x; i++) {
				const mem_addr_t load_address = system->register_store.index_register + i;
				system->register_store.gp_registers[i] = system->memory[load_address];
			}

			break;
		default:
			// Undefined instruction
			return false;
		}
		break;
	default:
		// Undefined instruction
		return false;
	}
    return true;
}
