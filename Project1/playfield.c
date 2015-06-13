#include"playfield.h"
#include"block.h"
#include<stdbool.h>

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

