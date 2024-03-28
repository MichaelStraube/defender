#include "sprite.h"

void sprite_move(struct sprite *sprite, float y, float x)
{
	if (!sprite)
		return;

	if (!sprite->alive)
		return;

	if (x < 0)
		x = 0;
	if (x > WIDTH - sprite->width)
		x = WIDTH - sprite->width;
	if (y < 0)
		y = 0;
	if (y > HEIGHT - sprite->height)
		y = HEIGHT - sprite->height;

	sprite->y = y;
	sprite->x = x;
}

void sprite_draw(struct sprite *sprite, struct canvas *canvas)
{
	if (!sprite)
		return;

	if (!sprite->alive)
		return;

	// implicit type casting
	int y = sprite->y;
	int x = sprite->x;

	// TODO: height for sprites with height > 1
	for (int i = 0; i < sprite->width; i++) {
		canvas->pixels[y * WIDTH + x + i].ascii = sprite->ascii[i];
		canvas->pixels[y * WIDTH + x + i].color = sprite->color;
	}
}

bool sprite_collision(struct sprite *sprite1, struct sprite *sprite2)
{
	if (!sprite1 || ! sprite2)
		return false;

	if (!sprite1->alive || !sprite2->alive)
		return false;

	if ((int)sprite1->x < (int)sprite2->x + sprite2->width &&
	    (int)sprite1->x + sprite1->width > (int)sprite2->x &&
	    (int)sprite1->y < (int)sprite2->y + sprite2->height &&
	    (int)sprite1->y + sprite1->height > (int)sprite2->y) {
		return true;
	}

	return false;
}
