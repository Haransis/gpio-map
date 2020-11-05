# gpio-map
This is a student project to implement a simple interface to activate/deactivate gpio pins.
It uses [wiringPi](http://wiringpi.com/) and [ncurses](https://invisible-island.net/ncurses/ncurses.html).

## Requirements
Cross-compiler for RPI, wiringPi and ncurses for RPI.

## Usage
The Makefile needs one environment variable for the compilation and cross-compilation.
```
RPI_COMPILER = /path/to/cross-compiler/
```
Then a `make libraries_RPI` followed by a `make` will do the trick. Executables are built inside the build folder. 
Librairies are compiled and installed in `libraries/name_of_the_library/built`.

If you do not want to compile the libraries, you can provide paths with the following environment variables
```
TARGET_NPC = /path/to/ncurses/
TARGET_NPI = /path/to/ncurses-for-rpi/
TARGET_WPI = /path/to/wiringPi/
```

## Library used  
This project used both wiringPi and ncurses libraries. The websites of their authors can be found in Acknowledgments.
Their source code is available in the `libraries` folder. 
The makefiles of wiringPi have been modified to allow automatic cross-compilation from the `RPI_COMPILER` environment 
variable. 

## Acknowledgments
Gordon (@Drogon) - https://projects.drogon.net  
Thomas Dickey - https://invisible-island.net/