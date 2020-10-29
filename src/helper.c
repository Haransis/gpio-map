#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <helper.h>
#include <unistd.h>

WINDOW* new_middle_window(WINDOW *win, int height, int width, int margin) {
	int sizex, sizey, startx, starty;
	WINDOW* middle_window;

	if(win == NULL)
		win = stdscr;
	getmaxyx(win, sizey, sizex);
	if(width > sizex/5 || width == 0)
		width = sizex/5;
	if(height > sizey - 2*margin || height == 0)
		height = sizey - 2*margin;
	
	startx = (sizex - width)/2;
	starty = (sizey - height)/2;
	middle_window = newwin(height, width, starty, startx);
	box(middle_window, 0, 0);
	wrefresh(middle_window);
	return middle_window;
}

int max_len(char * strings[], int size) {
    int max = 0;
    int i;
    for (i = 0; i < size; i++) {
        if ((int)strlen(strings[i]) > max)
            max = (int)strlen(strings[i]);
    }
    return max;
}

WINDOW* new_side_window(WINDOW *win, int height, int width, int side) {
    int starty, startx;
    WINDOW* new_window;

    starty = getbegy(win);
    if (side == LEFT_SIDE) {
        move(getbegy(win),getbegx(win));
        startx = getbegx(win)-width;
    } else {
        move(getbegy(win), getbegx(win) + getmaxx(win));
        startx = getcurx(stdscr);
    }
    new_window = newwin(height, width, starty, startx);
    //wrefresh(new_window);
    return new_window;
}

void print_in_middle(WINDOW *win, int starty, int startx, char *string)
{	
    int length, x, y;

    if(win == NULL)
		win = stdscr;
    
    length = strlen(string);
	getmaxyx(win, y, x);
    
    if(startx != 0){
        x = startx;
    } else {
        // Si la taille du message est trop grande...
        if (x < length){
            x = startx;
        } else{
            x = (x-length)/2;
        }
    }

	if(starty != 0){
		y = starty;
    } else {
        y = y/2;
    }

	mvwprintw(win, y, x, "%s", string);
	wrefresh(win);
}

void display_dialog(const char * string, ...) 
{
    WINDOW* dialog;
    char display[30];
    va_list arg;
    int width, height;

    // On construit le message à afficher en fonction des paramètres
    va_start (arg, string);
    vsprintf(display, string, arg);

    // On calcule la taille du dialog
    getmaxyx(stdscr, height, width);
    height = height/6;
    width = width/4;
    if (height < 5)
        height = 5;
    if (width < 35)
        width = 35;
    dialog = new_middle_window(stdscr, height, width, 0);
    print_in_middle(dialog, 0, 0, display);
    sleep(1);
    wclear(dialog);
    wrefresh(dialog);
    delwin(dialog);
}

bool contained(int value, const int a[], int size) {
    int i = 0;
    while (i < size && a[i] != value) i++;
    return i != size;
}