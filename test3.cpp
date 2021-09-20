#include <iostream>
using namespace std;

int main()
{
    int i = 1; // int int int
    double j = 0;
    long f;
    cout << "switch" << endl;
    switch (i)
    {
    case 0:
        break;
    case 1:
        break;
    case 2:
        break;
    default:
        break;
    }
    switch (j)
    {
    case 0:
        break;
    case 1:
        break; //switch switch
    default:
        break;
    }
    if (i < 0)
    {
        if (i < -1)
        {
        };
        else
        {
        };
    }
    else if (i > 0)
    {
        if (i > 2)
        {
        };
        else if (i == 2)
        {
        };
        else if (i > 1)
        {
        };
        else
        {
        };
    }
    else
    {
        if (j != 0)
        {
        }
        else
        {
        }
    }

    return 0;
}