#include <stdio.h>
#include <allegro5/allegro.h>
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_native_dialog.h"
#include <time.h>
#include <stdbool.h>
#include "block.h"


Element **mainMatrix;

static int block_data[7][4] = {
	{ 0xF00, 0x2222, 0xF00, 0x2222 }, // I
	{ 0x660, 0x660, 0x660, 0x660 }, // O
	{ 0x720, 0x2320, 0x2700, 0x2620 }, // T
	{ 0x360, 0x2310, 0x360, 0x2310 }, // S
	{ 0x630, 0x1320, 0x630, 0x1320 }, // Z
	{ 0x710, 0x3220, 0x4700, 0x2260 }, // J
	{ 0x740, 0x2230, 0x1700, 0x6220 }  // L
};
//
//
//		TO DO:
//		
//		
//
//
//
//
//

bool is_collision(Element *element, int dx, int dy){
	int k, m, i, j;
	k = 4;
	for (i = element->y - 1; i <= element->y + 2; i++){
		m = 1;
		for (j = element->x - 2; j <= element->x + 1; j++){
			if (block_data[element->type][element->dir] & 1 << k * 4 - m){
				//printf("1");
				if (mainMatrix[i + dy][j + dx].is_locked == 1){
					//printf("%d %d is locked\n", i + dy, j + dx);
					return true;
				}
			}
			//else printf("0");
			m++;
		}
		//printf("\n");
		k--;
	}
	return false;
} 

void moveBrick(Element *activeBlock, int dx, int dy){
	int col = mainMatrix[activeBlock->y][activeBlock->x].color;
	mainMatrix[activeBlock->y][activeBlock->x].color = 0;
	mainMatrix[activeBlock->y][activeBlock->x].is_locked = 0;
	mainMatrix[activeBlock->y + dy][activeBlock->x + dx].color = col;
	mainMatrix[activeBlock->y + dy][activeBlock->x + dx].is_locked = 0;
	activeBlock->color = col;
	activeBlock->x = activeBlock->x + dx;
	activeBlock->y = activeBlock->y + dy;
}

int checkForLines(){
	int i, j, k, m, brickCounter, lineCounter;
	lineCounter = 0;
	for (i = 19; i >= 0; i--){
		brickCounter = 0;
		for (j = 1; j <= 10; j++){
			if (mainMatrix[i][j].is_locked == 1){
				brickCounter++;
			}
		}
		if (brickCounter == 10){
			for (j = 1; j <= 10; j++){
				mainMatrix[i][j].is_locked = 0;
				mainMatrix[i][j].color = 0;
			}
			lineCounter++;
			for (k = i; k >= 1; k--){
				for (m = 1; m <= 10; m++){
					mainMatrix[k][m].color = mainMatrix[k - 1][m].color;
					mainMatrix[k][m].is_locked = mainMatrix[k - 1][m].is_locked;
				}
			}
			i++;
		}
		
	}
	return lineCounter;
}

Element  *generateBrick(){
	Element * tmp;
	mainMatrix[1][5].type = rand() % 7;
	mainMatrix[1][5].color = mainMatrix[1][5].type  + 1;
	//printf("%d", mainMatrix[1][5].color);
	mainMatrix[1][5].dir = rand() % 4;
	mainMatrix[1][5].is_locked = 0;
	mainMatrix[1][5].x = 5;
	mainMatrix[1][5].y = 1;
	tmp = &mainMatrix[1][5];
	return tmp;
}

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
	mainMatrix = (Element**)calloc(21, sizeof(Element*));
	for (i = 0; i < 21; i++){
		mainMatrix[i] = (Element*)calloc(12, sizeof(Element));
	}
	for (i = 0; i < 21; i++){
		for (j = 0; j < 12; j++){
			if ((i == 20) || (j == 0 || j == 11)){
				mainMatrix[i][j].color = -1;
				mainMatrix[i][j].is_locked = 1;
				mainMatrix[i][j].x = j;
				mainMatrix[i][j].y = i;
			}
		}
	}
	/*int k, m;
	for (i = 0; i < 7; i++){
		for (j = 0; j < 4; j++){
			for (k = 4; k > 0; k--){
				for (m = 1; m <= 4; m++){
					if (block_data[i][j] & 1 << k * 4 - m) printf("1 ");
					else printf("0 ");
				}
				printf("\n");
			}
			printf("\n");
		}
	}*/


	activeBlock = generateBrick();
	al_clear_to_color(al_map_rgb(0, 0, 0));
	bool downKeyDown = false;
	bool rightKeyDown = false;
	int keyDelay = 10;
	int x, k, m, test;
	while (true){
		//printf("%d %d\n", activeBlock->x, activeBlock->y);
		//printf("COLOR: %d\n", activeBlock->color);
		ALLEGRO_EVENT ev;
		al_get_next_event(event_queue, &ev);
		if (ev.type == ALLEGRO_EVENT_KEY_DOWN){
			switch (ev.keyboard.keycode){
			case ALLEGRO_KEY_LEFT:
				if (!is_collision(activeBlock, -1, 0)){
					moveBrick(activeBlock, -1, 0);
				}
				break;
			case ALLEGRO_KEY_RIGHT:
				if (!is_collision(activeBlock, 1, 0)){
					moveBrick(activeBlock, 1, 0);
				}
				break;
			case ALLEGRO_KEY_UP:
				x = activeBlock->dir;
				activeBlock->dir = (x + 1) % 4;
				if ((is_collision(activeBlock, 0, 0)) || (is_collision(activeBlock, 0, 0))) activeBlock->dir = x;
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
		else NUM_SECONDS = 0.5;
		this_time = clock();
		time_counter += (double)(this_time - last_time);
		last_time = this_time;

		if (time_counter > (double)(NUM_SECONDS * CLOCKS_PER_SEC))
		{
			time_counter -= (double)(NUM_SECONDS * CLOCKS_PER_SEC);

			if (!is_collision(activeBlock, 0, 1)){
				moveBrick(activeBlock, 0, 1);
			}
			else{
				k = 4;
				for (i = activeBlock->y - 1; i <= activeBlock->y + 2; i++){
					m = 1;
					for (j = activeBlock->x - 2; j <= activeBlock->x + 1; j++){
						if (block_data[activeBlock->type][activeBlock->dir] & 1 << k * 4 - m){
							mainMatrix[i][j].is_locked = 1;
							mainMatrix[i][j].color = activeBlock->color;
						}
						m++;
					}
					k--;
				}
				downKeyDown = false;
				test = checkForLines();
				activeBlock = generateBrick();
				if (is_collision(activeBlock, 0, 0)){
					printf("END GAME\n");
				}
				//lockedMove = 1;
			}
			//else lockedMove = 0;
		
		}

		al_clear_to_color(al_map_rgb(0, 0, 0));
		for (i = 0; i < 21; i++){
			for (j = 0; j < 12; j++){
				switch (mainMatrix[i][j].color){
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
				if (block_data[activeBlock->type][activeBlock->dir] & 1 << k * 4 - m){
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
		
		al_flip_display();
	}
	return 0;
}