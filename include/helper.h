#include <ncurses/ncurses.h>
#define LEFT_SIDE 0
#define RIGHT_SIDE 1

/* Renvoie une nouvelle fenêtre centrée sur win 
   aux dimensions passées en paramètres  */
WINDOW* new_middle_window(WINDOW *win, int height, int width, int margin);

/* Renvoie une nouvelle fenêtre à côté de win 
   aux dimensions passées en paramètres */
WINDOW* new_side_window(WINDOW *win, int height, int width, int side);

/* Renvoie la plus grande longueur d'une chaîne de caractère parmi une liste*/
int max_len(char *strings[], int size);

/* Affiche une string au milieu de win */
void print_in_middle(WINDOW *win, int starty, int startx, char *string);

/* Affiche un dialogue pendant 1s contenant string.
   Il faut redessiner les fenêtres après l'appel à cette fonction 
   (une piste d'amélioration serait d'utiliser un panel de la Library Panel) */
void display_dialog(const char * string, ...);

/* Renvoie true si value est dans a false sinon */
bool contained(int value, const int a[], int size);

WINDOW* create_waiting_window(const char * string, ...);