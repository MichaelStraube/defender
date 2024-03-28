#include <stdlib.h>
#include <stdio.h>
#include "canvas.h"
#include "term.h"

struct canvas *new_canvas(void)
{
	struct canvas *canvas = malloc(sizeof(*canvas));
	if (!canvas)
		return NULL;

	return canvas;
}

void delete_canvas(struct canvas *canvas)
{
	free(canvas);
}

void canvas_clear(struct canvas *canvas)
{
	if (!canvas)
		return;

	struct pixel *pixel = canvas->pixels;

	for (int i = 0; i < WIDTH * HEIGHT; i++) {
		pixel->ascii = ' ';
		pixel->color = NO_COLOR;
		pixel++;
	}
}

void canvas_print(struct canvas *canvas)
{
	if (!canvas)
		return;

	struct pixel *pixel = canvas->pixels;

	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			term_color(pixel->color);
			putchar(pixel->ascii);
			pixel++;
		}
		putchar('\n');
	}
	term_color(NO_COLOR);
}
