#ifndef CANVAS_H
#define CANVAS_H

#define WIDTH 80
#define HEIGHT 25

struct pixel {
	char ascii;
	int color;
};

struct canvas {
	struct pixel pixels[HEIGHT * WIDTH];
};

struct canvas *new_canvas(void);
void delete_canvas(struct canvas *canvas);
void canvas_clear(struct canvas *canvas);
void canvas_print(struct canvas *canvas);

#endif // CANVAS_H
