#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include "canvas.h"
#include "sprite.h"

#define MAX_TEXT_LEN 100

#define MAX_NUM_ALIENS 50
#define NUM_ALIENS 12

enum states {TITLE, PLAY, WAVE_CLEARED, GAME_OVER};

struct game {
	struct sprite ship;
	struct sprite bullet;
	struct sprite aliens[MAX_NUM_ALIENS];
	int state;
	int wave;
	int score;
	int hiscore;
	bool quit;
};

struct game *new_game(void);
void delete_game(struct game *game);
void game_draw(struct game *game, struct canvas *canvas);
void game_init_wave(struct game *game);

#endif // GAME_H
