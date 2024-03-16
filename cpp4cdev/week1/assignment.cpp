/*   Convert this program to C++

*   change to C++ io

*   change to one line comments

*   change defines of constants to const

*   change array to vector<>

*   inline any short function

*/

#include <iostream>
using namespace std;

const int N = 40;

void sum(int *p, int n, int d[])
{
    *p = 0;
    for (int i = 0; i < n; ++i)
        *p += d[i];
}

int main(void)
{
    int i;
    int accum = 0;
    int data[N];
    for (i = 0; i < N; ++i)
        data[i] = i;
    sum(&accum, N, data);
    cout << "Sum is " << accum << endl;

    return 0;
}