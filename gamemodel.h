#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include <world.h>
#include <math.h>

const float RecoverValue{200.0f};
const float PoisonValue{5.0f};
using namespace std;
class gameModel:public World
{
public:
    gameModel();
    gameModel(QString background, unsigned int nrOfEnermy, unsigned int nrOfHealthPack, float pRatio);
    void addPoisonedTile(int x,int y);
    bool addBlockedTile(int x,int y);
    bool checkEmpty(int x, int y) const;
    float findNearestEnemyValue(int protg_x, int protg_y);
    float findSecondNearestEnemyValue(int protg_x, int protg_y);
    float findThirdNearestEnemyValue(int protg_x, int protg_y);
    unique_ptr<Protagonist>& get_model_protg();
    std::vector<std::unique_ptr<Tile>>& get_model_tiles() ;
    std::vector<std::unique_ptr<Enemy>>& get_model_enemies() ;
    std::vector<std::unique_ptr<Tile>>& get_model_healthPacks() ;
    std::vector<std::unique_ptr<Tile> >& getPoisoned_tiles() ;
    std::vector<std::unique_ptr<Tile> >& getBlocked_tiles() ;
    void sortEnemy(int protg_x,int protg_y);

private:
    //these variables are used to get the ownership of unique pointers
    unique_ptr<Protagonist> protagonist;
    std::vector<std::unique_ptr<Tile>> model_tiles;
    std::vector<std::unique_ptr<Enemy>> model_enemies;
    std::vector<std::unique_ptr<Tile>> model_healthPacks;
    std::vector<std::unique_ptr<Tile>> poisoned_tiles;
    std::vector<std::unique_ptr<Tile>> blocked_tiles;


};

#endif // GAMEMODEL_H
