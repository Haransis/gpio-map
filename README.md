# gpio-map
This is a student project to implement a simple interface to activate/deactivate gpio pins.
It uses [wiringPi](http://wiringpi.com/) and [ncurses](https://invisible-island.net/ncurses/ncurses.html).

## Requirements
Cross-compiler for RPI, wiringPi and ncurses for RPI.

## Usage
The Makefile needs three environment variables for the compilation and cross-compilation.
```
TARGET_NPC = /path/to/ncurses/
TARGET_NPI = /path/to/ncurses-for-rpi/
RPI_COMPILER = /path/to/cross-compiler/
```

Then a simple `make` will do the trick. Executables are buit inside the build folder.

## Acknowledgments
Gordon (@Drogon) - https://projects.drogon.net  
Thomas Dickey - https://invisible-island.net/