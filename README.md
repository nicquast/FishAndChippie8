# FishAndChippie8

## About
Fish and Chippie 8, as the name implies, is a Chip8 emulator. It's not perfect, but it passes the majority of tests. This implements the standard Chip8 opcodes.
I just wrote this for fun and to give emulation development a try as a result it's not particulary performant or feature rich but it does pass the majority of tests.
Input handling is however not as standard, FishAndChippie8 detects inputs on press rather than once released; however, as far as I've tested this hasn't caused any issues.

## Build
The project can be built with CMake. I've only tested this on x86_64 under Linux so I can't guarantee it will work on other platforms.
This project has SDL3 as a dependency.

```console
cmake . && cmake --build
```

## Usage
```console
FishAndChippie8 [PATH_TO_ROM]
```
