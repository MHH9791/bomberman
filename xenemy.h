#ifndef XENEMY_H
#define XENEMY_H
#include "world.h"
#include "gamemodel.h"
const float XEnemyStrength{30.0f};

//XEnemy would randomly move to other empty tiles once it is attacked until it is attacked three times
class XEnemy: public Enemy
{
public:
    XEnemy(int xPosition, int yPosition, float strength, shared_ptr<gameModel> model);
    int getPrevious_X() const;

    int getPrevious_Y() const;

public slots:
    bool random_move();
private:
    shared_ptr<gameModel> model;
    int attacked_times {0};
    int previous_X;
    int previous_Y;
};

#endif // XENEMY_H
