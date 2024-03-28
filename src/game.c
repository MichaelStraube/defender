#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "game.h"
#include "term.h"
#include "sprite.h"
#include "title.h"

struct game *new_game(void)
{
	struct game *game = malloc(sizeof(*game));
	if (!game)
		return NULL;

	game->quit = false;
	return game;
}

void delete_game(struct game *game)
{
	free(game);
}

static void draw_text(char *text, int y, int x, int color, struct canvas *canvas)
{
	if (!text || !canvas)
		return;

	char *read = text;
	struct pixel *pixel = canvas->pixels;

	if (y < 0 || y >= HEIGHT || x < 0 || x >= WIDTH)
		return;

	pixel += y * WIDTH + x;

	while (x < WIDTH && *read) {
		pixel->ascii = *read++;
		pixel->color = color;
		pixel++;
		x++;
	}
}

static void draw_hud(struct game *game, struct canvas *canvas)
{
	if (!game || !canvas)
		return;

	int wave = game->wave;
	int lives = game->ship.lives;
	int score = game->score;
	int hiscore = game->hiscore;
	struct pixel *pixel;

	// waves
	pixel = canvas->pixels; // pointer to top left pixel
	for (int i = 0; i < wave; i++) {
		pixel->ascii = '*';
		pixel->color = GREEN;
		pixel++;
	}

	// lives
	pixel = canvas->pixels;
	pixel += WIDTH - 1; // pointer to top right pixel
	for (int i = 0; i < lives; i++) {
		pixel->ascii = '<';
		pixel->color = GREEN;
		pixel--;
		pixel->ascii = ' ';
		pixel--;
	}

	// score
	pixel = canvas->pixels;
	pixel += (HEIGHT * WIDTH) - 1; // pointer to bottom right pixel
	if (score == 0) {
		pixel->ascii = '0';
		pixel->color = GREEN;
	}
	while (score > 0) {
		pixel->ascii = (char)(score % 10 + 48);
		pixel->color = GREEN;
		pixel--;
		score /= 10;
	}

	// hiscore
	char buf[MAX_TEXT_LEN];
	snprintf(buf, MAX_TEXT_LEN, "%d", hiscore);
	draw_text(buf, HEIGHT - 1, 0, GREEN, canvas); // bottom left
}

void game_draw(struct game *game, struct canvas *canvas)
{
	if (!game || !canvas)
		return;

	struct sprite *ship = &game->ship;
	struct sprite *bullet = &game->bullet;
	struct sprite *aliens = game->aliens;

	switch (game->state) {
	case TITLE:
		print_title_screen();
		break;
	case PLAY:
		draw_hud(game, canvas);
		sprite_draw(ship, canvas);
		sprite_draw(bullet, canvas);
		for (int i = 0; i < NUM_ALIENS; i++) {
			sprite_draw(&aliens[i], canvas);
		}
		break;
	case WAVE_CLEARED:
		draw_hud(game, canvas);
		draw_text("Sector cleared", 10, 31, WHITE, canvas);
		break;
	case GAME_OVER:
		draw_hud(game, canvas);
		//draw_text("Game Over", 10, 35, WHITE, canvas);
		break;
	}
}

void game_init_wave(struct game *game)
{
	if (!game)
		return;

	struct sprite *ship = &game->ship;
	struct sprite *bullet = &game->bullet;
	struct sprite *aliens = game->aliens;
	float alien_speed = 15.0 + 1 * game->wave;

	srand(time(NULL));

	ship->x = WIDTH / 2 - 3;
	ship->y = HEIGHT - 2;
	ship->color = YELLOW;
	ship->velocity = 20.0;
	ship->alive = true;
	strncpy(ship->ascii, "/-I-\\", MAX_SPRITE_WIDTH);
	ship->width = strlen(ship->ascii);
	ship->height = 1;

	bullet->x = 0;
	bullet->y = 0;
	bullet->color = WHITE;
	bullet->velocity = 50.0;
	bullet->alive = false;
	strncpy(bullet->ascii, "|", MAX_SPRITE_WIDTH);
	bullet->width = strlen(bullet->ascii);
	bullet->height = 1;

	for (int i = 0; i < NUM_ALIENS; i++) {
		struct sprite *alien = &aliens[i];

		alien->color = rand() % NUM_COLORS;
		alien->velocity = alien_speed;
		alien->alive = true;
		strncpy(alien->ascii, "vQv", MAX_SPRITE_WIDTH);
		alien->width = strlen(aliens->ascii);
		alien->height = 1;
		alien->x = 1 + (aliens->width + 2) * i;
		alien->y = 1;
		alien->direction = RIGHT;
	}
}
