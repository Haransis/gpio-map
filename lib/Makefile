
ncurses_folder=ncurses-6.1
wiringPi_folder=wiringPi

mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
current_dir := $(patsubst %/,%,$(dir $(mkfile_path)))
default_dest_wiring_Pi := $(current_dir)/$(wiringPi_folder)/build# remove one level of imbrication and add build folder
default_dest_ncurses := $(current_dir)/$(ncurses_folder)/build# remove one level of imbrication and add build folder


#@ désactive l'affichage de la règle. Affiche uniquement la sortie de la commande
# Si V!=1 (en général V n'a pas de valeur, ce qui est différent de 1 en général)
ifneq ($V,1)
	Q ?= @ #Désactive l'affiche de la commande. $Q vaut @. Sinon $Q ne vaut rien.
endif

libraries_for_RPI: wiringPi ncurses_RPI
	$Q echo "wiringPi and ncurses have been cross-compiled"

ncurses_RPI:
	$Q echo "Compiling ncurses for RPI"
	$Q cd $(ncurses_folder) && ./configure --prefix $(default_dest_ncurses)/for_PI -with-shared --build=x86_64-pc-linux-gnu --host=arm-linux-gnueabihf --with-build-cc=gcc --disable-stripping
	$Q cd $(ncurses_folder) && $(MAKE) install
	$Q echo "ncurses for RPI has been cross-compiled"


ncurses_PC:
	$Q echo "Compiling ncurses for PC"
	$Q cd $(ncurses_folder) && ./configure --prefix $(default_dest_ncurses)/for_PC
	$Q cd $(ncurses_folder) && $(MAKE) install
	$Q echo "ncurses has been compiled for PC"

.PHONY : wiringPi
wiringPi :
	$Q echo "Compiling wiringPi"
	$Q cd $(wiringPi_folder)/wiringPi && $(MAKE) install

make clean :
	$Q rm -rf $(default_dest_wiring_Pi)
	$Q rm -rf $(default_dest_ncurses)
	$Q cd $(wiringPi_folder)/wiringPi && $(MAKE) clean
	$Q cd $(ncurses_folder) && $(MAKE) clean

#Building wiringPi
#dans wiringPi/wiringPi make install
