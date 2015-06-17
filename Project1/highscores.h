#ifndef INCLUDED_HIGHSCORES_H
#define INCLUDED_HIGHSCORES_H

typedef struct SCORE Score;
struct SCORE{
	int score;
	struct Score * next;
};
//dodawanie elementow do listy
Score * AddElement(Score *head, int x);
//usuwanie listy
Score * deleteList(Score *head);
//wpisywanie listy do pliku
Score * printListIntoFile(Score *head);
//czytanie listy z pliku
Score * readListFromFile(Score *head);
#endif