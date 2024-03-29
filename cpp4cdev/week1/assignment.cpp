// Convert this program to C++
// change to C++ io
// change to one line comments
// change defines of constants to const
// change array to vector<>
// inline any short function

#include <iostream>
#include <vector>
#include <numeric>

using namespace std;

const int N = 40;

// Calculate sum of vector using build in function
inline int sum_vector(const vector<int> &data)
{
    return accumulate(data.begin(), data.end(), 0);
}

// Changed main to automatically fill vector using build in function
int main()
{
    vector<int> data(N);
    iota(data.begin(), data.end(), 0);

    int sum = sum_vector(data);
    cout << "Sum is " << sum << endl;

    return 0;
}