#include"object.h"
#include<iostream>

using namespace std;

int main()
{
    Object o;
    o.print_matrix();
    o.translate(0.32, 0, 4);
    o.print_matrix();
    return 0;
}
