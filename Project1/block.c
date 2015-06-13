#include"block.h"
#include"playfield.h"

int block_data[7][4] = {
	{ 0xF00, 0x2222, 0xF00, 0x2222 }, // I
	{ 0x660, 0x660, 0x660, 0x660 }, // O
	{ 0x720, 0x2320, 0x2700, 0x2620 }, // T
	{ 0x360, 0x2310, 0x360, 0x2310 }, // S
	{ 0x630, 0x1320, 0x630, 0x1320 }, // Z
	{ 0x710, 0x3220, 0x4700, 0x2260 }, // J
	{ 0x740, 0x2230, 0x1700, 0x6220 }  // L
};

void moveBlock(Element *activeBlock, int dx, int dy){
	activeBlock->x = activeBlock->x + dx;
	activeBlock->y = activeBlock->y + dy;
}

Element  *generateBlock(){
	Element * tmp;
	tmp = malloc(sizeof(Element));
	tmp->x = 5;
	tmp->y = 1;
	tmp->type = rand() % 7;
	tmp->color = tmp->type + 1;
	tmp->dir = rand() % 4;
	tmp->is_locked = 0;
	return tmp;
}

bool is_collision(Element *element, int dx, int dy, Element ***tab){
	int k, m, i, j;
	k = 4;
	for (i = element->y - 1; i <= element->y + 2; i++){
		m = 1;
		for (j = element->x - 2; j <= element->x + 1; j++){
			if (block_data[element->type][element->dir] & 1 << ((k * 4) - m)){
				if (tab[i + dy][j + dx]->is_locked == 1){
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

void lockBlock(Element *activeBlock, Element ***tab){
	int k, m, i, j;
	k = 4;
	for (i = activeBlock->y - 1; i <= activeBlock->y + 2; i++){
		m = 1;
		for (j = activeBlock->x - 2; j <= activeBlock->x + 1; j++){
			if (block_data[activeBlock->type][activeBlock->dir] & 1 << ((k * 4) - m)){
				tab[i][j]->is_locked = 1;
				tab[i][j]->color = activeBlock->color;
			}
			m++;
		}
		k--;
	}
}

/*void drawBlock(Element *activeBlock, Element ***tab){
	int i, j, k, m;
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
}*/