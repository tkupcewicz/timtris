#ifndef INCLUDED_PLAYFIELD_H
#define INCLUDED_PLAYFIELD_H

#include<stdbool.h>
#include"block.h"

//sprawdza ile linii jest wypelnionych
int checkForLines(Element ***tab);
//rysuje blok active i next
void drawActiveBlock(Element *block);
//rysuje cala tablice
void drawPlayfield(Element ***tab);
#endif