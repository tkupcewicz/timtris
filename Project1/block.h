#ifndef INCLUDED_BLOCK_H
#define INCLUDED_BLOCK_H
#include <stdbool.h>
typedef struct {
	int is_locked;
	int color;
	int x, y;
	int dir;
	int type;
	int data;
} Element;

//Generowanie nowego klocka
Element *generate_block();

//Poruszanie sie klockiem w kierunku x y
void moveBlock(Element *activeBlock, int dx, int dy);

//Sprawdzanie czy wystepuje kolizja w kierunku x y; x = 1 ->; x = -1 <-; y = 1 \/; y = -1 ^;
bool is_collision(Element *element, int dx, int dy, int ***tab);

//Blokuje wszystkie elementy aktywnego klocka
void lockBlock(Element *activeBlock, Element ***tab);
#endif