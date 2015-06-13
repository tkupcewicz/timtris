#ifndef INCLUDED_PLAYFIELD_H
#define INCLUDED_PLAYFIELD_H

#include<stdbool.h>
#include"block.h"

int checkForLines(Element ***tab);
void drawActiveBlock(Element *block);
void drawPlayfield(Element ***tab);
#endif