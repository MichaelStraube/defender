#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include "term.h"

void term_color(int color)
{
	switch (color) {
	case RED:
		printf("\033[31m");
		break;
	case GREEN:
		printf("\033[32m");
		break;
	case YELLOW:
		printf("\033[33m");
		break;
	case BLUE:
		printf("\033[34m");
		break;
	case MAGENTA:
		printf("\033[35m");
		break;
	case CYAN:
		printf("\033[36m");
		break;
	case WHITE:
		printf("\033[37m");
		break;
	case BLINK:
		printf("\033[5m");
		break;
	case NO_COLOR:
		printf("\033[0m");
		break;
	default:
		// do nothing
		break;
	}
}

void term_clear_screen()
{
	printf("\033[1;1H\033[2J");
}

void term_move_cursor(int y, int x)
{
	printf("\033[%d;%dH", y, x);
}

void term_cursor(int mode)
{
	if (mode == HIDE)
		printf("\033[?25l");
	else if (mode == SHOW)
		printf("\033[?25h");
}

// mode 1 : noncanonical
// mode 0 : restore original mode
void term_change_mode(int mode)
{
	static struct termios oldt;
	struct termios newt;

	if (mode == 1) {
		tcgetattr(STDIN_FILENO, &oldt);
		newt = oldt;
		newt.c_lflag &= ~(ICANON | ECHO);
		tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	} else {
		tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	}
}

int term_kbhit(void)
{
	struct timeval tv;
	fd_set rdfs;

	tv.tv_sec = 0;
	tv.tv_usec = 0;

	FD_ZERO(&rdfs);
	FD_SET(STDIN_FILENO, &rdfs);

	select(STDIN_FILENO + 1, &rdfs, NULL, NULL, &tv);
	return FD_ISSET(STDIN_FILENO, &rdfs);
}
