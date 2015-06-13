#include <stdio.h>
#include <allegro5/allegro.h>
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_native_dialog.h"
#include <time.h>
#include <stdbool.h>
#include "block.h"
#include "playfield.h"

Element ***mainMatrix;
extern int block_data[7][4];


/*
bool is_collision(Element *element, int dx, int dy){
	int k, m, i, j;
	k = 4;
	for (i = element->y - 1; i <= element->y + 2; i++){
		m = 1;
		for (j = element->x - 2; j <= element->x + 1; j++){
			if (block_data[element->type][element->dir] & 1 << ((k * 4) - m)){
				if (mainMatrix[i + dy][j + dx]->is_locked == 1){
					//printf("%d %d is locked\n", i + dy, j + dx);
					return true;
				}
			}
			m++;
		}
		k--;
	}
	return false;
}
*/

enum MYKEYS {
	KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT
};

int main(int argc, char **argv)
{	
	float NUM_SECONDS = 0.5;
	srand(time(NULL));
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_BITMAP *brick_grey = NULL, *brick_purple = NULL, *brick_pink = NULL, *brick_blue = NULL, *brick_yellow = NULL, *brick_red = NULL, *brick_green = NULL, *brick_orange = NULL;
	
	
	if (!al_init()) {
		fprintf(stderr, "Failed to initialize allegro!\n");
		return 0;
	}

	if (!al_init_image_addon()) {
		fprintf(stderr, "Failed to initialize image addon!\n");
		return 0;
	}

	display = al_create_display(800, 672);
	if (!display) {
		fprintf(stderr, "Failed to create display!\n");
		return 0;
	}

	if (!al_install_keyboard()) {
		fprintf(stderr, "failed to initialize the keyboard!\n");
		return -1;
	}

	event_queue = al_create_event_queue();
	if (!event_queue) {
		fprintf(stderr, "failed to create event_queue!\n");
		al_destroy_display(display);
		return -1;
	}
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_keyboard_event_source());

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
		al_destroy_display(display);
		return 0;
	}


	Element *activeBlock;
	double time_counter = 0;

	clock_t this_time = clock();
	clock_t last_time = this_time;

	int i, j;
	mainMatrix = (Element***)calloc(21, sizeof(Element**));
	for (i = 0; i < 21; i++){
		mainMatrix[i] = (Element**)calloc(12, sizeof(Element*));
	}
	for (i = 0; i < 21; i++){
		for (j = 0; j < 12; j++){
			mainMatrix[i][j] = (Element*)calloc(1, sizeof(Element));
		}
	}
	for (i = 0; i < 21; i++){
		for (j = 0; j < 12; j++){
			if ((i == 20) || (j == 0 || j == 11)){
				mainMatrix[i][j]->color = -1;
				mainMatrix[i][j]->is_locked = 1;
				mainMatrix[i][j]->x = j;
				mainMatrix[i][j]->y = i;
			}
		}
	}


	activeBlock = generateBlock();
	al_clear_to_color(al_map_rgb(0, 0, 0));
	bool downKeyDown = false;
	bool rightKeyDown = false;
	int keyDelay = 10;
	int x, k, m, test;
	while (true){
		ALLEGRO_EVENT ev;
		al_get_next_event(event_queue, &ev);
		if (ev.type == ALLEGRO_EVENT_KEY_DOWN){
			switch (ev.keyboard.keycode){
			case ALLEGRO_KEY_LEFT:
				if (!is_collision(activeBlock, -1, 0, mainMatrix)){
					moveBlock(activeBlock, -1, 0);
				}
				break;
			case ALLEGRO_KEY_RIGHT:
				if (!is_collision(activeBlock, 1, 0, mainMatrix)){
					moveBlock(activeBlock, 1, 0);
				}
				break;
			case ALLEGRO_KEY_UP:
				x = activeBlock->dir;
				activeBlock->dir = (x + 1) % 4;
				if ((is_collision(activeBlock, 0, 0, mainMatrix)) || (is_collision(activeBlock, 0, 0, mainMatrix))) activeBlock->dir = x;
				break;
			case ALLEGRO_KEY_DOWN:
				downKeyDown = true;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_UP){
			switch (ev.keyboard.keycode){
			case ALLEGRO_KEY_DOWN:
				downKeyDown = false;
			}
		}


		if (downKeyDown) NUM_SECONDS = 0.1;
		else NUM_SECONDS = 1;
		this_time = clock();
		time_counter += (double)(this_time - last_time);
		last_time = this_time;

		if (time_counter > (double)(NUM_SECONDS * CLOCKS_PER_SEC))
		{
			time_counter -= (double)(NUM_SECONDS * CLOCKS_PER_SEC);
			if (!is_collision(activeBlock, 0, 1, mainMatrix)){
				moveBlock(activeBlock, 0, 1);
			}
			else{
				lockBlock(activeBlock, mainMatrix);
				downKeyDown = false;
				test = checkForLines(mainMatrix);
				activeBlock = generateBlock();
				if (is_collision(activeBlock, 0, 0, mainMatrix)){
					printf("END GAME\n");
				}
			}
		}

		al_clear_to_color(al_map_rgb(0, 0, 0));
		for (i = 0; i < 21; i++){
			for (j = 0; j < 12; j++){
				switch (mainMatrix[i][j]->color){
				case 1:  al_draw_bitmap(brick_purple, j * 32, i * 32, 0); break;
				case 2:  al_draw_bitmap(brick_pink, j * 32, i * 32, 0); break;
				case 3:  al_draw_bitmap(brick_blue, j * 32, i * 32, 0); break;
				case 4:  al_draw_bitmap(brick_yellow, j * 32, i * 32, 0); break;
				case 5:  al_draw_bitmap(brick_red, j * 32, i * 32, 0); break;
				case 6:  al_draw_bitmap(brick_green, j * 32, i * 32, 0); break;
				case 7:  al_draw_bitmap(brick_orange, j * 32, i * 32, 0); break;
				case -1:  al_draw_bitmap(brick_grey, j * 32, i * 32, 0); break;
				}
			}
		}
		bool test = false;

		k = 4;
		for (i = activeBlock->y - 1; i <= activeBlock->y + 2; i++){
			m = 1;
			for (j = activeBlock->x - 2; j <= activeBlock->x + 1; j++){
				if (block_data[activeBlock->type][activeBlock->dir] & 1 << ((k * 4) - m)){
					switch (activeBlock->color){
					case 1:  al_draw_bitmap(brick_purple, j * 32, i * 32, 0); break;
					case 2:  al_draw_bitmap(brick_pink, j * 32, i * 32, 0); break;
					case 3:  al_draw_bitmap(brick_blue, j * 32, i * 32, 0); break;
					case 4:  al_draw_bitmap(brick_yellow, j * 32, i * 32, 0); break;
					case 5:  al_draw_bitmap(brick_red, j * 32, i * 32, 0); break;
					case 6:  al_draw_bitmap(brick_green, j * 32, i * 32, 0); break;
					case 7:  al_draw_bitmap(brick_orange, j * 32, i * 32, 0); break;
					}
				}
				m++;
			}
			k--;
		}

		void drawBlock(activeBlock, mainMatrix);
		
		al_flip_display();
	}
	return 0;
}