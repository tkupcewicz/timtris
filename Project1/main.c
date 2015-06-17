#include <stdio.h>
#include <allegro5/allegro.h>
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_native_dialog.h"
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <time.h>
#include <stdbool.h>
#include "block.h"
#include "playfield.h"
#include "highscores.h"
#include "colors.h"

//glowna macierz gry
Element ***mainMatrix;
extern Score * Head;
int mainScore;

int NWD(int a, int b)
{
	if (b != 0)
		return NWD(b, a%b);
	return a;
}

enum MYKEYS {
	KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT
};

void printList(Score *head){
	Score *tmp;
	tmp = head;
	printf("\nLISTA:\n");
	while (tmp != NULL){
		printf("%d\n", tmp->score);
		tmp = tmp->next;
	}
}

void restartGame(Element *active, Element* next){
	int i, j;
	for (i = 0; i < 21; i++){
		for (j = 0; j < 12; j++){
			free(mainMatrix[i][j]);
		}
	}
	mainScore = 0;

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
}

int main(int argc, char **argv)
{
	
	float NUM_SECONDS;
	float speed;
	Score * scoreTmp;
	FILE *config;
	bool newHighscore;

	fopen_s(&config, "config.txt", "r+");

	fscanf_s(config, "%f", &speed);

	fclose(config);

	Head = readListFromFile(Head);
	newHighscore = false;
	NUM_SECONDS = speed;

	srand(time(NULL));
	
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	
	ALLEGRO_DISPLAY *display = NULL;

	al_init_font_addon(); // initialize the font addon
	al_init_ttf_addon();// initialize the ttf (True Type Font) addon
	
	if (!al_init()) {
		fprintf(stderr, "Failed to initialize allegro!\n");
		return 0;
	}

	if (!al_init_image_addon()) {
		fprintf(stderr, "Failed to initialize image addon!\n");
		return 0;
	}
	//Deklarowanie rozmiaru okna
	display = al_create_display(640, 672);
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

	ALLEGRO_FONT *font = al_load_ttf_font("prstart.ttf", 20, 0);

	if (!font){
		fprintf(stderr, "Could not load 'prstart.ttf'.\n");
		return -1;
	}

	//Ladowanie bitmap
	if (loadBitmaps() == false){
		al_destroy_display(display);
		return 0;
	}

	Element *activeBlock;
	Element *nextBlock;
	double time_counter = 0;

	clock_t this_time = clock();
	clock_t last_time = this_time;

	int i, j, tempScore, mainScore = 0;
	char *highscoresText = "Highscores:";
	char *nextText = "Next block:";
	char *scoreText = "Score:";
	char *newHighScoreText = "New high score!!!";
	char scoreNumberText[20];

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

	nextBlock = generateBlock();
	activeBlock = nextBlock;
	activeBlock->x = 5;
	activeBlock->y = 1;
	nextBlock = generateBlock();

	al_clear_to_color(al_map_rgb(0, 0, 0));
	bool downKeyDown = false;
	bool rightKeyDown = false;
	int keyDelay = 10;
	int x, k, m, test;
	bool gameOver = false;
	ALLEGRO_EVENT ev;
	// G³ówna pêtla gry

	while (true){

		// Pobieranie informacji o wcisnietych klawiszach

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
				break;
			case ALLEGRO_KEY_SPACE:
				if (gameOver){
					restartGame(activeBlock, nextBlock);
					free(activeBlock);
					free(nextBlock);
					nextBlock = generateBlock();
					activeBlock = nextBlock;
					activeBlock->x = 5;
					activeBlock->y = 1;
					nextBlock = generateBlock();
					time_counter = 0;
					mainScore = 0;
					newHighscore = false;
					this_time = clock();
					last_time = this_time;
					gameOver = false;
				}
			}
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_UP){
			switch (ev.keyboard.keycode){

			//Strzalka w dol - zmiana zmiennej downKeyDown na stan falszywy
			case ALLEGRO_KEY_DOWN:
				downKeyDown = false;
				break;
			}
		}

		//Jesli strzalka w dol jest wcisnieta to klocek porusza sie co 0.05sek, w przeciwnym wypadku 0.5s
		if (downKeyDown) NUM_SECONDS = 0.05;
		else NUM_SECONDS = speed;
		if (gameOver == false){
			//printf("DODAJE\n");
			this_time = clock();
			time_counter += (double)(this_time - last_time);
			last_time = this_time;
		}
		//Warunek wykonuj¹cy siê co czas okreœlony w NUM_SECONDS
		if (time_counter > (double)(NUM_SECONDS * CLOCKS_PER_SEC) && gameOver == false)
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
				tempScore = checkForLines(mainMatrix);
				switch (tempScore){
				case 1: mainScore = mainScore + 40; break;
				case 2: mainScore = mainScore + 100; break;
				case 3: mainScore = mainScore + 300; break;
				case 4: mainScore = mainScore + 1200; break;
				}
				//ZWALNIANIE PAMIECI!
				free(activeBlock);
				activeBlock = nextBlock;
				activeBlock->x = 5;
				activeBlock->y = 1;
				nextBlock = generateBlock();
				if (is_collision(activeBlock, 0, 0, mainMatrix)){
					printf("END GAME\n");
					if (mainScore > Head->score) newHighscore = true;
					Head = AddElement(Head, mainScore);
					Head = printListIntoFile(Head);
					gameOver = true;
				}
			}
		}
		if (gameOver == false){
			//printf("Powinno rysowac playground\n");
			//Czysci wyswietlacz na czarno
			al_clear_to_color(al_map_rgb(0, 0, 0));
			//Rysuje cala macierz
		drawPlayfield(mainMatrix);
			//Dorysowuje aktywny klocek
			drawActiveBlock(activeBlock);
	
			drawActiveBlock(nextBlock);

			sprintf_s(scoreNumberText, 20, "%d", mainScore);

			al_draw_text(font, al_map_rgb(255, 255, 255), 394, 220, ALLEGRO_ALIGN_LEFT, scoreNumberText);
			al_draw_text(font, al_map_rgb(255, 255, 255), 394, 198, ALLEGRO_ALIGN_LEFT, scoreText);
			al_draw_text(font, al_map_rgb(255, 255, 255), 394, 10, ALLEGRO_ALIGN_LEFT, nextText);
			al_flip_display();
		}
		else{
			//printf("Powinno rysowac endscreen\n");
			al_clear_to_color(al_map_rgb(0, 0, 0));
			al_draw_text(font, al_map_rgb(255, 255, 255), 640 / 2, 100, ALLEGRO_ALIGN_CENTER, "GAME OVER");
			if (newHighscore)
				al_draw_text(font, al_map_rgb(255, 0, 0), 640 / 2, 140, ALLEGRO_ALIGN_CENTER, newHighScoreText);
			al_draw_text(font, al_map_rgb(255, 255, 255), 320, 200, ALLEGRO_ALIGN_CENTER, highscoresText);
			scoreTmp = Head;
			for (i = 1; i <= 5; i++){
				sprintf_s(scoreNumberText, 20, "%d. %d", i, scoreTmp->score);
				scoreTmp = scoreTmp->next;
				al_draw_text(font, al_map_rgb(255, 255, 255), 320, 200 + i * 30, ALLEGRO_ALIGN_CENTER, scoreNumberText);
			}
			al_draw_text(font, al_map_rgb(255, 255, 255), 640 / 2, 500, ALLEGRO_ALIGN_CENTER, "Press (space) to start");
			al_flip_display();
		}
	}
	return 0;
}