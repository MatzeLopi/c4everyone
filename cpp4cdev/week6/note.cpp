// Notes for week 6

#include <iostream>
#include <cassert>

using namespace std;

enum class suit : short
{
    clubs,
    diamonds,
    hearts,
    spades
};

/**
 * @class pips
 * @brief Represents a playing card pip value.
 *
 * The `pips` class represents the pip value of a playing card. It stores an integer value
 * between 1 and 13, inclusive. The class provides a constructor to initialize the pip value
 * and an `int` member variable to store the value.
 */
class pips
{
public:
    /**
     * @brief Constructs a `pips` object with the specified value.
     *
     * @param value The pip value of the playing card.
     *
     * @pre `value` must be between 1 and 13, inclusive.
     */
    pips(int value) : value(value)
    {
        assert(value >= 1 && value <= 13);
    }

    int value; /**< The pip value of the playing card. */
};

/**
 * @brief Represents a playing card.
 */
class card
{
public:
    /**
     * @brief Constructs a card with the specified suit and pips.
     *
     * @param suit The suit of the card.
     * @param pips The pips (rank) of the card.
     */
    card(suit suit, pips pips) : suit(suit), pips(pips) {}

    /**
     * @brief Gets the suit of the card.
     *
     * @return The suit of the card.
     */
    suit get_suit() const { return suit; }

    /**
     * @brief Gets the pips (rank) of the card.
     *
     * @return The pips of the card.
     */
    pips get_pips() const { return pips; }

    /**
     * @brief Overloads the << operator to output the card.
     *
     * @param os The output stream.
     * @param c The card to be output.
     * @return The output stream.
     */
    friend ostream &operator<<(ostream &os, const card &c);

private:
    suit suit; /**< The suit of the card. */
    pips pips; /**< The pips (rank) of the card. */
};

ostream &operator<<(ostream &os, const card &c)
{
    switch (c.suit)
    {
    case suit::clubs:
        os << "Clubs";
        break;
    case suit::diamonds:
        os << "Diamonds";
        break;
    case suit::hearts:
        os << "Hearts";
        break;
    case suit::spades:
        os << "Spades";
        break;
    }

    os << " " << c.pips.value;

    return os;
}

int main()
{
    card c(suit::hearts, pips(10));
    cout << c << endl;
    return 0;
}