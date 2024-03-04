/* Honor Assignment of week2

Tasks:
    - Use a struct to define a card as an enumerated member that is its suit value and a short that is its pips value.
    - Write a function that randomly shuffles the deck.

    - Then deal out 7 card hands and evaluate the probability that a hand has no pair, one pair, two pair, three of a kind, full house and 4 of a kind.
    - This is a Monte Carlo method to get an approximation to these probabilities.  Use at least 1 million randomly generated hands.

Last edited: 03/MAR/2024
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Def of enum for suit
typedef enum suit
{
    HEARTS,
    DIAMONDS,
    CLUBS,
    SPADES
} suit;

// Def of card
typedef struct card
{
    suit suit;
    short pips;
} card;

// Simple random generator to return int in range [lower,upper]
int rand_range(int lower, int upper)
{
    return (rand() % (upper - lower + 1)) + lower;
}

void fill_deck(card deck[52])
{
    int card_count = 0;
    for (int i = 0; i < 4; i++) // i value for suit
    {
        for (int j = 0; j < 13; j++) // j is card value
        {
            card new_card = {i, j};
            deck[card_count] = new_card;
            card_count++;
        }
    }
}

// Function to shuffel a deck of cards
//
void shuffle(card deck[52])
{
    for (int i = 0; i < 10; i++) // Times to iterate through the deck
    {
        for (int j = 0; j < 52; j++) // Iterate through the whole deck swap two cards
        {
            card card_0 = deck[j];
            int new_pos = rand_range(0, 52);
            card card_1 = deck[new_pos];
            deck[new_pos] = card_0;
            deck[i] = card_1;
        }
    }
}
void deal_hand(card deck[52], card hand[7])
{
    shuffle(deck);
    for (int i = 0; i < 7; i++)
        hand[i] = deck[i];
}

// Main
int main(void)
{
    srand(time(0)); // Set seed for random generator

    int i = 0;
    card deck[52];
    fill_deck(deck);
    card hand[7];
    deal_hand(deck, hand);
}