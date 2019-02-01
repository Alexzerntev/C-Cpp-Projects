#include <iostream>
#include <vector>
#include "./include/gameplay.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    Gameplay *gameplay = new Gameplay();
    while (gameplay->ShowOptions())
    {
    }

    delete (gameplay);
    return 0;
}
