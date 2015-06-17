#include "playfield.h"
#include "colors.h"
#include "block.h"
#include <allegro5\allegro.h>
#include <stdbool.h>

extern ALLEGRO_BITMAP *brick_grey, *brick_purple, *brick_pink, *brick_blue, *brick_yellow, *brick_red, *brick_green, *brick_orange;
extern int block_data[7][4];

int checkForLines(Element ***tab){
	int i, j, k, m, brickCounter, lineCounter;
	lineCounter = 0;
	for (i = 19; i >= 0; i--){
		brickCounter = 0;
		for (j = 1; j <= 10; j++){
			if (tab[i][j]->is_locked == 1){
				brickCounter++;
			}
		}
		if (brickCounter == 10){
			for (j = 1; j <= 10; j++){
				tab[i][j]->is_locked = 0;
				tab[i][j]->color = 0;
			}
			lineCounter++;
			for (k = i; k >= 1; k--){
				for (m = 1; m <= 10; m++){
					tab[k][m]->color = tab[k - 1][m]->color;
					tab[k][m]->is_locked = tab[k - 1][m]->is_locked;
				}
			}
			i++;
		}

	}
	return lineCounter;
}

void drawActiveBlock(Element *block){
	//printf("Draw activeBlock\n");
	int i, j, k, m;
	k = 4;
	//printf("%d %d\n", block->x, block->y);
	for (i = block->y - 1; i <= block->y + 2; i++){
		m = 1;
		for (j = block->x - 2; j <= block->x + 1; j++){
			if (block_data[block->type][block->dir] & 1 << ((k * 4) - m)){
				switch (block->color){
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
}

void drawNextBlock(Element *block){
	//printf("Draw activeBlock\n");
	int i, j, k, m;
	k = 4;
	//printf("%d %d\n", block->x, block->y);
	for (i = block->y - 1; i <= block->y + 2; i++){
		m = 1;
		for (j = block->x - 2; j <= block->x + 1; j++){
			if (block_data[block->type][block->dir] & 1 << ((k * 4) - m)){
				switch (block->color){
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
}





void drawPlayfield(Element ***tab){
	//printf("Draw playfield\n");
	int i, j;
	for (i = 0; i < 21; i++){
		for (j = 0; j < 12; j++){
			switch (tab[i][j]->color){
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
}

