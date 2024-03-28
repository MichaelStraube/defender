#include <stdio.h>
#include "keys.h"
#include "hiscore.h"
#include "term.h"

// Title state
void handle_keys_title(struct starfield *starfield, struct game *game, char c)
{
	switch (c) {
	case 'q':
		game->quit = true;
		break;
	case ' ':
		// new game
		game->ship.lives = 3;
		game->score = 0;
		game->wave = 1;
		game->hiscore = hiscore_read();
		game_init_wave(game);
		starfield->animate = false;
		game->state = PLAY;
		break;
	}
}

// Play state
void handle_keys_play(struct starfield *starfield, struct game *game, char c)
{
	struct sprite *ship = &game->ship;
	struct sprite *bullet = &game->bullet;

	switch (c) {
	case 'q':
		game->state = TITLE;
		break;
	case 'w':
		sprite_move(ship, ship->y - 1, ship->x); // up
		break;
	case 's':
		sprite_move(ship, ship->y + 1, ship->x); // down
		break;
	case 'a':
		sprite_move(ship, ship->y, ship->x - 1); // left
		break;
	case 'd':
		sprite_move(ship, ship->y, ship->x + 1); // right
		break;
	case ' ':
		if (ship->alive && !bullet->alive) {
			// shoot
			bullet->y = ship->y - 1;
			bullet->x = ship->x + ship->width / 2;
			bullet->alive = true;
		}
		break;
	case 'o':
		starfield->animate = !starfield->animate;
		break;
	}
}

// Wave cleared state
void handle_keys_wave_cleared(struct starfield *starfield, struct game *game, char c)
{
	// TODO: autostart next wave after some seconds

	switch (c) {
	case ' ':
		// next wave
		game->wave++;
		game_init_wave(game);
		starfield->animate = false;
		game->state = PLAY;
		break;
	}
}

// Game over state
void handle_keys_game_over(struct game *game, char c)
{
	switch (c) {
	case 'q':
		game->state = TITLE;
		break;
	}
}

void handle_keys(struct starfield *starfield, struct game *game)
{
	if (!term_kbhit())
		return;

	char c = getchar();

	switch (game->state) {
	case TITLE:
		handle_keys_title(starfield, game, c);
		break;
	case PLAY:
		handle_keys_play(starfield, game, c);
		break;
	case WAVE_CLEARED:
		handle_keys_wave_cleared(starfield, game, c);
		break;
	case GAME_OVER:
		handle_keys_game_over(game, c);
		break;
	}
}
