#include "ncurses/ncurses.h"

void print_menu(WINDOW *menu_win, int highlight, char ** choices);
void change_format();
void fill_gpio(WINDOW* win);
void print_pin(WINDOW* win, int i, int alignment, int color);
int choose_color(char *pin);
void print_legend();