#include"body.h"

int main()
{
    Body b;
    b.spin_x = 1.5;
    b.print_matrix();
    b.update(0.2);
    b.print_matrix();

    return 0;
}
