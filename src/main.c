#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
//#include <wiringPi.h>

#include "main.h" // here are relative paths. You can use <> but did not work great with linter.
#include "helper.h"
//#include "helper.c"

#define N_CHOICES 20
#define WIDTH 10
#define HEIGHT N_CHOICES+2
#define MARGIN 2
#define CHOICES_LEFT_REGULAR = {}
#define GROUND "Ground"
#define POWER3 "3v3 Power"
#define POWER5 "5v Power"
#define EXIT "EXIT"
#define EXIT_INDEX N_CHOICES+1

char *choices_left_regular[] = {"3v3 Power", "GPIO 2", "GPIO 3", "GPIO 4", "Ground", "GPIO 17", "GPIO 27", "GPIO 22", "3v3 Power", "GPIO 10", "GPIO 9", "GPIO 11", "Ground", "GPIO 0", "GPIO 5", "GPIO 6", "GPIO 13", "GPIO 19", "GPIO 26", "Ground"};
char *choices_right_regular[] = { "5v Power", "5v Power", "Ground", "GPIO 14", "GPIO 15", "GPIO 18", "Ground", "GPIO 23", "GPIO 24", "Ground", "GPIO 25", "GPIO 8", "GPIO 7", "GPIO 1", "Ground", "GPIO 12", "Ground", "GPIO 16", "GPIO 20", "GPIO 21"};
char *choices_left_wiring[] = { "3v3 Power", "8", "9", "7", "Ground", "0", "2", "3", "3v3 Power", "12", "13", "14", "Ground", "30", "21", "22", "23", "24", "25", "Ground"};
char *choices_right_wiring[] = {"5v Power","5v Power","Ground","15","16","1","Ground","4","5","Ground","6","10","11","31","Ground","26","Ground","27","28","29"};
char *choices_left[] = {"3v3 Power", "GPIO 2", "GPIO 3", "GPIO 4", "Ground", "GPIO 17", "GPIO 27", "GPIO 22", "3v3 Power", "GPIO 10", "GPIO 9", "GPIO 11", "Ground", "GPIO 0", "GPIO 5", "GPIO 6", "GPIO 13", "GPIO 19", "GPIO 26", "Ground"};
char *choices_right[] = { "5v Power", "5v Power", "Ground", "GPIO 14", "GPIO 15", "GPIO 18", "Ground", "GPIO 23", "GPIO 24", "Ground", "GPIO 25", "GPIO 8", "GPIO 7", "GPIO 1", "Ground", "GPIO 12", "Ground", "GPIO 16", "GPIO 20", "GPIO 21"};
const int gpio[] = {4,17,27,22,5,6,13,26,23,24,25,12,16};
const int spi[] = {10,9,11,8,7};
const int i2c[] = {2,3,0,1};
const int uart[] = {14,15};
bool fromLeft = TRUE;

int main()
{
//    wiringPiSetup();
	WINDOW *left_menu_win, *gpio_win, *right_menu_win;
	int highlight = 1;
	int choice = 0;
	int c;

	initscr();
	clear(); /* Supprime le contenu du terminal */
	noecho(); /* Évite que les inputs utilisateurs ne soient affichés */
	cbreak();	/* Le buffer ne contient qu'un input. Les signaux (ex: CTRL+C) sont reçus. */
	curs_set(0); /* Cache le curseur */
	if (has_colors()) { /* Si le terminal supporte les couleurs */
		start_color(); /* Permet d'utiliser des couleurs */
		use_default_colors(); /* Nécessaire pour utiliser les bonnes couleurs */
		init_pair(COLOR_RED,     COLOR_RED,     COLOR_BLACK);
        init_pair(COLOR_GREEN,   COLOR_GREEN,   COLOR_BLACK);
        init_pair(COLOR_YELLOW,  COLOR_YELLOW,  COLOR_BLACK);
        init_pair(COLOR_BLUE,    COLOR_BLUE,    COLOR_BLACK);
        init_pair(COLOR_CYAN,    COLOR_CYAN,    COLOR_BLACK);
		init_pair(COLOR_MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
        init_pair(COLOR_BLACK+9, COLOR_BLACK,   COLOR_BLACK);
        init_pair(COLOR_WHITE,   COLOR_WHITE,   COLOR_BLACK);
	}

	//On vérifie que le terminal est suffisamment grand pour afficher le pinout.
	if (getmaxy(stdscr) < HEIGHT + 2*MARGIN){
		display_dialog("Terminal size insufficient");
		endwin();
		return 0;
	}

	gpio_win = new_middle_window(NULL, HEIGHT, WIDTH, MARGIN);
	fill_gpio(gpio_win);

	// On crée deux window de chaque côté du RPi Pinout
	left_menu_win = new_side_window(gpio_win, HEIGHT, max_len(choices_left,10)+3, LEFT_SIDE);
	right_menu_win = new_side_window(gpio_win, HEIGHT, max_len(choices_right,10)+3, RIGHT_SIDE);
	
	// Ces deux fenêtres sont capables d'interpréter les input utilisateurs.
	keypad(left_menu_win, TRUE);
	keypad(right_menu_win, TRUE);

	// Affiche les instructions sur stdscr.
	mvprintw(0, 0, "Press ENTER to select a choice.\n(BACKSPACE to switch to wiringPi)");
	print_in_middle(stdscr, getbegy(gpio_win)+getmaxy(gpio_win), 0, EXIT);
	refresh();

	// On affiche le gpio pinout
	wattron(gpio_win, A_BOLD); // On utilise des caractères gras pour cette window
	wattron(gpio_win, COLOR_PAIR(7));
	box(gpio_win, 0, 0);
	wrefresh(gpio_win);

	// On affiche la légende si la place nécessaire est disponible
	int available_width = getmaxx(stdscr)-(getbegx(right_menu_win) + getmaxx(right_menu_win));
	if (available_width > WIDTH*4)
		print_legend();

	print_menu(left_menu_win, highlight, choices_left); // 1 est sélectionné par défaut la colonne
	print_menu(right_menu_win, -1, choices_right); //print_menu(,-1,) déselectionne la colonne
	while(1) {	
		c = wgetch((fromLeft) ? left_menu_win : right_menu_win); // attends un input utilisateur
		switch(c) {	
			case KEY_UP:
				if(highlight == 1)
					highlight = EXIT_INDEX;
				else
					--highlight;
				break;
			case KEY_DOWN:
				if(highlight == EXIT_INDEX)
					highlight = 1;
				else 
					++highlight;
				break;
			case KEY_LEFT:
			case KEY_RIGHT:
				fromLeft = !fromLeft;
				break;
			case 10: // enter
				choice = highlight;
				break;
			case KEY_BACKSPACE: // backspace
			case '\b': // backspace
			case 127: // backspace
				change_format();
				break;
			default:
				mvprintw(getmaxy(stdscr)-1, 0, "Unauthorized key. You pressed '%c'", c);
				wrefresh(stdscr);
				break;
		}

		if (choice == EXIT_INDEX)	/* On quitte le programme */
			break;
		else if (choice != 0) {
			char * choice_char = (fromLeft) ? choices_left[choice-1] : choices_right[choice-1];
			if (strcmp(choice_char, GROUND) == 0 || strcmp(choice_char, POWER3) == 0 || strcmp(choice_char, POWER5) == 0)
                display_dialog("Cannot activate %s", choice_char);
			else {
                display_pin_menu(choice_char);
                //display_dialog("Activating pin %s", choice_char);
                //pinMode(wiring_Pi_choosen_pin,OUTPUT);
                //digitalWrite (wiring_Pi_choosen_pin, 0) ;
            }
			//TODO call to wiringPi here
			box(gpio_win, 0, 0);
			wrefresh(gpio_win);
			choice = 0;
		}
		// On update les menus pour afficher la surbrillance
		if (fromLeft) {
			print_menu(right_menu_win, -1, choices_right);
			print_menu(left_menu_win, highlight, choices_left);
		} else {
			print_menu(left_menu_win, -1, choices_left);
			print_menu(right_menu_win, highlight, choices_right);
		}
	}
	endwin();
	return 0;
}

void change_format()
{
	int i;
	if (strcmp(choices_left[3],"GPIO 4") == 0){
		for (i = 0; i < N_CHOICES; i++) {
			choices_left[i] = choices_left_wiring[i];
		}
	} else {
		for (i = 0; i < N_CHOICES; i++) {
			choices_left[i] = choices_left_regular[i];
		}
	}
	if (strcmp(choices_right[3],"GPIO 14") == 0){
		for (i = 0; i < N_CHOICES; i++) {
			choices_right[i] = choices_right_wiring[i];
		}
	} else {
		for (i = 0; i < N_CHOICES; i++) {
			choices_right[i] = choices_right_regular[i];
		}
	}
}

void display_pin_menu(const char * choice_pin)
{
    char *pin_mode_choices[] = {"INPUT","OUPUT ON","OUPUT OFF","OUPUT TOGGLE","OUPUT BLINK","OUPUT PWM"};
    char *pin_modes []={"INPUT","OUTPUT","PWM_OUT","CLOCK"};

    WINDOW* pin_menu_win;
    int NB_MODES =6;
    int MODE_EXIT_INDEX =NB_MODES+1;
 //   char display[30];
//    va_list arg;
    int width, height;
    int highlight =1;
    int mode_choice=0;
    char intro[100];
    // On construit le message à afficher en fonction des paramètres
    // va_start (arg, string);
    // vsprintf(display, string, arg);

    // On calcule la taille du dialog
    getmaxyx(stdscr, height, width);
    height = height/6;
    width = width;
    if (height < 10)
        height = 10;
    if (width < 1800)
        width = 1800;
    pin_menu_win = new_middle_window(stdscr, height, width, 0);

    //snprintf(intro, sizeof(intro), "Pin %s current mode : %s", choice_pin, pin_modes[get_alt(wiring_Pi_choosen_pin)]);
    snprintf(intro, sizeof(intro), "Pin %s current mode : %s", choice_pin, pin_modes[0]);
    keypad(pin_menu_win, TRUE);
    //print_in_middle(pin_menu_win, 0, 0, "Truc");
    print_pin_mode_menu(pin_menu_win, 1, pin_mode_choices,NB_MODES);
    print_in_middle(pin_menu_win, 1, 0, intro);

    while(1) {
        int c = wgetch(pin_menu_win); // attends un input utilisateur
        switch(c) {
            case KEY_UP:
                if(highlight == 1)
                    highlight = MODE_EXIT_INDEX;
                else
                    --highlight;
                break;
            case KEY_DOWN:
                if(highlight == MODE_EXIT_INDEX)
                    highlight = 1;
                else
                    ++highlight;
                break;
            case 10: // enter
                mode_choice = highlight;
                break;
            default:
                mvprintw(getmaxy(stdscr)-1, 0, "Unauthorized key. You pressed '%c'", c);
                wrefresh(stdscr);
                break;
        }

        if (mode_choice == MODE_EXIT_INDEX)	/* On quitte le programme */
            break;
        else if ( mode_choice != 0) {
            char * choice_char = pin_mode_choices[mode_choice-1];
            display_dialog("option %s", choice_char);


            wrefresh(pin_menu_win);
            mode_choice = 0;
        }
        print_pin_mode_menu(pin_menu_win, highlight, pin_mode_choices,NB_MODES);
        print_in_middle(pin_menu_win, 1, 0, intro);
    }
    endwin();
    wclear(pin_menu_win);
    wrefresh(pin_menu_win);
    delwin(pin_menu_win);
}
void print_pin_mode_menu(WINDOW *menu_win, int highlight, char ** choices, int nb_choices)
{
    int x, y, i;

    x = 1; // on démarre à 1 pour éviter la bordure
    y = 3;
    for(i = 0; i < nb_choices; ++i) {
        if(highlight == i + 1) { /* Met en surbrillance la ligne correspondante */
            wattron(menu_win, A_REVERSE);
            mvwprintw(menu_win, y, x, "%s", choices[i]);
            wattroff(menu_win, A_REVERSE);
        } else {
            mvwprintw(menu_win, y, x, "%s", choices[i]);
        }
        ++y;
    }
    if (highlight == nb_choices+1) {
        wattron(stdscr, A_REVERSE);
        print_in_middle(stdscr, getbegy(menu_win)+getmaxy(menu_win), 0, EXIT);
        wattroff(stdscr, A_REVERSE);
    } else {
        print_in_middle(stdscr, getbegy(menu_win)+getmaxy(menu_win), 0, EXIT);
    }
    wrefresh(menu_win);
}

void print_menu(WINDOW *menu_win, int highlight, char ** choices)
{
	int x, y, i;
	wclear(menu_win);

	x = 1; // on démarre à 1 pour éviter la bordure
	y = 1;
	for(i = 0; i < N_CHOICES; ++i) {	
		if(highlight == i + 1) { /* Met en surbrillance la ligne correspondante */
			wattron(menu_win, A_REVERSE);
			mvwprintw(menu_win, y, x, "%s", choices[i]);
			wattroff(menu_win, A_REVERSE);
		} else {
			mvwprintw(menu_win, y, x, "%s", choices[i]);
		}
		++y;
	}
	if (highlight == N_CHOICES+1) {
		wattron(stdscr, A_REVERSE);
		print_in_middle(stdscr, getbegy(menu_win)+getmaxy(menu_win), 0, EXIT);
		wattroff(stdscr, A_REVERSE);
	} else {
		print_in_middle(stdscr, getbegy(menu_win)+getmaxy(menu_win), 0, EXIT);
	}
	wrefresh(menu_win);
}

/* Remplit la window gpio avec les pins de couleurs correspondantes */
void fill_gpio(WINDOW* win){
	int i;

	for (i = 0; i < N_CHOICES; i++) {
		print_pin(win, i+1, MARGIN, choose_color(choices_left[i]));
		print_pin(win, i+1, getmaxx(win)-(MARGIN+1), choose_color(choices_right[i]));
	}
}

/* Affiche un pin */
void print_pin(WINDOW* win, int i, int alignment, int color){
	wattron(win, A_BOLD);
	wattron(win, color);
	mvwprintw(win, i, alignment, "o");
	wattroff(win, color);
	wattroff(win, A_BOLD);
}

/* Choisit la couleur à utiliser en fonction du pin */
int choose_color(char *pin){
	char *token, *str, *tofree;

	if (strcmp(pin, GROUND) == 0)
		return COLOR_PAIR(COLOR_BLACK+9);
	else if (strcmp(pin, POWER3) == 0)
		return COLOR_PAIR(COLOR_YELLOW);
	else if (strcmp(pin, POWER5) == 0)
		return COLOR_PAIR(COLOR_RED);
	else {
		// On copie pin pour éviter de le modifier
		tofree = str = strdup(pin);
		strsep(&str, " ");
		token = strsep(&str, " ");
		if (contained(atoi(token), gpio, 13))
			return COLOR_PAIR(COLOR_GREEN);
		else if (contained(atoi(token), spi, 5))
			return COLOR_PAIR(COLOR_MAGENTA);
		else if (contained(atoi(token), i2c, 4))
			return COLOR_PAIR(COLOR_BLUE);
		else if (contained(atoi(token), uart, 2))
			return COLOR_PAIR(COLOR_WHITE);
		else
			return COLOR_PAIR(COLOR_CYAN);
		free(tofree);
	}
}

void print_legend(){
	WINDOW *legend;
	int height, width;

	height = 12;
	width = WIDTH*4;
	legend = newwin(height, width, getmaxy(stdscr)-height, getmaxx(stdscr)-width);
	box(legend, 0, 0);
	wattron(legend, A_BOLD);
	wattron(legend, COLOR_PAIR(COLOR_WHITE));
	mvwprintw(legend, 0, 0, "LEGEND");
	print_pin(legend, 1, 1, COLOR_PAIR(COLOR_GREEN));
	wprintw(legend, " GPIO (General Purpose IO)");
	print_pin(legend, getcury(legend)+1, 1, COLOR_PAIR(COLOR_MAGENTA));
	wprintw(legend, " SPI (Serial Peripheral Interface)");
	print_pin(legend, getcury(legend)+1, 1, COLOR_PAIR(COLOR_BLUE));
	wprintw(legend, " I2C (Inter-integrated Circuit)");
	print_pin(legend, getcury(legend)+1, 1, COLOR_PAIR(COLOR_WHITE));
	wprintw(legend, " UART (Universal Asyncronous Receiver/Transmitter)");
	print_pin(legend, getcury(legend)+1, 1, COLOR_PAIR(COLOR_CYAN));
	wprintw(legend, " PCM (Pulse Code Modulation)");
	print_pin(legend, getcury(legend)+1, 1, COLOR_PAIR(COLOR_BLACK+9));
	wprintw(legend, " Ground");
	print_pin(legend, getcury(legend)+1, 1, COLOR_PAIR(COLOR_RED));
	wprintw(legend, " 5v (Power)");
	print_pin(legend, getcury(legend)+1, 1, COLOR_PAIR(COLOR_YELLOW));
	wprintw(legend, " 3.3v (Power)");
	wrefresh(legend);
}