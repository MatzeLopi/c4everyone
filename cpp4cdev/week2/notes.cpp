// This file contains some notes and things I wanted to try from the course

#include <iostream>

using namespace std;

template <class summable>
summable sum(const summable data[], int size, summable s = 0)
{
    for (int i = 0; i < size; i++)
        s += data[i];

    return s;
}

template <class substractable>
substractable subs(const substractable data[], int size, substractable s = 0)
{

    for (int i = 0; i < size; i++)
        s -= data[i];

    return s;
}

template <class multiplyable>
multiplyable multi(const multiplyable data[], int size, multiplyable s = 1)
{
    for (int i = 0; i < size; i++)
        s *= data[i];

    return s;
}

int main()
{
    int array[] = {1, 2, 3, 4, 5};
    int size = 5;
    cout << sum(array, size) << endl;
    cout << subs(array, size) << endl;
    cout << multi(array, size) << endl;
}