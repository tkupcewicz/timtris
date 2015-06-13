#ifndef INCLUDED_PLAYFIELD_H
#define INCLUDED_PLAYFIELD_H
#include<stdbool.h>
#include"block.h"

int checkForLines(Element ***tab);
bool is_collision(Element *element, int dx, int dy, int ***tab);

#endif