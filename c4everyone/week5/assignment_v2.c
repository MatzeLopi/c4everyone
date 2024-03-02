/* Programm to read ints from a file, average them and print the average

Last edited: 02/MAR/2024
 */

#include <stdio.h>

int main(void)
{
    FILE *myFile;
    int i, n, num;
    double sum = 0.0;

    myFile = fopen("/home/matthias/WS_all/C/c4everyone/week5/elephant_seal_data.txt", "r");

    // Check if file was opened successfully
    if (myFile == NULL)
    {
        printf("Error opening the file.\n");
        return 1;
    }

    fscanf(myFile, "%d", &n);

    for (i = 0; i < n; i++)
    {
        fscanf(myFile, "%d", &num);
        sum += num;
    }

    fclose(myFile);

    double average = sum / n;

    printf("Average = %f \n", average);
    return 0;
}
