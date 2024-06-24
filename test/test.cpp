#include <iostream>

using namespace std;

class m
{
    public:
    m() { cout << "mother" << endl; }
};

class n : m
{
    public:
    n() { cout << "daughter" << endl; }
};

int main()
{
    m M;
    n N;
}