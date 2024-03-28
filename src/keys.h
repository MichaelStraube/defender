#ifndef KEYS_H
#define KEYS_H

#include "starfield.h"
#include "game.h"

void handle_keys_title(struct starfield *starfield, struct game *game, char c);
void handle_keys_play(struct starfield *starfield, struct game *game, char c);
void handle_keys_wave_cleared(struct starfield *starfield, struct game *game, char c);
void handle_keys_game_over(struct game *game, char c);
void handle_keys(struct starfield *starfield, struct game *game);

#endif // KEYS_H
