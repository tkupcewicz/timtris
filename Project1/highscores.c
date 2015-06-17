#include "highscores.h"
#include <stdio.h>
#include <allegro5\allegro.h>


FILE * highscores;
typedef struct SCORE Score;
Score *Head;

void printList2(Score *head){
	Score *tmp;
	tmp = head;
	printf("\nLISTA:\n");
	while (tmp != NULL){
		printf("%d\n", tmp->score);
		tmp = tmp->next;
	}
}

Score * AddElement(Score *head, int x){

	if (head == NULL){
		//printf("Tworze head\n");
		head = malloc(sizeof(Score));
		head->score = x;
		head->next = NULL;
		return head;
	}
	Score *newScore;
	newScore = malloc(sizeof(Score));
	newScore->score = x;
	newScore->next = NULL;

	if (x > head->score){
		//printf("Dodaje na poczatek\n");
		newScore->next = head;
		return newScore;
	}

	Score *temp, *prev;
	temp = head->next;
	prev = head;
	while (temp != NULL && temp->score > x) {
		prev = temp;
		temp = temp->next;
	}
	//printf("Dodaje w srodku\n");
	newScore->next = temp;
	prev->next = newScore;
	return head;
}

Score * deleteList(Score *head){
	Score *tmp;
	while (head != NULL){
		tmp = head;
		head = head->next;
		free(tmp);
	}
	head = NULL;
	return head;
}


Score * printListIntoFile(Score *head){
	fopen_s(&highscores, "highscores.txt", "w+");
	//printf("Opened file\n");
	Score *tmp;
	tmp = head;
	while (tmp != NULL){
		fprintf(highscores, "%d\n", tmp->score);
		tmp = tmp->next;
	}
	fclose(highscores);
	return head;
}

Score * readListFromFile(Score *head){
	deleteList(head);
	int x = 0;
	fopen_s(&highscores, "highscores.txt", "r+");
	while (fscanf_s(highscores, "%d", &x) != EOF){
		//printf("Scanned: %d\n", x);
		head = AddElement(head, x);
	}
	fclose(highscores);
	return head;
}

