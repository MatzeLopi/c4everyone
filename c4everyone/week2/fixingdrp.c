/* Week 2 of fixing Dr. Ps mistakes.
Function which returns the volume of a sphere

Last edited: 12/FEB/2024

*/

#include <stdio.h>

#define PI 3.14159

int main(void)
{
    int radius;
    printf("Enter radius:");
    scanf("%d", &radius);
    printf("volume is : %lf \n\n", 4.0 * radius * radius * radius / 3);
    return 0;
}
