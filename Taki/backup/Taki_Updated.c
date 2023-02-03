//Student's name: Yuval Chait
//ID Number: 315232058

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define MAX_NAME 21
#define STARTING_SIZE 4
#define TOTAL_TYPE_COLORS 4
#define CARD_NUMBER 1
#define CARD_COLOR 2
#define MAX_SPECIAL_CARD_CHAR 7
#define ALL_CARDS_SIZE 14
#define PLUS_CARD 13
#define STOP_CARD 10
#define SWITCH_CARD 12
#define COLOR_CARD 14
#define TAKI_CARD 11
#define RED 1
#define BLUE 2
#define GREEN 3
#define YELLOW 4
#define DRAW_CARD 0


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

//Gets the names and the indexes of each player
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

//Takes a randimized number between 1 and 4 and converts it into a char representing a color
char convertNumToCardType(int card_type_int)
{
    //change to case
    char card_type;

    if (card_type_int == RED)
    {
        card_type = 'R';
    }
    else if (card_type_int == BLUE)
    {
        card_type = 'B';
    }
    else if (card_type_int == GREEN)
    {
        card_type = 'G';
    }
    else if (card_type_int == YELLOW)
    {
        card_type = 'Y';
    }
    return card_type;
}

//A function that prints a special card's name instead of the number represeting it
void printSpecialCard(int card_num)
{
    switch (card_num)
    {
    case STOP_CARD:
        printf("* STOP! *");
        break;
    case TAKI_CARD:
        printf("*  TAKI *");
        break;
    case 12:
        printf("*  <->  *");
        break;
    case PLUS_CARD:
        printf("*   +   *");
        break;
    case COLOR_CARD:
        printf("* COLOR *");
        break;
    }
}

//Similar to printSpecialCard function, only difference is that this function is used in printing the game statistics
char* returnSpecialCard(int card_num)
{
    switch (card_num)
    {
    case STOP_CARD:
        return "STOP!";
    case TAKI_CARD:
        return "TAKI";
    case SWITCH_CARD:
        return "<->";
    case PLUS_CARD:
        return "+";
    case COLOR_CARD:
        return "COLOR";
    }
}

//Takes an array of counters with each index representing the card number, and each time a card number appears in the game
//for example in a player's deck, in the top card etc. , card_counters[i] is incremented to track the frequency this card number has appeared
void addToCardCounters(int card_number, int card_counters[])
{
    card_counters[card_number]++;
}

//Dynamically allocates memory for all the cards array for each player in the game and radomizes 4 cards for each player.
void dealCards(PLAYER players[], int players_amount, int card_counters[])
{
    CARD* tempArr;
    int i, j, card_number, card_color, card_color_int;

    for (i = 0; i < players_amount; i++)
    {
        players[i].cards = (CARD*)malloc(STARTING_SIZE * sizeof(CARD));
        if (players[i].cards == NULL)
        {
            printf("\nMemory allocation failed! Existing program.");
            exit(1);
        }

        for (j = 0; j < STARTING_SIZE; j++)
        {
            card_number = (rand() % ALL_CARDS_SIZE) + 1;
            addToCardCounters(card_number, card_counters);
            card_color_int = (rand() % TOTAL_TYPE_COLORS) + 1;

            if (card_number == COLOR_CARD)
            {
                card_color = ' ';
            }
            else
            {
                card_color = convertNumToCardType(card_color_int);
            }

            players[i].cards[j].number = card_number;
            players[i].cards[j].color = card_color;
            players[i].size_cards = STARTING_SIZE;
        }
    }
}


//Prints a card using a card template with its number, color or the name of a special card (if it is a special card).
//is_top_card is a variable that track wether a card is the top card, because of the card "COLOR" is on top, the color the player chose
//to continue the game should be shown to all players.
void printCard(int card_number, char card_color, bool* is_top_card)
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
            if (*is_top_card == true)
            {
                printf("*   %c   *\n", card_color);
            }
            else
            {
                if (card_number == COLOR_CARD)
                {
                    printf("*       *\n");
                }
                else
                {
                    printf("*   %c   *\n", card_color);
                }
            }
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

//Prints the top card in the beginning of each turn.
//*previous_choice tracks the card the last player chose, so it could be displayed as the top card.
//*was_a_decision_made tracks wether a player has drawn a card or not. If he did, the value is false, else the value is true.
//So if the value of *was_a_decision_made was true, the top card should not be changed and the last top card is displayed.
void printTopCard(bool* was_a_decision_made, CARD* current_top_card, CARD* previous_choice)
{
    CARD top_card;
    int card_color_int;
    bool is_top_card = true;

    if (*was_a_decision_made == false)
    {
        printf("\nThe top card is:\n\n");
        printCard(previous_choice->number, previous_choice->color, &is_top_card);
        printf("\n\n");
    }
    else
    {
        printf("\nThe top card is:\n\n");
        printCard(current_top_card->number, current_top_card->color, &is_top_card);
        printf("\n\n");
    }
}

//Tracks who is the current player.
//As long as *is_inverted_rotation and *is_stop are false, the rotation starts from the player with index 0 and end with the player
//with index == players_amount - 1.
//if *is_inverted_row is true, then the rotation is flipped, and the first player of the rotation is the player with index == players_amount - 1
//and the last player is the player with index == 0.
//is *is_stop is true, then the next player in the rotation is skipped.
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


//Prints all the cards the current player is holding.
void printCurretPlayerCards(PLAYER* current_player, bool* was_a_decision_made)
{
    int i, card_number;
    char card_color;
    bool is_top_card = false;

    if (*was_a_decision_made == false)
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
        printCard(card_number, card_color, &is_top_card);
        printf("\n\n");
    }
}

//Deletes the card the player chose from his cards array and deleting all the gaps the were created in the proccess.
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

//Draws a new card and adds it to the players cards array. The function uses realloc to dynamically 
//increase the size of the players cards array if needed.
void drawCard(PLAYER* pCurrentPlayer, int card_counters[])
{
    int card_number, card_color_int;
    char card_color;

    pCurrentPlayer->cards = realloc(pCurrentPlayer->cards, (pCurrentPlayer->size_cards * 2) * sizeof(CARD));

    if (pCurrentPlayer->cards == NULL)
    {
        printf("\nMemory allocation failed! Exiting program.");
        exit(1);
    }

    card_number = (rand() % ALL_CARDS_SIZE) + 1;
    addToCardCounters(card_number, card_counters);
    card_color_int = (rand() % TOTAL_TYPE_COLORS) + 1;
    card_color = convertNumToCardType(card_color_int);

    pCurrentPlayer->cards[pCurrentPlayer->size_cards].color = card_color;
    pCurrentPlayer->cards[pCurrentPlayer->size_cards].number = card_number;
    pCurrentPlayer->size_cards++;
}

//This function takes the card the player chose and decides what should be done with it.
//If the card is not a special card (a regular number and color card), the card is discarded from the player's deck and is implemented
//to be the new top card (using *top_card).
//If the player choses to draw a card then a card is drawn and the function exists.
//Explanations on each special card are inside the function.
CARD playerChoice(PLAYER* pCurrentPlayer, int max_card, CARD* top_card, bool* was_a_decision_made, bool* is_inverted_rotation, bool* is_stop, int card_counters[])
{
    int i, j, chosen_card, new_color, choice_plus_card, taki_choice, temp_num;
    bool does_player_has_choice;
    bool is_choice_valid = false, is_valid_color = false, is_plus_valid = false, is_taki_stop = false;
    char taki_color;
    bool was_taki;
    bool was_plus = false;
    CARD temp_plus_card;
    CARD temp_chosen_card;
    //If the last player draw a card. -> top card remains the same.
    //*was_a_decision_made = playerHasChoices(pCurrentPlayer, max_card, *top_card);

    while (is_choice_valid == false) //while valid card was not chosen.
    {
        printf("\nMake your move:\n");
        printf("\nEnter the number of the card you want to use or enter \'0\' to draw a card:\n");
        scanf("%d", &chosen_card);

        if (chosen_card == DRAW_CARD)
        {
            drawCard(pCurrentPlayer, card_counters);
            *was_a_decision_made = false;
            is_choice_valid = true;
        }
        else
        {
            //Invalid choice cases
            if (chosen_card > max_card)
            {
                printf("\nYou only have %d cards! Try again.", max_card);
            }
            else if ((pCurrentPlayer->cards[chosen_card - 1].color != top_card->color)
                && (pCurrentPlayer->cards[chosen_card - 1].number != top_card->number) && (pCurrentPlayer->cards[chosen_card - 1].number != COLOR_CARD))
            {
                printf("\nThe top card doesn't match this card's color or number! Try again.");
            }
            else
            {
                was_taki = false; //For when player didn't use taki card


                //Taki card usage
                if (pCurrentPlayer->cards[chosen_card - 1].number == TAKI_CARD)
                {
                    was_taki = true; //Player used taki card, hence was_taki == true.
                    printf("\n\"TAKI\" card has been used! You can drop all the cards that match \"TAKI\'S\" color.\n");
                    taki_color = pCurrentPlayer->cards[chosen_card - 1].color; //To keep track of the color of taki
                    temp_chosen_card = pCurrentPlayer->cards[chosen_card - 1]; //Used later for when the last card used in taki "fall" is special
                    discardCard(pCurrentPlayer, chosen_card - 1);
                    temp_num = temp_chosen_card.number;

                    while (is_taki_stop == false)
                    {
                        //Prints the cards of the current player so he could see which card he still hasn't used
                        printCurretPlayerCards(pCurrentPlayer, was_a_decision_made); 
                        printf("\nEnter the card number you want to use or enter 0 to end your turn.\n");
                        scanf("%d", &taki_choice);

                        if (taki_choice == 0)//Player decided to stop using any more cards thus ending his turn
                        {
                            printf("\nTurn ended.\n");

                            is_taki_stop = true;
                        }
                        else
                        {
                            //Invalid choices cases
                            if ((taki_color != pCurrentPlayer->cards[taki_choice - 1].color) && (pCurrentPlayer->cards[taki_choice - 1].number != COLOR_CARD))
                            {
                                printf("\nThis card doesn't match TAKI\'S color! Try again.\n");
                            }
                            else if (taki_choice > pCurrentPlayer->size_cards)
                            {
                                printf("\nYou only have %d cards! Try again.\n", pCurrentPlayer->size_cards);
                            }
                            else
                            {
                                if (pCurrentPlayer->cards[taki_choice - 1].number == COLOR_CARD) //Stop taki "fall" if player chooses COLOR card
                                {
                                    temp_chosen_card = pCurrentPlayer->cards[taki_choice - 1];
                                    discardCard(pCurrentPlayer, taki_choice - 1);
                                    is_taki_stop = true;
                                    if (pCurrentPlayer->size_cards == 0) //For when player has no cards left while using taki card
                                    {
                                        return;
                                    }
                                }
                                else
                                {
                                    temp_chosen_card = pCurrentPlayer->cards[taki_choice - 1];
                                    discardCard(pCurrentPlayer, taki_choice - 1);
                                    if (pCurrentPlayer->size_cards == 0) //For when player has no cards left while using taki card
                                    {
                                        return;
                                    }
                                }

                            }
                        }
                    }
                }

                //For when the last card used in a taki "fall" was a special card
                //Explanations about each special card later in this function
                if (was_taki && temp_chosen_card.number >= 10)
                {
                    switch (temp_chosen_card.number)
                    {
                    case COLOR_CARD:
                        while (is_valid_color == false)
                        {
                            printf("\nWhich color do you want to continue the game? (1 - RED, 2 - BLUE, 3 - GREEN, 4 - YELLOW)\n");
                            scanf("%d", &new_color);

                            switch (new_color)
                            {
                            case RED:
                                temp_chosen_card.color = 'R';
                                is_valid_color = true;
                                break;
                            case BLUE:
                                temp_chosen_card.color = 'B';
                                is_valid_color = true;
                                break;
                            case GREEN:
                                temp_chosen_card.color = 'G';
                                is_valid_color = true;
                                break;
                            case YELLOW:
                                temp_chosen_card.color = 'Y';
                                is_valid_color = true;
                                break;
                            default:
                                printf("invlid choice");
                                break;
                            }
                        }
                        break;
                    case PLUS_CARD:
                        was_plus = true;
                        temp_plus_card = temp_chosen_card;
                        printCurretPlayerCards(pCurrentPlayer, was_a_decision_made);
                        printf("\nYou used \'+\' card! Enter a card with the same color or another \'+\' card:\n");
                        printf("\nIf you don't have a viable card, enter \'0\' to draw a card:\n");
                        while (is_plus_valid == false)
                        {
                            scanf("%d", &choice_plus_card);

                            if ((pCurrentPlayer->cards[choice_plus_card - 1].color != temp_plus_card.color) &&
                                (pCurrentPlayer->cards[choice_plus_card - 1].number != PLUS_CARD) && (choice_plus_card != 0))
                            {
                                printf("\nCard doesn't match to the color of the \'+\' you used and it isn't a \'+'\ either! Try again.\n");
                            }
                            else if (choice_plus_card == DRAW_CARD)
                            {
                                drawCard(pCurrentPlayer, card_counters);
                                *top_card = temp_plus_card; 
                                is_plus_valid = true;
                            }
                            else
                            {
                                if (pCurrentPlayer->cards[choice_plus_card - 1].number == PLUS_CARD)
                                {
                                    temp_plus_card.color = pCurrentPlayer->cards[choice_plus_card - 1].color;

                                    temp_plus_card.number = pCurrentPlayer->cards[choice_plus_card - 1].number;
                                }
                                else if (temp_plus_card.color == pCurrentPlayer->cards[choice_plus_card - 1].color)
                                {

                                    *top_card = pCurrentPlayer->cards[choice_plus_card - 1];

                                    discardCard(pCurrentPlayer, choice_plus_card - 1);

                                    is_plus_valid = true;
                                }
                            }
                        }
                        break;
                    case SWITCH_CARD:
                        printf("\nTurns rotation has been inverted!\n");
                        *is_inverted_rotation = !(*is_inverted_rotation);
                        break;
                    case STOP_CARD:
                        *is_stop = true;
                        printf("\n\"STOP!\" card has been used! The rotation skips next player\'s turn.\n");
                        break;
                    }
                }

                //For when a special card was used outside of a taki card "fall"
                else if (!was_taki && pCurrentPlayer->cards[chosen_card - 1].number >= 10)
                {
                    switch (pCurrentPlayer->cards[chosen_card - 1].number)
                    {
                        //COLOR card usage. The player chooses which color will continue the game. The top card will be displayed
                        //In the next turn as the top card with the color the last player chose.
                    case COLOR_CARD:
                        while (is_valid_color == false)
                        {
                            printf("\nWhich color do you want to continue the game? (1 - RED, 2 - BLUE, 3 - GREEN, 4 - YELLOW)\n");
                            scanf("%d", &new_color);

                            switch (new_color)
                            {
                            case RED:
                                pCurrentPlayer->cards[chosen_card - 1].color = 'R';
                                is_valid_color = true;
                                break;
                            case BLUE:
                                pCurrentPlayer->cards[chosen_card - 1].color = 'B';
                                is_valid_color = true;
                                break;
                            case GREEN:
                                pCurrentPlayer->cards[chosen_card - 1].color = 'G';
                                is_valid_color = true;
                                break;
                            case YELLOW:
                                pCurrentPlayer->cards[chosen_card - 1].color = 'Y';
                                is_valid_color = true;
                                break;
                            default:
                                printf("invlid choice");
                                break;
                            }
                        }
                        break;
                    //Plus card usage. Player is allowed the use another card with the same color as the + card or use another plus card.
                    case PLUS_CARD:
                        was_plus = true; //True when plus card was used
                        temp_plus_card = pCurrentPlayer->cards[chosen_card - 1];
                        discardCard(pCurrentPlayer, chosen_card - 1);
                        printf("\nYou used \'+\' card! Enter a card with the same color or another \'+\' card:\n");
                        printCurretPlayerCards(pCurrentPlayer, was_a_decision_made);
                        printf("\nIf you don't have a viable card, enter \'0\' to draw a card:\n");

                        while (is_plus_valid == false)
                        {
                            scanf("%d", &choice_plus_card);

                            if ((pCurrentPlayer->cards[choice_plus_card - 1].color != temp_plus_card.color) &&
                                (pCurrentPlayer->cards[choice_plus_card - 1].number != PLUS_CARD) && (choice_plus_card != 0))
                            {
                                printf("\nCard doesn't match to the color of the \'+\' you used and it isn't a \'+'\ either! Try again.\n");
                            }
                            //Player has to draw a card if doesn't have a card with a matching color to the plus card
                            //or if he doesn't have another plus card
                            else if (choice_plus_card == DRAW_CARD)
                            {
                                drawCard(pCurrentPlayer, card_counters);
                                *top_card = temp_plus_card; //Top card set to be the plus card if player doesn't have any matching cards
                                is_plus_valid = true;
                            }
                            else
                            {
                                if (pCurrentPlayer->cards[choice_plus_card - 1].number == 13)//Player uses another plus card
                                {
                                    temp_plus_card.color = pCurrentPlayer->cards[choice_plus_card - 1].color;

                                    temp_plus_card.number = pCurrentPlayer->cards[choice_plus_card - 1].number;
                                }
                                //For when player uses a card with a matching color to the plus card
                                else if (temp_plus_card.color == pCurrentPlayer->cards[choice_plus_card - 1].color)
                                {
                                    *top_card = pCurrentPlayer->cards[choice_plus_card - 1];

                                    discardCard(pCurrentPlayer, choice_plus_card - 1);

                                    is_plus_valid = true;
                                }
                            }
                        }
                        break;
                    //Switch card usage. sets *is_inverted_rotation boolean value to be the the opposite of what it was.
                    //Later it is passed to currentPlayer function to determine if the rotation should be regular or inverted.
                    case SWITCH_CARD:
                        printf("\nTurns rotation has been inverted!\n");
                        *is_inverted_rotation = !(*is_inverted_rotation);
                        break;
                        //Stop card usage. sets *is_stop to true.
                        //Later it is passed to currentPlayer function to determine if next player in the rotation should be skipped.
                    case STOP_CARD:
                        *is_stop = true;
                        printf("\n\"STOP!\" card has been used! The rotation skips next player\'s turn.\n");
                        break;
                    }
                }

                //If the first card of the turn was a taki card, the last card the was chosen in the taki "fall" (which is stored in 
                //"temp_chosen_card") now becomes the top card.
                if (was_taki)
                {
                    *top_card = temp_chosen_card;
                }
                else if (was_plus)
                {
                    return *top_card; 
                }
                //For when taki card was not used. the top card will become the card the player used.
                else 
                {
                    *top_card = pCurrentPlayer->cards[chosen_card - 1];
                    discardCard(pCurrentPlayer, chosen_card - 1);
                }
                is_choice_valid = true;
            }
        }
    }
    return *top_card;
}

//Iterates through the card_counters array that tracks the frequency of each card that appeared in the game.
//Finds the card that appeared in the highest frequency and sets it to be the tmp_max card and the max index to be equal to i
//which represesnts the card number of the current max card. The function returns the max_index.
//if max_index == 0 then later in printStatistics function, the loop stops and the function stops printing.
int getIndexOfMaxNumber(int card_counters[])
{
    int tmp_max = -1;
    int max_index = 0;
    int i;
    for (i = 0; i <= ALL_CARDS_SIZE; i++)
    {
        if (card_counters[i] > tmp_max)
        {
            tmp_max = card_counters[i];
            max_index = i;
        }
    }
    if (tmp_max == -1)
    {
        max_index = 0;
    }
    return max_index;
}

//Prints the frequency of each card number (or special card) that appeared in the game in a template, in a descending order (ordered by frequency).
//Stops printing when index == 0 (determined by getIndexOfMaxNumber function).
void printStatistics(int card_counters[])
{
    int index = getIndexOfMaxNumber(card_counters);
    printf("\n ************ Game Statistics ************");
    printf("\n\nCard #  |  Frequency");
    printf("\n_____________________");

    while (index != 0)
    {
        if (index > 9)
        {
            char* card_name = returnSpecialCard(index);
            if (index == STOP_CARD || index == COLOR_CARD)
            {
                printf("\n  %s", card_name);
            }
            else if (index == TAKI_CARD)
            {
                printf("\n  %s ", card_name);
            }
            else if (index == SWITCH_CARD)
            {
                printf("\n  %s  ", card_name);
            }
            else if (index == PLUS_CARD)
            {
                printf("\n   %s   ", card_name);
            }
            printf(" | ");
            printf("   %d", card_counters[index]);
        }

        else
        {
            printf("\n   %d   ", index);
            printf(" | ");
            printf("   %d  ", card_counters[index]);
        }
        card_counters[index] = -1;
        index = getIndexOfMaxNumber(card_counters);
        if (index == 0)
        {
            break;
        }
    }
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
    bool was_a_decision_made;
    bool is_inverted_rotation = false;
    bool is_stop;
    int card_counters[ALL_CARDS_SIZE + 1];

    srand(time(NULL));

    //Initializing card_counters for statistics
    
    for (i = 0; i <= ALL_CARDS_SIZE; i++)
    {
        if (i == 0)
        {
            card_counters[i] = -1;
        }
        else
        {
            card_counters[i] = 0;
        }
    }



    printf("************ Welcome to TAKI game !!! ************");
    printf("\n\nPlease enter the number of players:\n");
    scanf("%d", &players_amount);

    players = (PLAYER*)malloc(players_amount * sizeof(PLAYER));//Allocating memory for players array based on the amount of players.

    if (players == NULL)
    {
        printf("Memory allocation failed! Exiting the program.");
        exit(1);
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
            top_card.number = (rand() % ALL_CARDS_SIZE - 1) + 1;
            addToCardCounters(top_card.number, card_counters);
            top_card_color_int = (rand() % TOTAL_TYPE_COLORS) + 1;
            top_card.color = convertNumToCardType(top_card_color_int);
        }
        else
        {
            //Deciding who will be the next player based ob currentPlayer function
            pCurrentPlayer = currentPlayer(&pCurrentPlayer, players, players_amount, pCurrentPlayer->player_index, &is_inverted_rotation, &players[players_amount - 1], &is_stop);
        }

        //Resets 'is_stop' for the next player
        is_stop = false;

        printf("\n\nIt's %s's turn!\n\n", pCurrentPlayer->name);
        printf("******************\n");

        //Top card is printed
        printTopCard(&was_a_decision_made, &top_card, &previous_choice);

        was_a_decision_made = true; //Resets was_a_decision_made for the current player

        //Current player's cards are printed
        printCurretPlayerCards(pCurrentPlayer, &was_a_decision_made);


        //Implementing the player's choice in "previous_choice" for the program to decide which top card to print in the next turn.
        previous_choice = playerChoice(pCurrentPlayer, pCurrentPlayer->size_cards, &top_card, &was_a_decision_made, &is_inverted_rotation, &is_stop, card_counters);


        if (pCurrentPlayer->size_cards == 1)
        {
            printf("\n%s has one card left in his hand!", pCurrentPlayer->name);
        }
        else if (pCurrentPlayer->size_cards == 0)
        {
            printf("\n%s is the winner!\n\n", pCurrentPlayer->name);
            is_game_over = true;
        }
    }

    //Printing game statistics
    printStatistics(card_counters);

    //Freeing the cards array of each player and then freeing the entire players array
    for (i = 0; i < players_amount; i++)
    {
        free(players[i].cards);
    }

    free(players);

}