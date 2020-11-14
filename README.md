# gpio-map
This is a student project to implement a simple interface to activate/deactivate gpio pins.
It uses [wiringPi](http://wiringpi.com/) and [ncurses](https://invisible-island.net/ncurses/ncurses.html).

## Libraries Requirement
Gpio-map relies on both wiringPi and ncurses libraries. The websites of their authors can be found in Acknowledgments.
Their source code is available in the `lib` folder.  Those two libraries have to be available, so that the cross-compiled 
gpio-map can use the headers and be linked with the cross-compiled version.

If you do not have those libraries, we provide a makefile for the cross-compilation.
If the environment variable `RPI_COMPILER = /path/to/cross-compiler/` 
points to the cross-compiler and  `arm-linux-gnueabihf-gcc` is available in the path, then a `make libraries_RPI` will 
compile both libraries for raspberryPi.
 
If you do not want to compile the libraries, you can provide paths with the following environment variables
```
TARGET_NPC = /path/to/ncurses/
TARGET_NPI = /path/to/ncurses-for-rpi/
TARGET_WPI = /path/to/wiringPi/
```
`RPI_COMPILER = /path/to/cross-compiler/` will still be necessary for compiling the project.

## Compiling the project
Once the libraries are available, a simple `make` will do the trick providing that the environment variable
`RPI_COMPILER = /path/to/cross-compiler/` is set. The compiled application for raspberryPi will be 
available in /build.

## Acknowledgments
Gordon (@Drogon) - https://projects.drogon.net  
Thomas Dickey - https://invisible-island.net/