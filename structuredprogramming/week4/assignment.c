/*Assignment of week 4 | Option 2

Task:
    - Open and read a file of integer pairs into an array that is created with the first integer telling you how many to read.
    - Each pair will be a stuct rational -meaning it represents a rational number. So a struct will have two members—the numerator and the denominator.

    - Write four routines:
         - to add rationals
         - multiply rationals
         - subtract rationals
         - divide rationals.

Example:
    - So 2  9  11  4  5  would mean create an rational  array size 2 and read in the remaining 4 values into data[].
        Data[0] will be (9,11)  - the rational 9/11. Data[1] will be (4, 5)—the rational 4/5.
*/

#include <stdio.h>
#include <stdlib.h>

#define MAX_INTS 10000

// Typedef for rational number struct
typedef struct rational
{
    int numerator;
    int denominator;
} rational;

// Function to find the greatest common divisor using Euclidean algorithm
int gcd(int a, int b)
{
    while (b != 0)
    {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Function to simplify the rational number
rational simplify(rational number)
{
    if (number.numerator == 0 || number.denominator == 0)
        return number;
    // Find the greatest common divisor
    int divisor = gcd(number.numerator, number.denominator);

    // Simplify the fraction
    number.numerator /= divisor;
    number.denominator /= divisor;

    return number;
}
// Simple implementation of addition of rational numbers
rational add(rational x, rational y)
{
    rational number;

    if (x.numerator == 0 || x.denominator == 0)
        return y;

    if (y.numerator == 0 || y.denominator == 0)
        return x;

    if (x.denominator == y.denominator)
    {
        number.numerator = x.numerator + y.numerator;
        number.denominator = x.denominator;
    }
    else
    {
        int denominator = x.denominator * y.denominator;
        number.numerator = x.numerator * y.denominator + y.numerator * x.denominator;
        number.denominator = denominator;
    }

    return simplify(number);
}

// Simple implementation of substraction of rational numbers
rational substract(rational x, rational y)
{
    rational number;
    if (x.denominator == y.denominator)
    {
        number.numerator = x.numerator - y.numerator;
        number.denominator = x.denominator;
    }
    else
    {
        int denominator = x.denominator * y.denominator;
        number.numerator = x.numerator * y.denominator - y.numerator * x.denominator;
        number.denominator = denominator;
    }

    return simplify(number);
}

// Simple implementation of multiplikation of rational numbers
rational multiply(rational x, rational y)
{
    rational number;
    number.numerator = x.numerator * y.numerator;
    number.denominator = x.denominator * y.denominator;

    return simplify(number);
}

// Simple implementation of division of rational numbers
rational divide(rational x, rational y)
{
    rational number;
    number.numerator = x.numerator * y.denominator;
    number.denominator = x.denominator * y.numerator;

    return simplify(number);
}

int main(void)
{
    // Define array to read in ints
    int array[MAX_INTS];
    int elements = 0;

    // open file
    FILE *file = fopen("input.txt", "r");

    // Check if file was opened
    if (file == NULL)
    {
        printf("Unable to open the file.\n");
        return 1;
    }

    // read file
    while (fscanf(file, "%d", &array[elements]) != EOF)
    {
        elements++;
    }

    // close file
    fclose(file);

    // Create rational numbers
    int number_rationals = array[0];
    rational rationals[number_rationals];
    rational sum = {0, 0};
    rational average;

    for (int i = 1; i < number_rationals * 2 + 1; i += 2)
    {
        rational new_rational = {array[i], array[i + 1]};
        sum = add(sum, new_rational);
        if (i > 1)
            rationals[(i - 1) / 2] = new_rational;
        else
            rationals[0] = new_rational;
    }

    // print result
    float divider = number_rationals * 1.0;
    printf("Sum: %d / %d \n", sum.numerator, sum.denominator);
    printf("Average: %.2f / %.2f\n", sum.numerator / divider, sum.denominator / divider);

    return 0;
}