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

all: PC RPI

PC:
	gcc $(CFLAGS_PC) src/main.c $(LDFLAGS_PC) -o build/gpio_PC.exe

RPI:
	$(RPI_COMPILER) $(CFLAGS_PI) src/main.c $(LDFLAGS_PI) -o build/gpio_Pi.exe

clean:
	rm -rf build/*.exe