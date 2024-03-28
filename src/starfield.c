#include <stdlib.h>
#include <time.h>
#include "starfield.h"
#include "game.h"
#include "term.h"
#include "canvas.h"

struct starfield *new_starfield(void)
{
	struct starfield *sf = malloc(sizeof(*sf));
	if (!sf)
		return NULL;

	srand(time(NULL));
	for (int i = 0; i < NUM_STARS; i++) {
		sf->stars[i].x = rand() % WIDTH;
		sf->stars[i].y = rand() % HEIGHT;
		sf->stars[i].layer = rand() % NUM_LAYERS;
	}

	return sf;
}

void delete_starfield(struct starfield *sf)
{
	free(sf);
}

void starfield_update(struct starfield *sf, float ftime)
{
	if (!sf)
		return;

	float speed = 20;

	for (int i = 0; i < NUM_STARS; i++) {
		sf->stars[i].x -= speed * ftime * (sf->stars[i].layer + 1);
		if ((int)sf->stars[i].x < 0) {
			sf->stars[i].x = WIDTH - 1;
			sf->stars[i].y = rand() % HEIGHT;
			//sf->stars[i].layer = rand() % NUM_LAYERS;
		}
	}
}

void starfield_draw(struct starfield *sf, struct canvas *canvas)
{
	if(!sf || !canvas)
		return;

	int x, y, layer, color;
	struct pixel *pixel;

	for (int i = 0; i < NUM_STARS; i++) {
		x = sf->stars[i].x;
		y = sf->stars[i].y;
		layer = sf->stars[i].layer;
		switch (layer) {
		case 0:
			color = CYAN;
			break;
		case 1:
			color = WHITE;
			break;
		default:
			color = WHITE;
			break;
		}
		pixel = canvas->pixels;
		pixel += y * WIDTH + x;
		pixel->ascii = '.';
		pixel->color = color;
	}
}
