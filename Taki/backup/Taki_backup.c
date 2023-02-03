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
#define MAX_SPECIAL_CARD_CHAR 7
#define ALL_CARDS_SIZE 14

typedef struct
{
	int number;
	char color;
}CARD;

typedef struct
{
	char name[MAX_NAME];
	CARD* cards;
	int size_cards;
	int player_index;
}PLAYER;


void GetPlayersNamesAndIndex(PLAYER players[], int players_amount)
{
	int i;

	for (i = 0; i < players_amount; i++)
	{
		printf("\nPlease enter the first name of player #%d:\n", i + 1);
		scanf("%s", players[i].name);
		players[i].player_index = i;
	}
}

char convertNumToCardType(int card_type_int)
{
	//change to case
	char card_type;

	if (card_type_int == 1)
	{
		card_type = 'R';
	}
	else if (card_type_int == 2)
	{
		card_type = 'B';
	}
	else if (card_type_int == 3)
	{
		card_type = 'G';
	}
	else if (card_type_int == 4)
	{
		card_type = 'Y';
	}
	return card_type;
}

void printSpecialCard(int card_num) {
	switch (card_num)
	{
	case 10:
		printf("* STOP! *");
		break;
	case 11:
		printf("*  TAKI *");
		break;
	case 12:
		printf("*  <->  *");
		break;
	case 13:
		printf("*   +   *");
		break;
	case 14:
		printf("* COLOR *");
		break;
	}
}


void addToCardCounters(int card_number, int card_counters[])
{
	card_counters[card_number]++;
}

void dealCards(PLAYER players[], int players_amount, int card_counters[])
{
	CARD* tempArr;
	int i, j, card_number, card_color, card_color_int;

	for (i = 0; i < players_amount; i++)
	{
		players[i].cards = (CARD*)malloc(4 * sizeof(CARD));
		if (players[i].cards == NULL)
		{
			printf("\nMemory allocation failed! Existing program.");
			exit(1);
		}

		for (j = 0; j < 4; j++)
		{
			card_number = (rand() % 14) + 1;
			addToCardCounters(card_number, card_counters);
			card_color_int = (rand() % 4) + 1;

			if (card_number == 14)
			{
				card_color = ' ';
			}
			else
			{
				card_color = convertNumToCardType(card_color_int);
			}

			players[i].cards[j].number = card_number;
			players[i].cards[j].color = card_color;
			players[i].size_cards = 4;
		}
	}
}



void printCard(int card_number, char card_color)
{
	int i, j;
	char special_card[MAX_SPECIAL_CARD_CHAR];

	for (i = 0; i < 9; i++)
	{
		printf("*");
	}
	printf("\n");
	for (j = 0; j < 4; j++)
	{
		if (j == 1)
		{
			if (card_number >= 10)
			{
				printSpecialCard(card_number);
				printf("\n");
			}
			else
			{
				printf("*   %d   *\n", card_number);
			}
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


void printTopCard(bool* was_a_card_drawn, CARD* current_top_card, CARD* previous_choice)
{
	CARD top_card;
	int card_color_int;

	if (*was_a_card_drawn == false)
	{
		printf("\nThe top card is:\n\n");
		printCard(previous_choice->number, previous_choice->color);
		printf("\n\n");
	}
	else
	{
		printf("\nThe top card is:\n\n");
		printCard(current_top_card->number, current_top_card->color);
		printf("\n\n");
	}
}


PLAYER* currentPlayer(PLAYER** current_player, PLAYER* players[], int players_amount, int player_index, 
	bool* is_inverted_rotation, PLAYER* last_player, bool* is_stop)
{
	if (*is_inverted_rotation == true)
	{
		if (*is_stop == false)
		{
			if (player_index == 0)
			{
				*current_player = last_player;
			}
			else
			{
				*current_player = *current_player - 1;
			}
		}
		else if (*is_stop == true)
		{
			if (player_index - 2 == 0)
			{
				*current_player = last_player;
			}
			else if (player_index - 2 < 0)
			{
				*current_player = last_player;
				*current_player = *current_player - 1;
			}
			else
			{
				*current_player = *current_player - 2;
			}
		}
	}
	else
	{
		if (*is_stop == false)
		{
			if (player_index + 1 == players_amount)
			{
				*current_player = &players[0];
			}
			else
			{
				*current_player = *current_player + 1;
			}
		}
		else if (*is_stop == true)
		{
			if (player_index + 2 == players_amount)
			{
				*current_player = &players[0];
			}
			else if (player_index + 2 > players_amount)
			{
				*current_player = &players[0];
				*current_player = *current_player + 1;
			}
			else
			{
				*current_player = *current_player + 2;
			}
		}
		
	}
	return *current_player;
}



 void printCurretPlayerCards(PLAYER* current_player, bool* was_a_card_drawn)
{
	int i, card_number;
	char card_color;

	if (*was_a_card_drawn == false)
	{
		return;
	}

	printf("Your cards:\n");
	printf("_______________________\n\n");

	for (i = 0; i < current_player->size_cards; i++)
	{
		card_number = current_player->cards[i].number;
		card_color = current_player->cards[i].color;

		printf("(%d)\n\n", i + 1);
		printCard(card_number, card_color);
		printf("\n\n");
	}
}

void discardCard(PLAYER* pCurrentPlayer, int chosen_card)
{
	int i;
	pCurrentPlayer->cards[chosen_card].color = '\0';
	pCurrentPlayer->cards[chosen_card].number = 0;

	for (i = chosen_card; i < pCurrentPlayer->size_cards; i++)
	{
		if (i + 1 == pCurrentPlayer->size_cards)
		{
			pCurrentPlayer->size_cards--;
			return;
		}
		else
		{
			pCurrentPlayer->cards[i] = pCurrentPlayer->cards[i + 1];
			pCurrentPlayer->cards[i + 1].color = '\0';
			pCurrentPlayer->cards[i + 1].number = 0;
		}
	}
}

bool playerHasChoices(PLAYER* pCurrentPlayer, int size_cards, CARD top_card)
{
	if (size_cards <= 0)
	{
		return false;
	}

	for (int i = 0; i < size_cards; i++)
	{
		if (pCurrentPlayer->cards[i].color == top_card.color || pCurrentPlayer->cards[i].number == top_card.number)
		{
			return true;
		}
	}
	return false;
}

void drawCard(PLAYER* pCurrentPlayer, int card_counters[])
{
	int card_number, card_color_int;
	char card_color;

	pCurrentPlayer->cards = realloc(pCurrentPlayer->cards, (pCurrentPlayer->size_cards + 1) * sizeof(CARD));

	if (pCurrentPlayer->cards == NULL)
	{
		printf("\nMemory allocation failed! Exiting program.");
		exit(1);
	}

	card_number = (rand() % 14) + 1;
	addToCardCounters(card_number, card_counters);
	card_color_int = (rand() % 4) + 1;
	card_color = convertNumToCardType(card_color_int);

	pCurrentPlayer->cards[pCurrentPlayer->size_cards].color = card_color;
	pCurrentPlayer->cards[pCurrentPlayer->size_cards].number = card_number;
	pCurrentPlayer->size_cards++;
}

CARD playerChoice(PLAYER* pCurrentPlayer, int max_card, CARD* top_card, bool* was_a_card_drawn, bool* is_inverted_rotation, bool* is_stop, int card_counters[])
{
	int i, j, chosen_card, new_color, choice_plus_card, taki_choice;
	bool does_player_has_choice;
	bool is_choice_valid = false, is_valid_color = false, is_plus_valid = false, is_taki_stop = false;
	char taki_color;
	CARD temp_chosen_card;

	*was_a_card_drawn = playerHasChoices(pCurrentPlayer, max_card, *top_card);

	while (is_choice_valid == false)
	{
		printf("\nMake your move:\n");
		printf("\nEnter the number of the card you want to use or enter \'0\' to draw a card:\n");
		scanf("%d", &chosen_card);

		if (chosen_card == 0)
		{
			drawCard(pCurrentPlayer, card_counters);
			*was_a_card_drawn = false;
			is_choice_valid = true;
		}
		else
		{
			if (chosen_card > max_card)
			{
				printf("\nYou only have %d cards! Try again.", max_card);
			}
			else if ((pCurrentPlayer->cards[chosen_card - 1].color != top_card->color)
				&& (pCurrentPlayer->cards[chosen_card - 1].number != top_card->number) && (pCurrentPlayer->cards[chosen_card - 1].number != 14) )
			{
				printf("\nThe top card doesn't match this card's color or number! Try again.");
			}
			else
			{

				//Taki card usage

				if (pCurrentPlayer->cards[chosen_card - 1].number == 11)
				{
					printf("\n\"TAKI\" card has been used! You can drop all the cards that match \"TAKI\'S\" color.\n");
					taki_color = pCurrentPlayer->cards[chosen_card - 1].color;
					temp_chosen_card = pCurrentPlayer->cards[chosen_card - 1];
					discardCard(pCurrentPlayer, chosen_card - 1);

					while (is_taki_stop == false)
					{
						printCurretPlayerCards(pCurrentPlayer, was_a_card_drawn);
						printf("\nEnter the card number you want to use or enter 0 to end your turn.\n");
						scanf("%d", &taki_choice);

						if (taki_choice == 0)
						{
							printf("\nTurn ended.\n");

							pCurrentPlayer->cards[chosen_card - 1] = temp_chosen_card;

							is_taki_stop = true;
						}
						else
						{
							if ( (taki_color != pCurrentPlayer->cards[taki_choice - 1].color) && (pCurrentPlayer->cards[taki_choice - 1].number != 14) )
							{
								printf("\nThis card doesn't match TAKI\'S color! Try again.\n");
							}
							else if (taki_choice > pCurrentPlayer->size_cards)
							{
								printf("\nYou only have %d cards! Try again.\n", pCurrentPlayer->size_cards);
							}
							else
							{
								if (pCurrentPlayer->cards[taki_choice - 1].number == 14) //Stop taki "fall" if player chooses COLOR card
								{
									pCurrentPlayer->cards[chosen_card - 1] = pCurrentPlayer->cards[taki_choice - 1];
									discardCard(pCurrentPlayer, taki_choice - 1);
									is_taki_stop = true;
								}
								else
								{
									temp_chosen_card = pCurrentPlayer->cards[taki_choice - 1];
									discardCard(pCurrentPlayer, taki_choice - 1);
								}

							}
						}
					}
				}

				if (pCurrentPlayer->cards[chosen_card - 1].number >= 10)
				{
					switch (pCurrentPlayer->cards[chosen_card - 1].number)
					{
					case 14:
						while (is_valid_color == false)
						{
							printf("\nWhich color do you want to continue the game? (1 - RED, 2 - BLUE, 3 - GREEN, 4 - YELLOW)\n");
							scanf("%d", &new_color);

							switch (new_color)
							{
							case 1:
								pCurrentPlayer->cards[chosen_card - 1].color = 'R';
								is_valid_color = true;
								break;
							case 2:
								pCurrentPlayer->cards[chosen_card - 1].color = 'B';
								is_valid_color = true;
								break;
							case 3:
								pCurrentPlayer->cards[chosen_card - 1].color = 'G';
								is_valid_color = true;
								break;
							case 4:
								pCurrentPlayer->cards[chosen_card - 1].color = 'Y';
								is_valid_color = true;
								break;
							}
						}
						break;
					case 13:
						printf("\nYou used \'+\' card! Enter a card with the same color or another \'+\' card:\n");
						printf("\nIf you don't have a viable card, enter \'0\' to draw a card:\n");
						while (is_plus_valid == false)
						{
							scanf("%d", &choice_plus_card);

							if ((pCurrentPlayer->cards[choice_plus_card - 1].color != pCurrentPlayer->cards[chosen_card - 1].color) &&
								(pCurrentPlayer->cards[choice_plus_card - 1].number != 13) && (choice_plus_card != 0) )
							{
								printf("\nCard doesn't match to the color of the \'+\' you used and it isn't a \'+'\ either! Try again.\n");
							}
							else if (choice_plus_card == 0)
							{
								drawCard(pCurrentPlayer, card_counters);
								is_plus_valid = true;
							}
							else
							{
								if (pCurrentPlayer->cards[choice_plus_card - 1].number == 13)
								{
									pCurrentPlayer->cards[chosen_card - 1].color = pCurrentPlayer->cards[choice_plus_card - 1].color;

									pCurrentPlayer->cards[chosen_card - 1].number = pCurrentPlayer->cards[choice_plus_card - 1].number;
								}
								else
								{
									pCurrentPlayer->cards[chosen_card - 1] = pCurrentPlayer->cards[choice_plus_card - 1];

									discardCard(pCurrentPlayer, choice_plus_card - 1);

									is_plus_valid = true;
								}
							}
						}
						break;
					case 12:
						printf("\nTurns rotation has been inverted!\n");
						*is_inverted_rotation = !(*is_inverted_rotation);
						break;
					case 10:
						*is_stop = true;
						printf("\n\"STOP!\" card has been used! The rotation skips next player\'s turn.\n");
						break;
					}
				}

				*top_card = pCurrentPlayer->cards[chosen_card - 1];
				discardCard(pCurrentPlayer, chosen_card - 1);
				is_choice_valid = true;
			}
		}
	}
	return *top_card;
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
	int top_card_color_int;
	CARD top_card;
	CARD previous_choice;
	bool was_a_card_drawn;
	bool is_inverted_rotation = false;
	bool is_stop;
	int card_counters[ALL_CARDS_SIZE];

	//srand(time(NULL));
	srand(105);


	//Initializing card_counters for statistics

	for (i = 1; i <= ALL_CARDS_SIZE; i++)
	{
		card_counters[i] = 0;
	}


	printf("************ Welcome to TAKI game !!! ************");
	printf("\n\nPlease enter the number of players:\n");
	scanf("%d", &players_amount);

	players = (PLAYER*)malloc(players_amount * sizeof(PLAYER));
	
	if (players == NULL)
	{
		printf("Memory allocation failed! Exiting the program.");
		exit(1);
		was_a_card_drawn = false;
	}
	
	GetPlayersNamesAndIndex(players, players_amount);

	for (i = 0; i < players_amount; i++)
	{
		printf("\nPlayer #%d name: %s", i + 1, players[i].name);
	}

	dealCards(players, players_amount, card_counters);

	printf("\n\nCards have been delt to all players. The game has begun!\n");
	printf("**********************************************************");


	//Who is the current player:

	while (is_game_over == false)
	{
		if (pCurrentPlayer == NULL)
		{
			//Setting the first player
			pCurrentPlayer = &players[0];

			//Drawing the first top card of the game
			top_card.number = (rand() % 13) + 1;
			addToCardCounters(top_card.number, card_counters);
			top_card_color_int = (rand() % 4) + 1;
			top_card.color = convertNumToCardType(top_card_color_int);
		}
		else
		{
			pCurrentPlayer = currentPlayer(&pCurrentPlayer, players, players_amount, pCurrentPlayer->player_index, &is_inverted_rotation, &players[players_amount - 1], &is_stop);
		}

		//Resets 'is_stop' for the next player
		is_stop = false;

		printf("\n\nIt's %s's turn!\n\n", pCurrentPlayer->name);
		printf("******************\n");


		printTopCard(&was_a_card_drawn, &top_card, &previous_choice);

		printf("\n\nTop card number: %d\nTop card color: %c\n\n", top_card.number, top_card.color);


		was_a_card_drawn = true; //Resets was_a_card_drawn for there current player

		printCurretPlayerCards(pCurrentPlayer, &was_a_card_drawn);


		//Implementing the player's choice in "previous_choice" for the program to decide which top card to print in the next turn.
		previous_choice = playerChoice(pCurrentPlayer, pCurrentPlayer->size_cards, &top_card, &was_a_card_drawn, &is_inverted_rotation, &is_stop, card_counters);


		printf("\n\n");
		for (i = 1; i <= ALL_CARDS_SIZE; i++)
		{
			printf("%d: %d\n", i, card_counters[i]);
		}

		//Winning conditions:

		if (pCurrentPlayer->size_cards == 1)
		{
			printf("\nYuval has one card left in his hand!");
		}
		else if (pCurrentPlayer->size_cards == 0)
		{
			printf("\n%s won!");
			is_game_over = true;
		}
	}

	for (i = 0; i < players_amount; i++)
	{
		free(players[i].cards);
	}
	
	free(players);
}