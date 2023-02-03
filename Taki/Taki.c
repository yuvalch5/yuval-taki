#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define MAX_NAME 21
#define STARTING_SIZE 4
#define TOTAL_CARDS 9
#define TOTAL_TYPE_COLORS 4
#define CARD_COLOR_AND_NUMBER 3
#define CARD_NUMBER 1
#define CARD_COLOR 2

typedef struct
{
	int number;
	char color;
}CARD;

typedef struct
{
	char name[MAX_NAME];
	CARD cards;
}PLAYER;



void GetPlayersNames(PLAYER players[], int players_amount)
{
	int i;

	for (i = 0; i < players_amount; i++)
	{
		printf("\nPlease enter the first name of player #%d:\n", i + 1);
		scanf("%s", players[i].name);
	}
}

char convertColorIntToColor(int card_color_int)
{
	char card_color;

	if (card_color_int == 1)
	{
		card_color = 'R';
	}
	else if (card_color_int == 2)
	{
		card_color = 'B';
	}
	else if (card_color_int == 3)
	{
		card_color = 'G';
	}
	else if (card_color_int == 4)
	{
		card_color = 'Y';
	}

	return card_color;
}

void dealCards(PLAYER* players[], int players_amount)
{
	int i, j, card_number, card_color, card_color_int;

	for (i = 0; i < players_amount; i++)
	{
		for (j = 0; j < 4; j++)
		{
			card_number = (rand() % 9) + 1;
			card_color_int = (rand() % 4) + 1;

			card_color = convertColorIntToColor(card_color_int);

			players[i].cards[j].number = card_number;
			players[i].cards[j].color = card_color;
		}
	}
}





void printCard(int card_number, char card_color)
{
	int i, j;

	for (i = 0; i < 9; i++)
	{
		printf("*");
	}
	printf("\n");
	for (j = 0; j < 4; j++)
	{
		if (j == 1)
		{
			printf("*   %d   *\n", card_number);
		}
		else if (j == 2)
		{
			printf("*   %c   *\n", card_color);
		}
		else
		{
			printf("*       *\n");
		}
	}
	for (i = 0; i < 9; i++)
	{
		printf("*");
	}
}


CARD printTopCard()
{
	CARD top_card;
	int card_color_int;

	top_card.number = (rand() % 9) + 1;
	card_color_int = (rand() % 4) + 1;
	top_card.color = convertColorIntToColor(card_color_int);

	printf("\nThe top card is:\n\n");
	printCard(top_card.number, top_card.color);
	printf("\n\n");

	return top_card;
}



PLAYER* currentPlayer(PLAYER** current_player, int players_amount)
{
	if (*current_player + 1 == players_amount)
	{
		*current_player = 0;
		return *current_player;
	}
	else
	{
		*current_player = *current_player + 1;
		return *current_player;
	}
}

int printCurretPlayerCards(PLAYER players[], int players_amount, PLAYER* current_player)
{
    int i, card_number;
    char card_color;

    printf("Your cards:\n");
    printf("_______________________\n\n");

    for (i = 0; i < players_amount; i++)
    {
		card_number = current_player->cards.number;
		card_color = current_player->cards.color;

        printf("(%d)\n\n", i + 1);
        printCard(card_number, card_color);
        printf("\n\n");
    }
    return i;
}


void discardCard(PLAYER* pCurrentPlayer, int chosen_card, int max_card)
{
	int i;

	pCurrentPlayer->cards[chosen_card].
	pCurrentPlayer.cards[chosen_card].number = 0;

	for (i = chosen_card; i < max_card - 1; i++)
	{
		pCurrentPlayer.cards[i] = pCurrentPlayer.cards[i + 1];
	}

	pCurrentPlayer.cards[max_card - 1].color = '\0';
	pCurrentPlayer.cards[max_card - 1].number = 0;
}


bool playerHasChoices(PLAYER* pCurrentPlayer, int max_card, CARD top_card)
{
	if (max_card <= 0)
	{
		return false;
	}

	for (int i = 0; i < max_card; i++)
	{
		if (pCurrentPlayer->cards[i].color == top_card.color || pCurrentPlayer->cards[i].number == top_card.number)
		{
			return true;
		}
	}
	return false;
}

void drawCard(PLAYER* pCurrentPlayer, int current_size)
{
	CARD* new_cards = (CARD*)malloc(sizeof(CARD) * (current_size + 1));

	if (new_cards == NULL)
	{
		printf("\nMemory allocation failed! Exiting the program.");
		exit(1);
	}

	for (int i = 0; i < current_size; i++)
	{
		new_cards[i] = pCurrentPlayer->cards[i];
	}

	free(pCurrentPlayer->cards);
	pCurrentPlayer->cards = new_cards;
}




CARD playerChoice(PLAYER* pCurrentPlayer, int max_card, CARD top_card)
{
	int i, j, chosen_card;
	bool does_player_has_choice;
	bool is_choice_valid = false;

	does_player_has_choice = playerHasChoices(pCurrentPlayer, max_card, top_card);

	if (does_player_has_choice == false)
	{
		printf("\nYou don't have any viable moves! Card has been drawn.");
		return;
	}

	while (is_choice_valid == false)
	{
		printf("\nMake your move:\n");
		printf("\nEnter the number of the card you want to use:\n");
		scanf("%d", &chosen_card);

		if (chosen_card > max_card)
		{
			printf("\nYou only have %d cards! Try again.", max_card);
		}
		else
		{
			chosen_card -= 1;
			top_card = pCurrentPlayer->cards[chosen_card];
			discardCard(pCurrentPlayer, chosen_card, max_card);
			is_choice_valid = true;
		}
	}
	return top_card;
}





void main()
{
	int players_amount;
	PLAYER* players = NULL;
	int i;
	bool is_game_over = false;
	PLAYER* pCurrentPlayer = NULL;
	int first_player = 0;
	int max_card;
	CARD top_card;
	
	srand(time(NULL));

	printf("************ Welcome to TAKI game !!! ************");
	printf("\n\nPlease enter the number of players:\n");
	scanf("%d", &players_amount);

	players = (PLAYER*)malloc(players_amount * sizeof(PLAYER));
	
	if (players == NULL)
	{
		printf("Memory allocation failed! Exiting the program.");
		exit(1);
	}
	
	GetPlayersNames(players, players_amount);


	for (i = 0; i < players_amount; i++)
	{
		printf("\nPlayer #%d name: %s", i + 1, players[i].name);
	}

	dealCards(players, players_amount);

	for (i = 0; i < 1; i++)
	{
		if (pCurrentPlayer == NULL)
		{
			pCurrentPlayer = &players[0];
		}
		else
		{
			pCurrentPlayer = currentPlayer(&pCurrentPlayer, players_amount);
		}

		printf("\n\nIt's %s's turn!\n\n", pCurrentPlayer->name);

		top_card = printTopCard();

		//max_card = printCurretPlayerCards(players, pCurrentPlayer);

		max_card = printCurretPlayerCards(players, players_amount, pCurrentPlayer);

		//is_game_over = true;

		top_card = playerChoice(pCurrentPlayer, max_card, top_card);

		printf("\n\n");
		printCard(top_card.number, top_card.color);

		max_card = printCurretPlayerCards(players, players_amount, pCurrentPlayer);

	}

	free(players);
}