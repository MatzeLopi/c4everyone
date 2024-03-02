/* Write some code to read the sourc data for the seal population and calculate the average.

Last edited: 02/MAR/2024

Note:
    - Not efficient and error prone...
    - Still kept it to see the diff to version 2
*/

#include <stdio.h>
#include <stdlib.h>

double sum(int array[], int size)
{
    double sum = 0.0;

    for (int i = 0; i < size; i++)
    {
        sum += array[i];
    }
    return sum;
}

double process_file(const char *file_path)
{
    FILE *my_file;
    char buffer[10000] = {-1};

    int values[10000];
    int value_count = 0;

    // Opening file in reading mode
    my_file = fopen(file_path, "r");

    if (NULL == my_file)
    {
        perror("Error when opening the file");
        return -1;
    }

    int end = 0;
    int start = 0;
    // Reading a line from the file
    if (fgets(buffer, sizeof(buffer), my_file) != NULL)
    {

        for (int i = 0; buffer[i] != '\0'; i++)
        {
            if (buffer[i] != ' ')
            {
                if (buffer[i] == '\t')
                    start = i;
            }
            else
            {
                end = i;

                // Create the four didget string and convert it to int
                char four_didget_int[5];
                for (int j = start + 1, k = 0; j < end; j++, k++)
                    four_didget_int[k] = buffer[j];

                four_didget_int[4] = '\0';

                // Convert the string to an integer
                int value = atoi(four_didget_int);

                // Add the value to the array
                values[value_count++] = value;
            }
        }
    }
    else
    {
        perror("Error reading from the file");
    }

    // Closing the file
    fclose(my_file);

    double average = sum(values, value_count) / value_count;

    return average;
}

int main(void)
{
    double average = process_file("/home/matthias/WS_all/C/c4everyone/week5/elephant_seal_data.txt");

    printf("Average weight is %f \n", average);

    return 0;
}
