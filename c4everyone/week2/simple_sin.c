/*

Code to output the value of a sign function based on an input value between (0,1)

Last edit: 20/FEB/2024
*/

#include <stdio.h>
#include <math.h>

int main(void)
{
    double input_value = 0.0;
    double result = 0.0;

    printf("\nPlease input value in range (0,1)\n");

    scanf("%lf", &input_value);
    result = sin(input_value);
    printf("sin(%.lf) = %.lf\n", input_value, result);

    return 0;
}