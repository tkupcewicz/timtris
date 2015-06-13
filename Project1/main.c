#include <stdio.h>
#include <allegro5/allegro.h>
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_native_dialog.h"
#include <time.h>
#include <stdbool.h>
#include "block.h"
#include "playfield.h"
#include "colors.h"

//glowna macierz gry
Element ***mainMatrix;

enum MYKEYS {
	KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT
};

int main(int argc, char **argv)
{	
	FILE *fp;
	fopen_s(&fp, "config.txt", "w+");
	fclose(fp);
	float NUM_SECONDS = 0.5;
	srand(time(NULL));
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_DISPLAY *display = NULL;
	
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

	//Ladowanie bitmap
	if (loadBitmaps() == false){
		al_destroy_display(display);
		return 0;
	}

	Element *activeBlock;
	double time_counter = 0;

	clock_t this_time = clock();
	clock_t last_time = this_time;

	int i, j;

	//Alokowanie pamieci na tablice

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

	// G³ówna pêtla gry

	while (true){

		// Pobieranie informacji o wcisnietych klawiszach

		ALLEGRO_EVENT ev;
		al_get_next_event(event_queue, &ev);
		if (ev.type == ALLEGRO_EVENT_KEY_DOWN){
			switch (ev.keyboard.keycode){

			//Strzalka w lewo
			case ALLEGRO_KEY_LEFT:
				if (!is_collision(activeBlock, -1, 0, mainMatrix)){
					moveBlock(activeBlock, -1, 0);
				}
				break;

			//Strzalka w prawo
			case ALLEGRO_KEY_RIGHT:
				if (!is_collision(activeBlock, 1, 0, mainMatrix)){
					moveBlock(activeBlock, 1, 0);
				}
				break;

			//Strzalka w gore - obracanie klocka
			case ALLEGRO_KEY_UP:
				x = activeBlock->dir;
				//zmiana kierunku
				activeBlock->dir = (x + 1) % 4;
				//jesli powstala kolizja - powrot do stanu wczesniejszego
				if (is_collision(activeBlock, 0, 0, mainMatrix)) activeBlock->dir = x;
				break;
			
			//Strzalka w dol - zmiana zmiennej downKeyDown na stan prawdziwy
			case ALLEGRO_KEY_DOWN:
				time_counter = 0.05;
				downKeyDown = true;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_UP){
			switch (ev.keyboard.keycode){

			//Strzalka w dol - zmiana zmiennej downKeyDown na stan falszywy
			case ALLEGRO_KEY_DOWN:
				downKeyDown = false;
			}
		}

		//Jesli strzalka w dol jest wcisnieta to klocek porusza sie co 0.05sek, w przeciwnym wypadku 0.5s
		if (downKeyDown) NUM_SECONDS = 0.05;
		else NUM_SECONDS = 0.5;
		this_time = clock();
		time_counter += (double)(this_time - last_time);
		last_time = this_time;
		//Warunek wykonuj¹cy siê co czas okreœlony w NUM_SECONDS
		if (time_counter > (double)(NUM_SECONDS * CLOCKS_PER_SEC))
		{
			time_counter -= (double)(NUM_SECONDS * CLOCKS_PER_SEC);
			//Jesli nie ma kolizji miedzy aktywnym klockiem, w dolnym kierunku przesuwa go
			if (!is_collision(activeBlock, 0, 1, mainMatrix)){
				moveBlock(activeBlock, 0, 1);
			}
			//Jesli kolizja jest, blokuje go, i tworzy nowy klocek. Jesli w momencie stworzenia nowego klocka wystepuje kolizja oznacza to koniec gry
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
		//Czysci wyswietlacz na czarno
		al_clear_to_color(al_map_rgb(0, 0, 0));
		//Rysuje cala macierz
		drawPlayfield(mainMatrix);
		//Dorysowuje aktywny klocek
		drawActiveBlock(activeBlock);
		
		al_flip_display();
	}
	return 0;
}