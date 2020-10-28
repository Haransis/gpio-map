# gpio-map
This is a student project to implement a User Interface on a Raspberry Pi.
It uses

## Usage
The Makefile needs two environment variables for the compilation and cross-compilation.
```
TARGET_NPC = /path/to/ncurses/
TARGET_NPI = /path/to/ncurses-for-rpi/
```

Then a simple `make` will do the trick. Executables are buit inside the build folder.