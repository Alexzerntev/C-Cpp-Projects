#include "../../include/Grid/common_square.hpp"
#include "../../include/enums.hpp"
#include "../../include/Livings/Monsters/monster.hpp"
#include "../../include/Livings/Monsters/dragon.hpp"
#include "../../include/Livings/Monsters/exoskeleton.hpp"
#include "../../include/Livings/Monsters/spirit.hpp"

using namespace std;

CommonSquare::CommonSquare(Monster *monster)
{
    this->monsters_on_squere.push_back(monster);
}
CommonSquare::CommonSquare()
{
}
CommonSquare::~CommonSquare() {}

void CommonSquare::print_monsters()
{
    for (int i = 0; i < this->monsters_on_squere.size(); i++)
    {
        if (Exoskeleton *excs = dynamic_cast<Exoskeleton *>(this->monsters_on_squere[i]))
        {
            cout << "Exoskeleton named " << excs->get_name() << endl;
        }
        if (Dragon *dragon = dynamic_cast<Dragon *>(this->monsters_on_squere[i]))
        {
            cout << "Dragon named " << dragon->get_name() << endl;
        }
        if (Spirit *spirit = dynamic_cast<Spirit *>(this->monsters_on_squere[i]))
        {
            cout << "Spirit named " << spirit->get_name() << endl;
        }
    }
}