#include <stdio.h>
#include <ncurses.h>
#include <string.h>
#include <unistd.h>
#include "../include/main.h" // here are relative paths. You can use <> but did not work great with linter.
#include "../include/helper.h"
#include "../lib/helper.c"

#define WIDTH 10
#define HEIGHT 21
#define MARGIN 2
#define N_CHOICES 19
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
bool fromLeft = TRUE;

int main()
{	
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
		init_pair(1, COLOR_RED,     COLOR_BLACK);
        init_pair(2, COLOR_GREEN,   COLOR_BLACK);
        init_pair(3, COLOR_YELLOW,  COLOR_BLACK);
        init_pair(4, COLOR_BLUE,    COLOR_BLACK);
        init_pair(5, COLOR_CYAN,    COLOR_BLACK);
        init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
        init_pair(7, COLOR_WHITE,   COLOR_BLACK);
	}

	gpio_win = new_middle_window(NULL, HEIGHT, WIDTH, MARGIN);
	wattron(gpio_win, A_BOLD); // On utilise des caractères gras pour cette window
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
	wattron(gpio_win, COLOR_PAIR(7));
	box(gpio_win, 0, 0);
	wrefresh(gpio_win);

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
			else
				display_dialog("Activating pin %s", choice_char);
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

void print_pin(WINDOW* win, int i, int color){
	wattron(win, color);
	mvwprintw(win, i, MARGIN, "O");
	wattron(win, color);
}

void fill_gpio(WINDOW* win){
	print_pin(win, 1, COLOR_PAIR(1));
}