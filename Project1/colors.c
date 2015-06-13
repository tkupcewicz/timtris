#include "colors.h"
#include "playfield.h"
#include "block.h"
#include <allegro5\allegro.h>

ALLEGRO_BITMAP *brick_grey = NULL, *brick_purple = NULL, *brick_pink = NULL, *brick_blue = NULL, *brick_yellow = NULL, *brick_red = NULL, *brick_green = NULL, *brick_orange = NULL;

bool loadBitmaps(){
	brick_grey = al_load_bitmap("brick_grey.png");
	brick_purple = al_load_bitmap("brick_purple.png");
	brick_pink = al_load_bitmap("brick_pink.png");
	brick_blue = al_load_bitmap("brick_blue.png");
	brick_yellow = al_load_bitmap("brick_yellow.png");
	brick_red = al_load_bitmap("brick_red.png");
	brick_green = al_load_bitmap("brick_green.png");
	brick_orange = al_load_bitmap("brick_orange.png");

	if (!brick_grey || !brick_purple || !brick_pink || !brick_blue || !brick_yellow || !brick_red || !brick_green || !brick_orange) {
		fprintf(stderr, "failed to load one of brick images!\n");
		return false;
	}
	return true;
}