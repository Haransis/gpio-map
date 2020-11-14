#
# Makefile:
#	wiringPi - Wiring Compatable library for the Raspberry Pi
#
#	Copyright (c) 2012-2015 Gordon Henderson
#################################################################################
# This file is part of wiringPi:
#	https://projects.drogon.net/raspberry-pi/wiringpi/
#
#    wiringPi is free software: you can redistribute it and/or modify
#    it under the terms of the GNU Lesser General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    wiringPi is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU Lesser General Public License for more details.
#
#    You should have received a copy of the GNU Lesser General Public License
#    along with wiringPi.  If not, see <http://www.gnu.org/licenses/>.
#################################################################################

VERSION=$(shell cat ../VERSION)
#MODIFICATION 2 : On souhaite l'emplacement d'arrivée de l'installation
#DESTDIR?=/usr # ?= donne un valeur seulement si DESTDIR n'a pas déjà une valeur dans l'environnement.
#PREFIX?=/local
#DESTDIR?=/home/inao/CentraleLille/PSE/S6/target_wiringPi
#PREFIX=
mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
current_dir := $(patsubst %/,%,$(dir $(mkfile_path)))
default_dest_wiring_Pi := $(patsubst %/,%,$(dir $(current_dir)))/build# remove one level of imbrication and add build folder
DESTDIR?= $(default_dest_wiring_Pi)

#MODIFICATION 1: On souhaite compiler avec un compilateur croisé. Il va donc falloir le spécifier. Avec $(CC)
#création de la macro CC.
CC=$(RPI_COMPILER)

LDCONFIG?=ldconfig

#@ désactive l'affichage de la règle. Affiche uniquement la sortie de la commande
# Si V!=1 (en général V n'a pas de valeur, ce qui est différent de 1 en général)
ifneq ($V,1)
Q ?= @ #Désactive l'affiche de la commande. $Q vaut @. Sinon $Q ne vaut rien.
endif

STATIC=libwiringPi.a
DYNAMIC=libwiringPi.so.$(VERSION)

#DEBUG	= -g -O0
DEBUG	= -O2
INCLUDE	= -I.
DEFS	= -D_GNU_SOURCE
CFLAGS	= $(DEBUG) $(DEFS) -Wformat=2 -Wall -Wextra -Winline $(INCLUDE) -pipe -fPIC
#CFLAGS	= $(DEBUG) $(DEFS) -Wformat=2 -Wall -Wextra -Wconversion -Winline $(INCLUDE) -pipe -fPIC

LIBS    = -lm -lpthread -lrt -lcrypt

###############################################################################

SRC	=	wiringPi.c						\
		wiringSerial.c wiringShift.c				\
		piHiPri.c piThread.c					\
		wiringPiSPI.c wiringPiI2C.c				\
		softPwm.c softTone.c					\
		mcp23008.c mcp23016.c mcp23017.c			\
		mcp23s08.c mcp23s17.c					\
		sr595.c							\
		pcf8574.c pcf8591.c					\
		mcp3002.c mcp3004.c mcp4802.c mcp3422.c			\
		max31855.c max5322.c ads1115.c				\
		sn3218.c						\
		bmp180.c htu21d.c ds18b20.c rht03.c			\
		drcSerial.c drcNet.c					\
		pseudoPins.c						\
		wpiExtensions.c

HEADERS =	$(shell ls *.h)

# ${SRC:.c=.o} means, take the variable value ${SRC}, which is a string composed of words separated by spaces, and for each word, replace the suffix .c with .o
OBJ	=	$(SRC:.c=.o)

all:		$(DYNAMIC)

# PHONY specify that static is a fictionnal target. Make should not expect to find a file.
.PHONY:	static
static:	
		$Q cat noMoreStatic

$(DYNAMIC):	$(OBJ)
	$Q echo "[Link (Dynamic)]"
	$Q echo $(DESTDIR)
	$Q $(CC) -shared -Wl,-soname,libwiringPi.so$(WIRINGPI_SONAME_SUFFIX) -o libwiringPi.so.$(VERSION) $(LIBS) $(OBJ)

#$@ is the name of the target being generated, and $< the first prerequisite (plusieurs prerequisite)
# It's an old-fashioned suffix rule. The more up-to-date way to do it is to use a pattern rule: %.o : %.c
# "%.o: %.c" means that any file ending in .o depends on the same filename ending in .c to be present.
# $^ full prerequisites
.c.o:
	$Q echo [Compile] $<
	$Q $(CC) -c $(CFLAGS) $< -o $@


.PHONY:	clean
clean:
	$Q echo "[Clean]"
	$Q rm -f $(OBJ) $(OBJ_I2C) *~ core tags Makefile.bak libwiringPi.*

.PHONY:	tags
tags:	$(SRC)
	$Q echo [ctags]
	$Q ctags $(SRC)


.PHONY:	install
install:	$(DYNAMIC)
	$Q echo "[Install Headers]"
	$Q install -m 0755 -d						$(DESTDIR)$(PREFIX)/include
	$Q install -m 0644 $(HEADERS)					$(DESTDIR)$(PREFIX)/include
	$Q echo "[Install Dynamic Lib]"
	$Q install -m 0755 -d						$(DESTDIR)$(PREFIX)/lib
	$Q install -m 0755 libwiringPi.so.$(VERSION)			$(DESTDIR)$(PREFIX)/lib/libwiringPi.so.$(VERSION)
	$Q ln -sf $(DESTDIR)$(PREFIX)/lib/libwiringPi.so.$(VERSION)	$(DESTDIR)/lib/libwiringPi.so
#$Q $(LDCONFIG) #We are compiling on a x64-86 for an arm. So we don't wan't to run ldconfig to notify the system that this library exist as it can't use it

.PHONY:	install-deb
install-deb:	$(DYNAMIC)
	$Q echo "[Install Headers: deb]"
	$Q install -m 0755 -d							~/wiringPi/debian-template/wiringPi/usr/include
	$Q install -m 0644 $(HEADERS)						~/wiringPi/debian-template/wiringPi/usr/include
	$Q echo "[Install Dynamic Lib: deb]"
	install -m 0755 -d							~/wiringPi/debian-template/wiringPi/usr/lib
	install -m 0755 libwiringPi.so.$(VERSION)				~/wiringPi/debian-template/wiringPi/usr/lib/libwiringPi.so.$(VERSION)
	ln -sf ~/wiringPi/debian-template/wiringPi/usr/lib/libwiringPi.so.$(VERSION)	~/wiringPi/debian-template/wiringPi/usr/lib/libwiringPi.so

.PHONY:	uninstall
uninstall:
	$Q echo "[UnInstall]"
	$Q cd $(DESTDIR)$(PREFIX)/include/ && rm -f $(HEADERS)
	$Q cd $(DESTDIR)$(PREFIX)/lib/     && rm -f libwiringPi.*
	$Q $(LDCONFIG)


.PHONY:	depend
depend:
	makedepend -Y $(SRC) $(SRC_I2C)

# DO NOT DELETE

wiringPi.o: softPwm.h softTone.h wiringPi.h ../version.h
wiringSerial.o: wiringSerial.h
wiringShift.o: wiringPi.h wiringShift.h
piHiPri.o: wiringPi.h
piThread.o: wiringPi.h
wiringPiSPI.o: wiringPi.h wiringPiSPI.h
wiringPiI2C.o: wiringPi.h wiringPiI2C.h
softPwm.o: wiringPi.h softPwm.h
softTone.o: wiringPi.h softTone.h
mcp23008.o: wiringPi.h wiringPiI2C.h mcp23x0817.h mcp23008.h
mcp23016.o: wiringPi.h wiringPiI2C.h mcp23016.h mcp23016reg.h
mcp23017.o: wiringPi.h wiringPiI2C.h mcp23x0817.h mcp23017.h
mcp23s08.o: wiringPi.h wiringPiSPI.h mcp23x0817.h mcp23s08.h
mcp23s17.o: wiringPi.h wiringPiSPI.h mcp23x0817.h mcp23s17.h
sr595.o: wiringPi.h sr595.h
pcf8574.o: wiringPi.h wiringPiI2C.h pcf8574.h
pcf8591.o: wiringPi.h wiringPiI2C.h pcf8591.h
mcp3002.o: wiringPi.h wiringPiSPI.h mcp3002.h
mcp3004.o: wiringPi.h wiringPiSPI.h mcp3004.h
mcp4802.o: wiringPi.h wiringPiSPI.h mcp4802.h
mcp3422.o: wiringPi.h wiringPiI2C.h mcp3422.h
max31855.o: wiringPi.h wiringPiSPI.h max31855.h
max5322.o: wiringPi.h wiringPiSPI.h max5322.h
ads1115.o: wiringPi.h wiringPiI2C.h ads1115.h
sn3218.o: wiringPi.h wiringPiI2C.h sn3218.h
bmp180.o: wiringPi.h wiringPiI2C.h bmp180.h
htu21d.o: wiringPi.h wiringPiI2C.h htu21d.h
ds18b20.o: wiringPi.h ds18b20.h
drcSerial.o: wiringPi.h wiringSerial.h drcSerial.h
pseudoPins.o: wiringPi.h pseudoPins.h
wpiExtensions.o: wiringPi.h mcp23008.h mcp23016.h mcp23017.h mcp23s08.h
wpiExtensions.o: mcp23s17.h sr595.h pcf8574.h pcf8591.h mcp3002.h mcp3004.h
wpiExtensions.o: mcp4802.h mcp3422.h max31855.h max5322.h ads1115.h sn3218.h
wpiExtensions.o: drcSerial.h pseudoPins.h bmp180.h htu21d.h ds18b20.h
wpiExtensions.o: wpiExtensions.h
