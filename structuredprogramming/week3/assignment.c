/*Assignment week3: Option 2

Task:
    - Develop a struct that can store an atomic element.
        - Fields: Name, Weight, Symbol

    - Create first 10 elements from terminal input as a linked list
    - Print data in nice and readable way

Last edited: 14/MAR/2024
*/

#include <stdio.h>
#include <stdlib.h>

// Typedef of an periodic element
typedef struct element
{
    char name[20];
    char symbol[3];
    float weight;
} element;

// Typdef of a list of periodic elements
typedef struct node
{
    element data_;
    struct node *next;
} node;

// Function to check if list is empty
int is_empty(const node *l) { return (l == NULL); }

// Function to create a list element
node *create_list(element data)
{
    node *head = malloc(sizeof(node));
    head->data_ = data;
    head->next = NULL;
    return head;
}

// Function to link a list
node *prepend(element data, node *linked_list)
{
    node *new_head = create_list(data);
    new_head->next = linked_list;
    return new_head;
}

void print_list(node *element_list)
{
    printf("%-20s|%-20s|%-20s\n", "Element", "Symbol", "Weight");
    printf("--------------------------------------------------\n");
    while (element_list != NULL)
    {
        element data = element_list->data_;
        printf("%-20s|%-20s|%-20f\n", data.name, data.symbol, data.weight);
        element_list = element_list->next;
    }
}

// Main function
int main(void)
{
    int i = 0;
    int number_elements;
    node *element_list = NULL;

    // Ask user for input
    printf("How many periodic elements should be created?\n");
    scanf("%d", &number_elements);

    // Get as many elements as defined
    while (i < number_elements)
    {

        element new_element;

        // Get name
        printf("Please input name for the %d element.\n", i);
        scanf("%s", new_element.name);

        // Get symbol
        printf("Please input symbol for %s.\n", new_element.name);
        scanf("%2s", new_element.symbol);

        // Get weight
        printf("Please input weight for the %s\n", new_element.name);
        scanf("%f", &new_element.weight);

        element_list = prepend(new_element, element_list);

        i++;
    }
    print_list(element_list);

    return 0;
}