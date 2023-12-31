#include "common.h"
#include "sc_color.h"
#include "sc_common.h"
#include "sc_painter.h"
#include "sc_sdlv1.h"
#include <SDL.h>
#include <math.h>
#include <stdio.h>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 320

void fancy_display(struct painter *painter) {
	static int current_cnt = 0, step = 1;
	struct point p;
	struct point size;
	uint32_t color;
	int i;

	painter_size(painter, &size);
	point_initialize(&p, size.x / 2, size.y / 2);
	for (i = 0; i < 31; i++) {
		color = (ABS(current_cnt - i) < 3) ? BLACK_24bit : CYAN_24bit;
		painter_draw_circle(painter, p, i, color);
	}
	painter_flush(painter);

	if (current_cnt == 31)
		step = -1;
	else if (current_cnt == 0)
		step = 1;

	current_cnt += step;

	SDL_Delay(10);
}

void graphic_play(struct painter *painter) {
	struct point p1, p2, p3, size;
	struct text_painter text_painter;

	painter_clear(painter, BLACK_24bit);

	/// text drawing
	text_painter_initialize(&text_painter, painter);

	color_pair_initialize(&text_painter.color, RED_24bit, BLACK_24bit);
	point_initialize(&text_painter.pos, 0, 0);

	text_draw_string(&text_painter, "1.5 Programming!", 32);

	color_pair_initialize(&text_painter.color, GREEN_24bit, BLACK_24bit);
	point_initialize(&text_painter.pos, 0, 32);

	text_draw_string(&text_painter, "1.5 Programming!", 16);

	painter_size(painter, &size);

	point_initialize(&p1, size.x / 2 - 50, size.y / 2 - 20);
	point_initialize(&p2, size.x / 2 + 50, size.y / 2 + 20);
	painter_draw_rectangle(painter, p1, p2, YELLOW_24bit);

	point_initialize(&p1, size.x / 2 - 50, size.y / 2 - 20);
	painter_draw_circle(painter, p1, 5, MAGENTA_24bit);

	point_initialize(&p1, 10, size.y / 2 - 20);
	point_initialize(&p2, 10, size.y / 2 + 20);
	painter_draw_line(painter, p1, p2, WHITE_24bit);

	point_initialize(&p1, 60, 60);
	point_initialize(&p2, 10, size.y - 60);
	point_initialize(&p3, size.x, -60);

	painter_draw_bezier(painter, p1, p2, p3, BLUE_24bit);

	painter_flush(painter);
}

int loop(SDL_Surface *surface);

int main() {
	SDL_Surface *surface;
	struct sdlv1_screen screen;
	struct painter painter;
	int r;

	r = SDL_Init(SDL_INIT_VIDEO);
	if (r < 0)
		exit_info(1, "failed initializing SDL: %s\n", SDL_GetError());

	SDL_WM_SetCaption("1.5 Programming", NULL);

	surface = SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 0, SDL_ANYFORMAT);
	if (surface == NULL)
		exit_info(2, "failed getting main display surface: %s\n", SDL_GetError());

	sdlv1_initialize(&screen, surface, WINDOW_WIDTH, WINDOW_HEIGHT);
	painter.drawing_board = (const struct drawing_i **)&screen;

	graphic_play(&painter);
	SDL_UpdateRect(surface, 0, 0, 0, 0);

	while (loop(surface))
		fancy_display(&painter);

	SDL_Quit();

	return 0;
}

int loop(SDL_Surface *surface) {
	SDL_Event ev;

	if (SDL_PollEvent(&ev)) {
		if (ev.type == SDL_QUIT)
			return 0;
	}

	SDL_UpdateRect(surface, 0, 0, 0, 0);

	return 1;
}
