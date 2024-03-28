#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "scroller.h"

struct scroller *new_scroller(const char *text, float y, int color, float speed)
{
	struct scroller *s = malloc(sizeof(*s));
	if (!s)
		return NULL;

	s->x = WIDTH;
	s->y = y;
	s->color = color;
	s->speed = speed;
	s->spacer = WIDTH / 2;
	snprintf(s->buf, SCROLLER_LEN, text);

	return s;
}

void delete_scroller(struct scroller *s)
{
	free(s);
}

void scroller_update(struct scroller *s, float ftime)
{
	if (!s)
		return;

	s->x -= s->speed * ftime;
	if (s->x < -((int)strlen(s->buf) + s->spacer)) {
		s->x = WIDTH;
	}
}

void scroller_draw(struct scroller *s, struct canvas *canvas)
{
	if (!s)
		return;

	char *read = s->buf;
	struct pixel *pixel = canvas->pixels;
	int x = s->x;
	int y = s->y;

	pixel += y * WIDTH + x;

	// clipping
	if (y < 0 || y >= HEIGHT || x >= WIDTH)
		return;
	if (x < 0) {
		read += -x;
		pixel += -x;
		x = 0;
	}

	// FIXME: reads to far if text is longer than SCROLLER_LEN ?
	while (x < WIDTH && *read) {
		pixel->ascii = *read++;
		pixel->color = s->color;
		pixel++;
		x++;
	}
}
