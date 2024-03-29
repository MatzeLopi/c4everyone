/* Honor assignment of week3

Tasks:
    - Modify the singly linked list to be a doubly linked list.
    - Now write a routine that removes all duplicate data in the doubly linked list.
        - The data will be integers generated at random from [0,49].
        - Initially have a list of 200 elements.

    - Now do this in one of two ways.
        - Sort the list by its data field. Remove adjacent elements of the sorted list with the same value.
        - Or, take the first element—search the remainder of the list for elements with the same data and remove them.
            Then take the second element and do the same. Repeat until only one element is left.

Last edited: 14/MAR/2024
*/

#include <stdlib.h>
#include <stdio.h>

typedef struct node
{
    struct node *previous;
    int data;
    struct node *next;
} node;

// Simple random generator to return int in range [lower,upper]
int rand_range(int lower, int upper)
{
    return (rand() % (upper - lower + 1)) + lower;
}

// create new list element
node *new_element(int data)
{
    node *linked_list = malloc(sizeof(node));
    linked_list->data = data;
    linked_list->previous = NULL;
    linked_list->next = NULL;

    return linked_list;
}

// prepend element to a list
node *prepend(node *linked_list, int data)
{
    node *new_head = new_element(data);
    new_head->next = linked_list;
    linked_list->previous = new_head;
    return new_head;
}

// Append a new list element to an existing linked list
node *append(node *linked_list, int data)
{
    node *new_tail = new_element(data);
    node *head = linked_list;

    while (linked_list != NULL)
        linked_list = linked_list->next;

    new_tail->previous = linked_list;
    linked_list->next = new_tail;

    return head;
}

// Create a double linked list from array
node *from_array(int array[], int size)
{
    int i = 1;

    node *head = new_element(array[0]);

    while (i < size)
    {
        head = prepend(head, array[i]);
        i++;
    }
    return head;
}

// Function to print list
void print_list(node *linked_list)
{
    int i = 0;

    while (linked_list != NULL)
    {
        printf("%d, ", linked_list->data);
        linked_list = linked_list->next;
        i++;
    }
    printf("\n");
}

// remove dublicate values from list
node *remove_dublicates(node *head)
{
    node *current = head;
    while (current->next != NULL)
    {
        if (current->data == current->next->data)
        {
            node *temp = current->next;
            current->next = temp->next;
            if (temp->next != NULL)
                temp->next->previous = current;
            free(temp);
        }
        else
            current = current->next;
    }
    return head;
}

// Sort a doubly linked list using bubble sort
node *sort_list(node *head)
{
    if (head == NULL || head->next == NULL)
        return head;

    int swapped;
    node *last = NULL;
    while (swapped)
    {
        swapped = 0;
        node *current = head;
        while (current->next != last)
        {
            if (current->data > current->next->data)
            {
                // Swap data
                int temp = current->data;
                current->data = current->next->data;
                current->next->data = temp;
                swapped = 1;
            }
            current = current->next;
        }
        last = current;
    };

    return remove_dublicates(head);
}

// Main
int main(void)
{
    const int SIZE = 200;
    int array[SIZE];

    for (int i = 0; i < SIZE; i++)
    {
        array[i] = rand_range(0, 49);
    }

    node *linked_list = from_array(array, SIZE);
    linked_list = sort_list(linked_list);
    print_list(linked_list);
    return 0;
}