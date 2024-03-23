// This file contains some notes and things I wanted to try from the course

#include <iostream>

using namespace std;

// 2D Point
class point
{
public:
    // Constructor wit initializer list
    point(double x = 0.0, double y = 0.0) : x(x), y(y) {}

    double getx() { return x; }
    void setx(double value) { x = value; }
    double gety() { return y; }
    void sety(double value) { y = value; }

    friend point operator+(const point &p1, const point &p2);
    friend point operator-(const point &p1, const point &p2);
    friend ostream &operator<<(ostream &out, const point &p);

private:
    double x, y;
};

// + Operator for two points
point operator+(const point &p1, const point &p2)
{
    point sum = {p1.x + p2.x, p1.y + p2.y};
    return sum;
}

point operator-(const point &p1, const point &p2)
{
    point subs = {p1.x - p2.x, p1.y - p2.y};
    return subs;
}

// Idiom for custom out
ostream &operator<<(ostream &os, const point &p)
{
    os << '(' << p.x << ", " << p.y << ')';
    return os;
}

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

// Template for miltiplying all values in an array
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

    point p1 = {1, 2};
    cout << p1 << endl;
    point p2 = {2, 3};
    cout << p2 << endl;

    point p3 = p1 + p2;
    cout << p3 << endl;
}