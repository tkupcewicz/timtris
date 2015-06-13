#ifndef INCLUDED_BLOCK_H
#define INCLUDED_BLOCK_H

typedef struct {
	int is_locked;
	int color;
	int x, y;
	int dir;
	int type;
	int data;
} Element;

Element *generate_block();
void rotate_block(Element *element);
void moveBlock(Element *activeBlock, int dx, int dy);
void lockBlock(Element *activeBlock, Element ***tab);
//void drawBlock(Element *activeBlock, Element ***tab);
#endif