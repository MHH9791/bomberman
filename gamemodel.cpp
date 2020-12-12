#include "gamemodel.h"

gameModel::gameModel(QString map, unsigned int nrOfEnermy, unsigned int nrOfHealthPack, float pRatio)
    :World()
{
    createWorld(map,nrOfEnermy,nrOfHealthPack,pRatio);
    protagonist=this->getProtagonist();
    model_tiles=getTiles();
    model_enemies=getEnemies();
    model_healthPacks=getHealthPacks();
}

gameModel::gameModel()
{

}

unique_ptr<Protagonist>& gameModel::get_model_protg()
{
    return protagonist;
}

std::vector<std::unique_ptr<Tile>>& gameModel::get_model_tiles()
{
    return model_tiles;
}
std::vector<std::unique_ptr<Enemy>>& gameModel::get_model_enemies()
{
    return model_enemies;
}
std::vector<std::unique_ptr<Tile>>& gameModel::get_model_healthPacks()
{
    return model_healthPacks;
}

std::vector<std::unique_ptr<Tile> >& gameModel::getPoisoned_tiles()
{
    return poisoned_tiles;
}

std::vector<std::unique_ptr<Tile> >& gameModel::getBlocked_tiles()
{
    return blocked_tiles;
}

//*****************Add new poisoned tiles into vector*****************
void gameModel::addPoisonedTile(int x, int y)
{
    bool flag=true;
    bool result=checkEmpty(x,y);
    if(result)
    {
        //if this tile is already poisoned, then increase the poisonLevel
        for(auto& a:poisoned_tiles)
        {
            if(a->getXPos()==x&&a->getYPos()==y)
            {
                a->setValue(a->getValue()+PoisonValue);
                flag=false;
                break;
            }
        }

        if(flag)
        {
            auto newPoisonedTile=make_unique<Tile>(x,y,PoisonValue);
            poisoned_tiles.push_back(move(newPoisonedTile));
        }
    }
}

//*****************Add new blocked tiles into vector*****************
bool gameModel::addBlockedTile(int x,int y)
{
    bool flag=true;
    bool result=checkEmpty(x,y);
    if(result)
    {
        for(auto& a:blocked_tiles)
        {
            if(a->getXPos()==x&&a->getYPos()==y)
            {
                flag=false;
                break;
            }
        }

        if(flag)
        {

            auto newBlockedTile=make_unique<Tile>(x,y,numeric_limits<float>::infinity());
            blocked_tiles.push_back(move(newBlockedTile));
        }
    }
    return result;
}

//*****************Check whether the given location is empty*****************
//*****************true means there is position is empty*****************
bool gameModel::checkEmpty(int x, int y) const
{
    bool result=true;
    for(auto& a:model_enemies)
    {
        if(a->getXPos()==x&&a->getYPos()==y)
        {
            result=false;
            break;
        }
    }
    for(auto& a:model_healthPacks)
    {
        if(a->getXPos()==x&&a->getYPos()==y)
        {
            result=false;
            break;
        }
    }
    return result;
}

//*****************Sort enemy based on the distance from current location of protagonist*****************
void gameModel::sortEnemy(int protg_x, int protg_y)
{
    sort(model_enemies.begin(),model_enemies.end(),[=](auto& e1,auto& e2)
    {return sqrtf((e1->getXPos() - protg_x)*(e1->getXPos() - protg_x) + (e1->getYPos() - protg_y)*(e1->getYPos() - protg_y))<sqrtf((e2->getXPos() - protg_x)*(e2->getXPos() - protg_x) + (e2->getYPos() - protg_y)*(e2->getYPos() - protg_y));});
}

//*****************Find the strength of the nearest enemy*****************
float gameModel::findNearestEnemyValue(int protg_x, int protg_y)
{
    for(auto& e:get_model_enemies())
    {
        if(!e->getDefeated())
        {
            return e->getValue();
        }
    }
    return -1;
}

//*****************Find the strength of the second nearest enemy*****************
//*****************Return -1 if only one enemy is alive*****************
float gameModel::findSecondNearestEnemyValue(int protg_x, int protg_y)
{
    int count=0;
//    sortEnemy(protg_x, protg_y);
    for(auto& e:get_model_enemies())
    {
        if(!e->getDefeated())
        {
            count++;
            if(count==2)
                return e->getValue();
        }
    }
    return -1;

}

//*****************Find the strength of the third nearest enemy*****************
//*****************Return -1 if only two enemy are alive*****************
float gameModel::findThirdNearestEnemyValue(int protg_x, int protg_y)
{
    int count=0;
//    sortEnemy(protg_x, protg_y);
    for(auto& e:get_model_enemies())
    {
        if(!e->getDefeated())
        {
            count++;
            if(count==3)
                return e->getValue();
        }
    }
    return -1;
}


