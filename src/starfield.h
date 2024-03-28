#ifndef STARFIELD_H
#define STARFIELD_H

#include <stdbool.h>
#include "canvas.h"

#define NUM_STARS 30
#define NUM_LAYERS 2

struct star {
	float x;
	float y;
	int layer;
};

struct starfield {
	struct star stars[NUM_STARS];
	bool animate;
};

struct starfield *new_starfield(void);
void delete_starfield(struct starfield *sf);
void starfield_update(struct starfield *sf, float ftime);
void starfield_draw(struct starfield *sf, struct canvas *canvas);

#endif // STARFIELD
