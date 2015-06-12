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

void generate_block(Element  *element);
void rotate_block(Element *element);

#endif