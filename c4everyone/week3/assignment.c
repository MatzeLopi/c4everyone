/*
Write a C program that has a function that prints a table of values for sine and cosine between (0, 1).

Last edited: 02/MAR/2024

*/

#include <stdio.h>
#include <math.h>

int main(void)
{
    int break_condition = 50;
    float increment = 1.0 / break_condition;

    for (int i = 0; i < break_condition + 1; i++)
    {
        printf("sin( %f ) = %f \tcos( %f ) = %f\n", increment * i, sin(increment * i), increment * i, cos(increment * i));
    }
    return 0;
}