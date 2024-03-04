/*Assignment week2: Option 2

Task:
    - Write a struct that has:
        1. A member that is the name of a business department:
            HR, Sales, Research, Software, and Executive (use enums for the 4 departments).
        2. A member that is an annual salary as an int.
        3. A member that is a social security number(unsigned).

    - Generate 10 employees with different social security numbers.
    - Invent a salary generator for each department (use some base salary for the department and add in a randomly generated offset).
    - Print out the 10 employees and their values to the screen-one line at a time.



Last edited: 02/MAR/2024

*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Base Salary per department

// Declaring constants
const int DEPARTMENTS = 5;
const int MAX_DEPARTMENTS = (DEPARTMENTS - 1);
const int BASE_SALARIES[5] = {30000, 35000, 28000, 50000, 80000};

// Department type holds info which departments exist
typedef enum department
{
    HR,
    SALES,
    RESEARCH,
    SOFTWARE,
    EXECUTIVE
} department;

// employee combines some relevant information about an employee into one datastructure
typedef struct employee
{
    department department;
    int salery;
    unsigned int social_security_number;
} employee;

// Simple random generator to return int in range [lower,upper]
int rand_range(int lower, int upper)
{
    return (rand() % (upper - lower + 1)) + lower;
}

// Function to generate a salary depending on the department
int salery_generator(department department)
{
    int salery = BASE_SALARIES[department] * (1 + rand_range(0, 10) / 10.0);
    return salery;
}

// Function to populate an array of employees
void employee_generator(employee employees[], int number_employees)
{
    int i = 0;
    while (i < number_employees)
    {

        // Create new employee and assign values
        employee new_employee =
            {
                rand_range(0, MAX_DEPARTMENTS),                                  // Randomly assigns one of the five departments
                new_employee.salery = salery_generator(new_employee.department), // Generate Salary based on department
                rand_range(10000000, 99000000),                                  // Randomly assigns a number between 10 and 99 Mio
            };

        // Store new employee in array
        employees[i] = new_employee;

        i++;
    }
}

// Function to print the department name instead of index
void print_department(department department)
{
    switch (department)
    {
    case HR:
        printf("HR");
        break;
    case SALES:
        printf("Sales");
        break;
    case RESEARCH:
        printf("Research");
        break;
    case SOFTWARE:
        printf("Software");
        break;
    case EXECUTIVE:
        printf("Executive");
        break;
    default:
        printf("Undefined");
        break;
    }
}

// Function to print an employee
void print_employee(employee employee)
{
    printf(
        "Employee with Social Securety number: %d, earns: %d, and works in ",
        employee.social_security_number, employee.salery);
    print_department(employee.department);
    printf("\n");
}

// Main
int main(void)
{
    srand(time(0)); // Set key for pseudo random generator

    int i = 0;

    int number_employees = 10; // Change number of employees here
    employee employees[number_employees];

    employee_generator(employees, number_employees); // Generate employees

    while (i < number_employees)
    {
        print_employee(employees[i]);
        i++;
    }

    return 0;
}