#ifndef SPRITE_H
#define SPRITE_H

#include <stdbool.h>
#include "canvas.h"

#define MAX_SPRITE_WIDTH 10

enum directions {LEFT, RIGHT};

struct sprite {
	float y;
	float x;
	int width;
	int height;
	float velocity;
	bool alive;
	int lives;
	int color;
	char ascii[MAX_SPRITE_WIDTH];
	int direction;
};

// struct sprite *new_sprite();
// void delete_sprite(struct sprite *sprite);
void sprite_move(struct sprite *sprite, float y, float x);
void sprite_draw(struct sprite *sprite, struct canvas *canvas);
bool sprite_collision(struct sprite *sprite1, struct sprite *sprite2);

#endif // SPRITE_H
