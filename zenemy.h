#ifndef ZENEMY_H
#define ZENEMY_H
#include "world.h"

//ZEnemy is a kind of new enemy. Once it is attacked, the three tiles on the left and
//three tiles on the right are unallowed to pass
//strength of Zenemy is a random number in the range of (0,50)
class ZEnemy: public Enemy
{
public:
    ZEnemy(int xPosition, int yPosition, float strength);
};

#endif // ZENEMY_H
