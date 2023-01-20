# LGFX_Simulator_SDL
PC simulator project for LovyanGFX on linux with SDL
## Usage

### Get the PC project

Clone the PC project and the related sub modules:

```
git clone --recursive https://github.com/Forairaaaaa/LGFX_Simulator_SDL.git
```

### Install SDL
You can download SDL from https://www.libsdl.org/

On on Linux you can install it via terminal:
```
sudo apt-get update && sudo apt-get install -y build-essential libsdl2-dev
```

## CMake

The following steps can be used with CMake on a Unix-like system. This may also work on other OSes but has not been tested.

1. Ensure CMake is installed, i.e. the `cmake` command works on the terminal.
2. Make a new directory. The name doesn't matter but `build` will be used for this tutorial.
3. Type `cd build`.
4. Type `cmake ..`. CMake will generate the appropriate build files.
5. Type `make ` 

6. The binary will be in `./Play`, and can be run by typing that command.

