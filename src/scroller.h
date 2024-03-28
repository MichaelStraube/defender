#ifndef SCROLLER_H
#define SCROLLER_H

#include "canvas.h"

#define MAX_SCROLLER_TEXT_LEN 200
#define SCROLLER_LEN MAX_SCROLLER_TEXT_LEN

struct scroller {
	float x;
	float y;
	float speed;
	int color;
	char buf[SCROLLER_LEN];
	int spacer; // controls when we wrap x pos
};

struct scroller *new_scroller(const char *text, float y, int color, float speed);
void delete_scroller(struct scroller *s);
void scroller_update(struct scroller *s, float ftime);
void scroller_draw(struct scroller *s, struct canvas *canvas);

#endif // SCROLLER_H
