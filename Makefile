INCLUDE = -I./include
LIB = -L./

INCLUDE_NCURSES_PC = -I$(TARGET_NPC)/include $(INCLUDE)
LIB_NCURSES_PC = -L$(TARGET_NPC)/lib $(LIB)
CFLAGS_PC = -Wall -Wextra $(INCLUDE_NCURSES_PC) -Winline -pipe
LDFLAGS_PC = $(LIB_NCURSES_PC) -lncurses -ldl

INCLUDE_NCURSES_PI = -I$(TARGET_NPI)/include $(INCLUDE)
LIB_NCURSES_PI = -L$(TARGET_NPI)/lib $(LIB)
CFLAGS_PI = -Wall -Wextra $(INCLUDE_NCURSES_PI) -Winline -pipe
LDFLAGS_PI = $(LIB_NCURSES_PI) -lncurses -ldl

ifeq ($(TARGET_NPI),)
	TARGET_NPI=./libraries/ncurses-6.1/build/for_PI/
endif

ifeq ($(TARGET_NPC),)
	TARGET_NPC=./libraries/ncurses-6.1/build/for_PC/
endif

ifeq ($(TARGET_WPI),)
	export TARGET_WPI=./libraries/wiringPi/build
endif
all: PC RPI

PC:
	gcc $(CFLAGS_PC) src/main.c $(LDFLAGS_PC) -o build/gpio_PC.exe

RPI:
	$(RPI_COMPILER) $(CFLAGS_PI) src/main.c src/helper.c $(LDFLAGS_PI) -o build/gpio_Pi.exe

libraries_RPI:
	cd libraries && $(MAKE)

libraries_PC:
	cd libraries && $(MAKE) ncurses_PC

libraries_clean:
	cd libraries && $(MAKE) clean

clean:
	rm -rf build/*.exe