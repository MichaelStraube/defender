#ifndef TERM_H
#define TERM_H

enum colors {BLINK = 100, NO_COLOR, RED = 0, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE, NUM_COLORS};
enum modes {HIDE, SHOW};

void term_color(int color);
void term_clear_screen();
void term_move_cursor(int x, int y);
void term_cursor(int mode);
void term_change_mode(int mode);
int term_kbhit(void);

#endif // TERM_H
