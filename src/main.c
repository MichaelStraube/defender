#include <stdio.h>
#include <stdbool.h>
#include <unistd.h> // usleep
#include <stdlib.h>
#include <errno.h>
#include "term.h"
#include "util.h"
#include "sprite.h"
#include "game.h"
#include "hiscore.h"
#include "starfield.h"
#include "canvas.h"
#include "scroller.h"
#include "keys.h"

#define NUM_SCROLLERS 4

static void init_scrollers(struct scroller **scrollers)
{
	float speed = 30;
	int y = 10;
	int color = CYAN;
	const char *lines[NUM_SCROLLERS] = {
		"  _____                    __             __  __     _   _____           _      ___ ____    ___  ___  ___ ___",
		" / ___/__ ___ _  ___   __ / /__ ___ _    / / / /__  (_) / ___/__  ___   | | /| / (_) __/__ |_  |/ _ \\|_  <  /",
		"/ (_ / _ `/  ' \\/ -_) / // / _ `/  ' \\  / /_/ / _ \\/ / / (_ / _ \\/ -_)  | |/ |/ / /\\ \\/ -_) __// // / __// / ",
		"\\___/\\_,_/_/_/_/\\__/  \\___/\\_,_/_/_/_/  \\____/_//_/_/  \\___/\\___/\\__/   |__/|__/_/___/\\__/____/\\___/____/_/  ",
	};

	for (int i = 0; i < NUM_SCROLLERS; i++) {
		scrollers[i] = new_scroller(lines[i], y + i, color, speed);
	}
}

static void destroy_scrollers(struct scroller **scrollers)
{
	for (int i = 0; i < NUM_SCROLLERS; i++) {
		delete_scroller(scrollers[i]);
	}
}

static void draw(struct canvas *canvas, struct game *game, struct starfield *starfield, struct scroller **scrollers)
{
	canvas_clear(canvas);

	// draw starfield
	if (game->state != TITLE)
		starfield_draw(starfield, canvas);

	// draw scrollers
	if (game->state == GAME_OVER) {
		for (int i = 0; i < NUM_SCROLLERS; i++) {
			scroller_draw(scrollers[i], canvas);
		}
	}

	game_draw(game, canvas);

	// print canvas to screen
	term_clear_screen();
	if (game->state != TITLE)
		canvas_print(canvas);
}

static void update(struct game *game, struct starfield *starfield, struct scroller **scrollers, float ftime)
{
	struct sprite *ship = &game->ship;
	struct sprite *bullet = &game->bullet;
	struct sprite *aliens = game->aliens;

	switch (game->state) {
	case PLAY:
		// move aliens
		for (int i = 0; i < NUM_ALIENS; i++) {
			struct sprite *alien = &aliens[i];
			float deltax = alien->velocity * ftime;

			if (alien->direction == LEFT) {
				deltax *= -1;
			}
			sprite_move(alien, alien->y, alien->x + deltax);
			if (alien->x == WIDTH - alien->width) {
				sprite_move(alien, alien->y + 2, alien->x);
				alien->direction = LEFT;
				alien->velocity += 5;
			}
			if (alien->x == 0) {
				sprite_move(alien, alien->y + 2, alien->x);
				alien->direction = RIGHT;
			}
		}

		// move bullet
		if (bullet->alive) {
			sprite_move(bullet, bullet->y - bullet->velocity * ftime, bullet->x);
			if (bullet->y == 0) {
				bullet->alive = false;
			}
		}

		// check for collisions
		int dead_aliens = 0;
		for (int i = 0; i < NUM_ALIENS; i++) {
			struct sprite *alien = &aliens[i];

			if (alien->alive) {
				if (bullet->alive && sprite_collision(bullet, alien)) {
					bullet->alive = false;
					alien->alive = false;
					game->score += 10 + game->wave * 10 + (int)alien->y;
				}

				if (sprite_collision(ship, alien)) {
					alien->alive = false;
					ship->lives--;
					if (ship->lives > 0) {
						// restart current wave
						game_init_wave(game);
					} else {
						ship->alive = false;
						game->state = GAME_OVER;
						starfield->animate = true;
						if (game->score > game->hiscore) {
							// TODO: print "new hiscore" or soemthing
							hiscore_write(game->score);
						}
					}
				}
			} else {
				dead_aliens++;
			}
		}

		// check if wave cleared
		if (dead_aliens == NUM_ALIENS && ship->alive) {
			game->state = WAVE_CLEARED;
			starfield->animate = true;
			game->score += 100 * game->wave;
		}
		break;
	case GAME_OVER:
		// animate scrollers
		for (int i = 0; i < NUM_SCROLLERS; i++) {
			scroller_update(scrollers[i], ftime);
		}
		break;
	}

	// animate starfield
	if (starfield->animate)
		starfield_update(starfield, ftime);
}

int main(void)
{
	int ret = 0;
	float ftime;
	float elapsed;
	long start_ticks;
	struct canvas *canvas = NULL;
	struct starfield *starfield = NULL;
	struct scroller *scrollers[NUM_SCROLLERS] = {0};
	struct game *game = NULL;

	init_scrollers(scrollers);
	for (int i = 0; i < NUM_SCROLLERS; i++) {
		if (!scrollers[i]) {
			ret = ENOMEM;
			goto cleanup;
		}
	}

	canvas = new_canvas();
	starfield = new_starfield();
	game = new_game();

	if (!canvas || !starfield || !game) {
		ret = ENOMEM;
		goto cleanup;
	}

	// set terminal in noncanonical mode
	term_change_mode(1);
	term_cursor(HIDE);

	game->state = TITLE;
	start_ticks = get_ticks();

	// main loop
	while (!game->quit) {
		ftime = (get_ticks() - start_ticks) / 1000.0; // in seconds
		start_ticks = get_ticks();

		handle_keys(starfield, game);

		update(game, starfield, scrollers, ftime);

		draw(canvas, game, starfield, scrollers);

		// cap to 60 "FPS"
		elapsed = get_ticks() - start_ticks; // in milliseconds
		usleep((long)(16.666 - elapsed) * 1000);
	}

	// restore terminal
	term_cursor(SHOW);
	term_change_mode(0);

cleanup:
	destroy_scrollers(scrollers);
	delete_game(game);
	delete_starfield(starfield);
	delete_canvas(canvas);

	return ret;
}
