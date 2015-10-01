#include "inline.h"

int main(int argc, const char *argv[])
{
    CPeople cI(10, false, NULL);

    cI.SetHouse(true);
    cI.Show();
    return 0;
}
