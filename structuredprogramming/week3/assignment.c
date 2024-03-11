/*Assignment week3: Option 2

Task:
    - Develop a struct that can store an atomic element.
        - Fields: Name, Weight, Symbol

    - Create first 10 elements from terminal input as a linked list
    - Print data in nice and readable way

Last edited: 11/MAR/2024
*/

#include <stdio.h>
#include <stdlib.h>

// Typedef of an periodic element
typedef struct element
{
    char *name;
    float weight;
    char symbol;
} element;

// Typdef of a list of periodic elements
typedef struct list
{
    element data_;
    struct list *next;
} list;

// Function to check if list is empty
int is_empty(const list *l) { return (l == NULL); }

// Function to create a list element
list *create_list(element data)
{
    list *head = malloc(sizeof(list));
    head->data_ = data;
    head->next = NULL;
    return head;
}

// Function to link a list
list *prepend(element data, list *linked_list)
{
    list *new_head = create_list(data);
    new_head->next = linked_list;
}

// Main function
int main(void)
{
    int number_elements;
    int i = 0;
    printf("How many periodic elements should be created?\n");
    scanf("%d", &number_elements);

    while (i < number_elements)
    {
        float weight;
        char symbol;
        char *name;
        printf("\n Please input name, symbol and weight for the %d element.\n Seperate with ','.\n", i);

        scanf("%s, %c, %f", name, &symbol, &weight);
        element new_element = {
            name,
            symbol,
            weight,
        };
        if (i == 0)
        {
            list *list = create_list(new_element);
        }
        else
        {
            list *list = prepend(new_element, list);
        };

        i++;
    }
    printf("Success");

    return 0;
}