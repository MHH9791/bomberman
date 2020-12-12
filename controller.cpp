#include "controller.h"
#include "template.h"
#include "world.h"
#include "gamemodel.h"
using namespace std;
controller::controller(){
    mainWindow=make_shared<MainWindow>();
    mainWindow->show();

    startGame();
}

//*****************Start or restart game*****************
void controller::startGame()
{
    gameover_flag=false;
    mainWindow->setToggle(false);
    mainWindow->showGraphicView();
    mainWindow->setEndAutoplay(false);
    nrOfEnermy=5;
    pRatio=0.25;
    nrOfHealthPack=5;
    nrOfXEnemy=1;
    nrOfZEnemy=1;
    maxE=300.0f;
    maxH=300.0f;
    initSetting();
    QString map=mainWindow->getMap();
    QString s=":/image/";
    s.append(map);
    worldMap=s;
    createWorldModel();
    checkMap(map);
    worldModel->get_model_protg()->setHealth(maxH);
    worldModel->get_model_protg()->setEnergy(maxE);
    for(unsigned int i=0;i<nrOfXEnemy;i++)
    {
        insertNewXEnemy();
    }
    for(unsigned int i=0;i<nrOfZEnemy;i++)
    {
        insertNewZEnemy();
    }
    mainWindow->init(worldModel,this);
    pf=make_shared<pathFinder>(worldModel);
    pf->initPathFinder();
    signal_slot_init();
    mainWindow->showWelcomeMsg();
}

//*****************Change initial location of protagonist based on map*****************
void controller::checkMap(QString map)
{
    if(map=="maze1.png")
    {
        worldModel->get_model_protg()->setXPos(4);
        worldModel->get_model_protg()->setYPos(4);
    }
    else if(map=="maze2.png")
    {
        worldModel->get_model_protg()->setXPos(6);
        worldModel->get_model_protg()->setYPos(6);
    }
}

//*****************Initial game setting*****************
void controller::initSetting()
{
    if(mainWindow->getEnemyNr()>=0)
        nrOfEnermy=mainWindow->getEnemyNr();
    if(mainWindow->getPratio()>=0&&mainWindow->getPratio()<1)
        pRatio=mainWindow->getPratio();
    if(mainWindow->getXEnemyNr()>=0)
        nrOfXEnemy=mainWindow->getXEnemyNr();
    if(mainWindow->getZEnemyNr()>=0)
        nrOfZEnemy=mainWindow->getZEnemyNr();
    if(mainWindow->getHealthPackNr()>=0)
        nrOfHealthPack=mainWindow->getHealthPackNr();
    if(mainWindow->getMaxHealth()>0)
        maxH=mainWindow->getMaxHealth();
    if(mainWindow->getMaxEnegy()>0)
        maxE=mainWindow->getMaxEnegy();

}

controller::controller(unsigned int nrOfEnermy, float pRatio, unsigned int nrOfHealthPack, unsigned int nrOfXEnemy, unsigned int nrOfZEnemy)
    :nrOfEnermy{nrOfEnermy}, pRatio{pRatio}, nrOfHealthPack{nrOfHealthPack}
{
    worldMap=":/image/map_4.jpg";
    mainWindow=make_shared<MainWindow>();
    createWorldModel();
    for(unsigned int i=0;i<nrOfXEnemy;i++)
    {
        insertNewXEnemy();
    }
    for(unsigned int i=0;i<nrOfZEnemy;i++)
    {
        insertNewZEnemy();
    }
    mainWindow->init(worldModel,this);
    mainWindow->show();
    signal_slot_init();
    mainWindow->showWelcomeMsg();

}

//*****************Insert new XEnemy to empty tiles based on the number of X enemies defined in game setting*****************
void controller::insertNewXEnemy()
{
    int random_x,random_y;
    int flag=true;
    int continue_flag;
    while(flag)
    {
        continue_flag=false;
        random_x=rand()%worldModel->getCols();
        random_y=rand()%worldModel->getRows();
        for(auto& a:worldModel->get_model_enemies())
        {
            if(random_x==a->getXPos()&&random_y==a->getYPos())
            {
                continue_flag=true;
                break;
            }
        }
        if(continue_flag)
            continue;
        for(auto& a:worldModel->get_model_healthPacks())
        {
            if(random_x==a->getXPos()&&random_y==a->getYPos())
            {
                continue_flag=true;
                break;
            }
        }
        if(continue_flag)
            continue;
        if(random_x==worldModel->get_model_protg()->getXPos()&&random_y==worldModel->get_model_protg()->getYPos())
        {
            continue;
        }
        flag=false;
    }
    auto new_X=make_unique<XEnemy>(random_x,random_y,XEnemyStrength,worldModel);
    worldModel->get_model_enemies().push_back(move(new_X));
}


//*****************Insert new ZEnemy to empty tiles based on the number of Z enemies defined in game setting*****************
void controller::insertNewZEnemy()
{
    int random_x,random_y;
    int flag=true;
    int continue_flag;
    while(flag)
    {
        continue_flag=false;
        random_x=rand()%worldModel->getCols();
        random_y=rand()%worldModel->getRows();
        for(auto& a:worldModel->get_model_enemies())
        {
            if(random_x==a->getXPos()&&random_y==a->getYPos())
            {
                continue_flag=true;
                break;
            }
        }
        if(continue_flag)
            continue;
        for(auto& a:worldModel->get_model_healthPacks())
        {
            if(random_x==a->getXPos()&&random_y==a->getYPos())
            {
                continue_flag=true;
                break;
            }
        }
        if(continue_flag)
            continue;
        if(random_x==worldModel->get_model_protg()->getXPos()&&random_y==worldModel->get_model_protg()->getYPos())
        {
            continue;
        }
        flag=false;
    }
    auto new_Z=make_unique<ZEnemy>(random_x,random_y,rand()%50);
    worldModel->get_model_enemies().push_back(move(new_Z));
}

void controller::createWorldModel()
{

    worldModel= make_shared<gameModel>(worldMap, nrOfEnermy, nrOfHealthPack, pRatio);

}

//*****************Initialize connection between signals and slots*****************
void controller::signal_slot_init()
{
    connect(worldModel->get_model_protg().get(),SIGNAL(posChanged(int,int)),mainWindow.get(),SLOT(update_protg_position(int,int)));
    for(auto& e:worldModel->get_model_enemies())
    {
        connect(e.get(),SIGNAL(dead()),mainWindow.get(),SLOT(update_dead_enemy()));
    }
    connect(worldModel->get_model_protg().get(),SIGNAL(healthChanged(int)),mainWindow.get(),SLOT(update_protg_health(int)));
    connect(worldModel->get_model_protg().get(),SIGNAL(energyChanged(int)),mainWindow.get(),SLOT(update_protg_energy(int)));

}

QString controller::getWorldMap() const
{
    return worldMap;
}

controller::~controller(){

}

shared_ptr<gameModel> controller::getWorld() const
{
    return worldModel;
}

void controller::moveUp()
{
    mainWindow->clearMsg();
    if(checkEnemyAlive()&&checkIsAlive()&&possibleToWin())
    {
        int x_pos = worldModel->get_model_protg()->getXPos();
        int y_pos = worldModel->get_model_protg()->getYPos();
        if(!isBlocked(x_pos,y_pos-1)&&!isBlack(x_pos,y_pos-1))
        {
            bool isEnemy=attackEnemy(x_pos,y_pos-1);
            if(!isEnemy)
            {
                if(y_pos-1 >= 0)
                {
                    bool r=meetHealthPack(x_pos,y_pos-1);

                    mainWindow->updateOldlocation(worldModel->get_model_protg()->getXPos(),worldModel->get_model_protg()->getYPos());
                    worldModel->get_model_protg()->setPos(x_pos,y_pos-1);
                    if(r)
                    {
                        mainWindow->update_health_protg(x_pos,y_pos-1);
                        mainWindow->updateTextView();
                    }
                    consumeEnergy(x_pos,y_pos-1);
                    getPoisoned(x_pos,y_pos-1);
                }
                else
                    mainWindow->showOutOfBoundaryMsg();
            }
        }
        else
            mainWindow->showBlockedMsg();
    }
}

void controller::moveDown()
{
    mainWindow->clearMsg();
    if(checkEnemyAlive()&&checkIsAlive()&&possibleToWin())
    {
        int x_pos = worldModel->get_model_protg()->getXPos();
        int y_pos = worldModel->get_model_protg()->getYPos();
        if(!isBlocked(x_pos,y_pos+1)&&!isBlack(x_pos,y_pos+1))
        {
            bool isEnemy=attackEnemy(x_pos,y_pos+1);
            if(!isEnemy)
            {
                if(y_pos+1 < worldModel->getRows())
                {
                    bool r=meetHealthPack(x_pos,y_pos+1);
                    mainWindow->updateOldlocation(worldModel->get_model_protg()->getXPos(),worldModel->get_model_protg()->getYPos());
                    worldModel->get_model_protg()->setPos(x_pos,y_pos+1);
                    if(r)
                    {
                        mainWindow->update_health_protg(x_pos,y_pos+1);
                        mainWindow->updateTextView();
                    }
                    consumeEnergy(x_pos,y_pos+1);
                    getPoisoned(x_pos,y_pos+1);
                }
                else
                    mainWindow->showOutOfBoundaryMsg();
            }
        }
        else
            mainWindow->showBlockedMsg();
    }
}

void controller::moveLeft()
{
    mainWindow->clearMsg();
    if(checkEnemyAlive()&&checkIsAlive()&&possibleToWin())
    {
        int x_pos = worldModel->get_model_protg()->getXPos();
        int y_pos = worldModel->get_model_protg()->getYPos();
        if(!isBlocked(x_pos-1,y_pos)&&!isBlack(x_pos-1,y_pos))
        {
            bool isEnemy=attackEnemy(x_pos-1,y_pos);
            if(!isEnemy)
            {
                if(x_pos-1 >= 0)
                {
                    bool r=meetHealthPack(x_pos-1,y_pos);
                    mainWindow->updateOldlocation(worldModel->get_model_protg()->getXPos(),worldModel->get_model_protg()->getYPos());
                    worldModel->get_model_protg()->setPos(x_pos-1,y_pos);
                    if(r)
                    {
                        mainWindow->update_health_protg(x_pos-1,y_pos);
                        mainWindow->updateTextView();
                    }
                    consumeEnergy(x_pos-1,y_pos);
                    getPoisoned(x_pos-1,y_pos);
                }
                else
                    mainWindow->showOutOfBoundaryMsg();
            }
        }
        else
            mainWindow->showBlockedMsg();
    }
}

void controller::moveRight()
{
    mainWindow->clearMsg();
    if(checkEnemyAlive()&&checkIsAlive()&&possibleToWin())
    {
        int x_pos = worldModel->get_model_protg()->getXPos();
        int y_pos = worldModel->get_model_protg()->getYPos();
        if(!isBlocked(x_pos+1,y_pos)&&!isBlack(x_pos+1,y_pos))
        {
            bool isEnemy=attackEnemy(x_pos+1,y_pos);
            if(!isEnemy)
            {
                if(x_pos+1 < worldModel->getCols())
                {
                    bool r=meetHealthPack(x_pos+1,y_pos);
                    mainWindow->updateOldlocation(worldModel->get_model_protg()->getXPos(),worldModel->get_model_protg()->getYPos());
                    worldModel->get_model_protg()->setPos(x_pos+1,y_pos);
                    if(r)
                    {
                        mainWindow->update_health_protg(x_pos+1,y_pos);
                        mainWindow->updateTextView();
                    }
                    consumeEnergy(x_pos+1,y_pos);
                    getPoisoned(x_pos+1,y_pos);
                }
                else
                    mainWindow->showOutOfBoundaryMsg();
            }
        }
        else
            mainWindow->showBlockedMsg();
    }
}

//*****************Check whether the protagonist is alive by checking both health and energy*****************
bool controller::checkIsAlive()
{
    if(worldModel->get_model_protg()->getEnergy() <= 0 || worldModel->get_model_protg()->getHealth() <=0 )
    {
        mainWindow->showNormalOverMsg();
        mainWindow->showTryAgainGif();
        gameover_flag=true;
        return false;
    }
    return true;
}

//*****************Check whether all enemies are defeated*****************
bool controller::checkEnemyAlive()
{
    for(auto & enemy: worldModel->get_model_enemies())
    {
        if(!enemy->getDefeated())
            return true;

    }
    mainWindow->showWinMsg();
    mainWindow->showCongratulationGif();
    gameover_flag=true;
    return false;
}

//*****************Decrease energy when protagonist moves to other tile*****************
void controller::consumeEnergy(int next_X, int next_Y)
{

    float required_energy{0};
    for(auto& a:getWorld()->get_model_tiles())
    {
        if(a->getXPos()==next_X&&a->getYPos()==next_Y)
            required_energy=a->getValue();
    }
    float current_energy=worldModel->get_model_protg()->getEnergy();
    worldModel->get_model_protg()->setEnergy(current_energy-required_energy);
    if(worldModel->get_model_protg()->getEnergy()<=0)
        mainWindow->showInsufficientEnergyMsg();
}

//*****************Return value represent whether an enemy is here no matther whether it is dead or alive*****************
//*****************if P enemy is attacked, these tiles around it would be poisoned*****************
//*****************if X enemy is attacked, it would randomly move to another location*****************
//*****************if Z enemy is attacked, six tiles which are on the same row of this enemy would be blocked*****************
bool controller::attackEnemy(int x_pos, int y_pos)
{
    bool result=false;
    float current_health=worldModel->get_model_protg()->getHealth();
    for(auto& a: worldModel->get_model_enemies())
    {
        //First find the enemy at this location
        if(a->getXPos()==x_pos&&a->getYPos()==y_pos)
        {
            result=true;
            if(a->getDefeated()==false)
            {
                //Check type of enemy to perform different operations
                if(typeid(*a)==typeid(PEnemy))
                {
                    float required_health=a->getValue();
                    if(required_health>current_health)
                    {
                        mainWindow->showInsufficientHealthMsg(required_health);
                    }

                    else{
                        auto raw_enemy_ptr=dynamic_cast<PEnemy*>(a.get());
                        raw_enemy_ptr->setDefeated(true);
                        worldModel->get_model_protg()->setHealth(current_health-required_health);
                        //restore energy after defeating an enermy
                        worldModel->get_model_protg()->setEnergy(maxE);
                        mainWindow->update_dead_enemy_g(x_pos,y_pos);
                        updatePosionedTile(x_pos,y_pos);
                        mainWindow->update_poisoned_tiles();
                        checkEnemyAlive();
                    }
                }
                else if(typeid(*a)==typeid(XEnemy))
                {
                    float required_health=a->getValue();
                    if(required_health>current_health)
                    {
                        mainWindow->showInsufficientHealthMsg(required_health);
                    }

                    else{
                        mainWindow->update_attacking_protg(worldModel->get_model_protg()->getXPos(),worldModel->get_model_protg()->getYPos());
                        auto raw_enemy_xtr=dynamic_cast<XEnemy*>(a.get());
                        bool ismoved=raw_enemy_xtr->random_move();
                        if(ismoved)
                        {
                            pf->nodes[raw_enemy_xtr->getPrevious_Y()*worldModel->getCols()+raw_enemy_xtr->getPrevious_X()]->wall=false;
                            pf->nodes[raw_enemy_xtr->getPrevious_Y()*worldModel->getCols()+raw_enemy_xtr->getPrevious_X()]->value=worldModel->get_model_tiles()[raw_enemy_xtr->getPrevious_Y()*worldModel->getCols()+raw_enemy_xtr->getPrevious_X()]->getValue();
                            pf->nodes[raw_enemy_xtr->getYPos()*worldModel->getCols()+raw_enemy_xtr->getXPos()]->wall=true;
                            pf->nodes[raw_enemy_xtr->getYPos()*worldModel->getCols()+raw_enemy_xtr->getXPos()]->value=raw_enemy_xtr->getValue();
                        }
                        else
                        {
                            pf->nodes[raw_enemy_xtr->getYPos()*worldModel->getCols()+raw_enemy_xtr->getXPos()]->wall=true;
                        }

                        if(!raw_enemy_xtr->getDefeated())
                        {
                            mainWindow->update_XEnemy(raw_enemy_xtr);
                        }
                        else{
                            mainWindow->update_dead_enemy_g(x_pos,y_pos);
                        }
                        worldModel->get_model_protg()->setHealth(current_health-required_health);
                        //restore energy after defeating an enermy
                        worldModel->get_model_protg()->setEnergy(maxE);

                        checkEnemyAlive();
                    }
                }
                else if(typeid(*a)==typeid(ZEnemy))
                {
                    float required_health=a->getValue();
                    if(required_health>current_health)
                    {
                        mainWindow->showInsufficientHealthMsg(required_health);
                    }

                    else{

                        auto raw_enemy_ztr=dynamic_cast<ZEnemy*>(a.get());
                        raw_enemy_ztr->setDefeated(true);
                        worldModel->get_model_protg()->setHealth(current_health-required_health);
                        //restore energy after defeating an enermy
                        worldModel->get_model_protg()->setEnergy(maxE);
                        mainWindow->update_dead_enemy_g(x_pos,y_pos);
                        updateBlockedTile(x_pos,y_pos);
                        mainWindow->update_blocked_tiles();

                        checkEnemyAlive();
                    }
                }
                else
                {
                    float required_health=a->getValue();
                    if(required_health>current_health)
                    {
                        mainWindow->showInsufficientHealthMsg(required_health);
                    }
                    else
                    {
                        mainWindow->update_attacking_protg(worldModel->get_model_protg()->getXPos(),worldModel->get_model_protg()->getYPos());
                        worldModel->get_model_protg()->setHealth(current_health-required_health);
                        worldModel->get_model_protg()->setEnergy(maxE);
                        a->setDefeated(true);
                        mainWindow->update_dead_enemy_g(x_pos,y_pos);
                        checkEnemyAlive();
                    }
                }
            }
        }
    }
    return result;
    //return value is used to check whether is it enemy in given position
}

//*****************Check health pack on given location*****************
bool controller::meetHealthPack(int next_X, int next_Y)
{
    if(worldModel->get_model_healthPacks().size()>0)
    {
        for(auto& h: worldModel->get_model_healthPacks())
        {
            if(h->getXPos() == next_X && h->getYPos() == next_Y)
            {
                auto it = find(worldModel->get_model_healthPacks().begin(), worldModel->get_model_healthPacks().end(), h);
                worldModel->get_model_healthPacks().erase(it);
                float currentHp = worldModel->get_model_protg()->getHealth();
                //max health is limited by maxH defined by user
                if(currentHp+RecoverValue<=maxH)
                    worldModel->get_model_protg()->setHealth(currentHp+RecoverValue);
                else
                    worldModel->get_model_protg()->setHealth(maxH);

                mainWindow->removeHealthPack(next_X,next_Y);
                return true;

                break;
            }
        }
    }
    return false;
}

//*****************Check whether the given location is poisoned,if it is poisoned, health would decrease*****************
void controller::getPoisoned(int x,int y)
{
    for(auto& a:worldModel->getPoisoned_tiles())
    {
        if(a->getXPos()==x&&a->getYPos()==y)
        {
            int currentHp = worldModel->get_model_protg()->getHealth();
            worldModel->get_model_protg()->setHealth(currentHp-a->getValue());
            mainWindow->update_poisoned_protg(x,y);
            mainWindow->updateTextView();
        }
    }

}

shared_ptr<MainWindow> controller::getMainWindow() const
{
    return mainWindow;
}

//*****************Check the possibility to win. If it is impossible to win, end this game in advance*****************
bool controller::possibleToWin()
{
    int required_health_sum{0};
    int health_sum=worldModel->get_model_protg()->getHealth()+worldModel->get_model_healthPacks().size()*RecoverValue;
    for(auto& a:worldModel->get_model_enemies())
    {
        if(!a->getDefeated())
        {
            required_health_sum+=a->getValue();
        }
    }
    if(required_health_sum>health_sum)
    {
        mainWindow->showLoseMsg();
        mainWindow->showTryAgainGif();
        gameover_flag=true;
        return false;
    }
    return true;
}

shared_ptr<pathFinder> controller::getPf() const
{
    return pf;
}

unsigned int controller::getNrOfXEnemy() const
{
    return nrOfXEnemy;
}

bool controller::getGameover_flag() const
{
    return gameover_flag;
}

float controller::getMaxE() const
{
    return maxE;
}

void controller::setMaxE(float value)
{
    maxE = value;
}

float controller::getMaxH() const
{
    return maxH;
}

void controller::setMaxH(float value)
{
    maxH = value;
}


//*****************Add poisoned tiles around position of P enemy (x,y)*****************
void controller::updatePosionedTile(int x, int y)
{
    int row=worldModel->getRows();
    int col=worldModel->getCols();
    if(x-1>=0&&y-1>=0)
    {
        worldModel->addPoisonedTile(x-1,y-1);
    }
    if(y-1>=0)
    {
        worldModel->addPoisonedTile(x,y-1);
    }
    if(x+1<col&&y-1>=0)
    {
        worldModel->addPoisonedTile(x+1,y-1);
    }
    if(x-1>=0)
    {
        worldModel->addPoisonedTile(x-1,y);
    }
    if(x+1<col)
    {
        worldModel->addPoisonedTile(x+1,y);
    }
    if(x-1>=0&&y+1<row)
    {
        worldModel->addPoisonedTile(x-1,y+1);
    }
    if(y+1<row)
    {
        worldModel->addPoisonedTile(x,y+1);
    }
    if(x+1<col&&y+1<row)
    {
        worldModel->addPoisonedTile(x+1,y+1);
    }
}

//*****************Update blocked tiles after Z enemy is attacked*****************
void controller::updateBlockedTile(int x, int y)
{
    int col=worldModel->getCols();
    if(x-3>=0)
    {
        bool result=worldModel->addBlockedTile(x-3,y);
        if(result)
            pf->nodes[(y)*worldModel->getCols()+x-3]->wall=true;
    }
    if(x-2>=0)
    {
        bool result=worldModel->addBlockedTile(x-2,y);
        if(result)
            pf->nodes[(y)*worldModel->getCols()+x-2]->wall=true;
    }
    if(x-1>=0)
    {
        bool result=worldModel->addBlockedTile(x-1,y);
        if(result)
            pf->nodes[(y)*worldModel->getCols()+x-1]->wall=true;
    }
    if(x+1<col)
    {
        bool result=worldModel->addBlockedTile(x+1,y);
        if(result)
            pf->nodes[(y)*worldModel->getCols()+x+1]->wall=true;
    }
    if(x+2<col)
    {
        bool result=worldModel->addBlockedTile(x+2,y);
        if(result)
            pf->nodes[(y)*worldModel->getCols()+x+2]->wall=true;
    }
    if(x+3<col)
    {
        bool result=worldModel->addBlockedTile(x+3,y);
        if(result)
            pf->nodes[(y)*worldModel->getCols()+x+3]->wall=true;
    }

}

//*****************Check whether the given location is blocked*****************
bool controller::isBlocked(int x,int y) const
{
    bool result=false;
    for(auto& a:worldModel->getBlocked_tiles())
    {
        if(a->getXPos()==x&&a->getYPos()==y)
        {
            result=true;
            break;
        }
    }
    return result;
}

//*****************Check whether the given location is black tile*****************
bool controller::isBlack(int x, int y) const
{
    int col=worldModel->getCols();
    int row=worldModel->getRows();
    const float infinity {numeric_limits<float>::infinity()};
    if(x>=0&&x<row&&y>=0&&y<col)
    {
        if(worldModel->get_model_tiles().at(y*col+x)->getValue()==infinity)
            return true;
        else
            return false;
    }
    return false;
}

//*****************Set position of protagonist*****************
void controller::setPosition(int x, int y)
{
    int col=worldModel->getCols();
    int row=worldModel->getRows();
    if(x>=0&&x<col&&y>=0&&y<row)
    {
        worldModel->get_model_protg()->setXPos(x);
        worldModel->get_model_protg()->setYPos(y);
    }
    else
        mainWindow->showOutOfBoundaryMsg();
}

//*****************Save current game progress*****************
void controller::saveGame(){
    QString filename="Data.dat";
    QFile file( filename );
    if(!file.open(QFile::WriteOnly | QFile::Text)){
        qWarning("Couldn't open save file.");
    }
    QJsonObject gameDataInJson;
    addDataToFile(gameDataInJson);
    QJsonDocument saveFile(gameDataInJson);
    QByteArray array = saveFile.toJson().toBase64();
    file.write(array);
    file.flush();
    file.close();
    QMessageBox msgBox;
    msgBox.setText("Save successfully");
    msgBox.exec();
}

//*****************Load game that is saved last time*****************
void controller::loadGame(){
    QString filename="Data.dat";
    QFile file( filename );
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning("Couldn't open save file.");
    }

    QByteArray array = file.readAll();

    QJsonDocument loadData(QJsonDocument::fromJson(array.fromBase64(array)));

    rebuildGame(loadData.object());

    file.close();
    QMessageBox msgBox;
    msgBox.setText("Load successfully");
    msgBox.exec();
}

void controller::addDataToFile(QJsonObject & JsonData) const
{
    JsonData["toggle"] = mainWindow->getToggle();
    JsonData["map"] = getWorldMap();
    QJsonArray protagData;
    QJsonObject protagX;
    QJsonObject protagY;
    QJsonObject protagHealth;
    QJsonObject protagEnergy;
    protagX["xpos"] = worldModel->get_model_protg()->getXPos();
    protagY["ypos"] = worldModel->get_model_protg()->getYPos();
    protagHealth["health"] = worldModel->get_model_protg()->getHealth(); //static_cast<int>(model->getProtagonist()->getHealth());
    protagEnergy["energy"] = worldModel->get_model_protg()->getEnergy();//static_cast<int>(model->getProtagonist()->getEnergy());
    protagData.append(protagX);
    protagData.append(protagY);
    protagData.append(protagHealth);
    protagData.append(protagEnergy);
    JsonData["protagonist"] = protagData;

    QJsonArray enemyArray;
    QJsonArray penemyArray;
    QJsonArray xenemyArray;
    QJsonArray zenemyArray;
    for(auto & e: worldModel->get_model_enemies())
    {
        if(typeid(*e)==typeid(PEnemy))
            {
            QJsonObject enemyObject;
            enemyObject["xpos"] = e->getXPos();
            enemyObject["ypos"] = e->getYPos();
            enemyObject["defeated"] = e->getDefeated();
            enemyObject["strength"] = e->getValue();
            penemyArray.append(enemyObject);
            }
        else if(typeid(*e)==typeid(XEnemy))
            {
            QJsonObject enemyObject;
            enemyObject["xpos"] = e->getXPos();
            enemyObject["ypos"] = e->getYPos();
            enemyObject["defeated"] = e->getDefeated();
            xenemyArray.append(enemyObject);
            }
        else if(typeid(*e)==typeid(ZEnemy))
            {
            QJsonObject enemyObject;
            enemyObject["xpos"] = e->getXPos();
            enemyObject["ypos"] = e->getYPos();
            enemyObject["defeated"] = e->getDefeated();
            enemyObject["strength"] = e->getValue();
            zenemyArray.append(enemyObject);
            }
        else
            {
            QJsonObject enemyObject;
            enemyObject["xpos"] = e->getXPos();
            enemyObject["ypos"] = e->getYPos();
            enemyObject["defeated"] = e->getDefeated();
            enemyObject["strength"] = e->getValue();
            enemyArray.append(enemyObject);
            }
    }
    JsonData["enemies"] = enemyArray;
    JsonData["penemies"] = penemyArray;
    JsonData["xenemies"] = xenemyArray;
    JsonData["zenemies"] = zenemyArray;
    JsonData["enemies_nr"] = (int)nrOfEnermy;
    JsonData["penemies_ratio"] = (float)pRatio;
    JsonData["xenemies_nr"] = (int)nrOfXEnemy;
    JsonData["zenemies_nr"] = (int)nrOfZEnemy;


    JsonData["healthpack_nr"] = (int)worldModel->get_model_healthPacks().size();
    QJsonArray healthpackArray;
    for(auto& hp: worldModel->get_model_healthPacks())
    {
        QJsonObject healthpackObject;
        healthpackObject["xpos"] = hp->getXPos();
        healthpackObject["ypos"] = hp->getYPos();
        healthpackArray.append(healthpackObject);
    }
    JsonData["healthpacks"] = healthpackArray;
}

void controller::rebuildGame(const QJsonObject & loadData)
{
    if (loadData.contains("map") && loadData["map"].isString())
    {
        worldMap = loadData["map"].toString();
    }
    if(loadData.contains("enemies_nr"))
    {
        nrOfEnermy = 0;
    }
    if(loadData.contains("penemies_ratio"))
    {
        pRatio = 0;
    }
    if(loadData.contains("xenemies_nr"))
    {
        nrOfXEnemy = loadData["xenemies_nr"].toInt();
    }
    if(loadData.contains("zenemies_nr"))
    {
        nrOfZEnemy = loadData["zenemies_nr"].toInt();
    }
    if(loadData.contains("healthpack_nr"))
    {
        nrOfHealthPack = loadData["healthpack_nr"].toInt();
    }

    createWorldModel();


    if(loadData.contains("xenemies") && loadData["xenemies"].isArray())
    {
        QJsonArray enemyArray = loadData["xenemies"].toArray();
        for(int index = 0; index < enemyArray.size(); index++)
        {
            QJsonObject enemyObject = enemyArray[index].toObject();
            int xpos = enemyObject["xpos"].toInt();
            int ypos = enemyObject["ypos"].toInt();
            bool defeated = enemyObject["defeated"].toBool();
            auto new_X=make_unique<XEnemy>(xpos,ypos,XEnemyStrength,worldModel);
            new_X->setDefeated(defeated);
            worldModel->get_model_enemies().push_back(move(new_X));

        }
    }
    if(loadData.contains("zenemies") && loadData["zenemies"].isArray())
    {
        QJsonArray enemyArray = loadData["zenemies"].toArray();
        for(int index = 0; index < enemyArray.size(); index++)
        {
            QJsonObject enemyObject = enemyArray[index].toObject();
            int xpos = enemyObject["xpos"].toInt();
            int ypos = enemyObject["ypos"].toInt();
            bool defeated = enemyObject["defeated"].toBool();
            float strength = (float)enemyObject["strength"].toDouble();
            auto new_Z=make_unique<ZEnemy>(xpos,ypos,strength);
            if(defeated)
            {
                updateBlockedTile(xpos,ypos);
            }
            new_Z->setDefeated(defeated);
            worldModel->get_model_enemies().push_back(move(new_Z));

        }
    }
    if(loadData.contains("penemies") && loadData["penemies"].isArray())
    {
        QJsonArray enemyArray = loadData["penemies"].toArray();
        for(int index = 0; index < enemyArray.size(); index++)
        {
            QJsonObject enemyObject = enemyArray[index].toObject();
            int xpos = enemyObject["xpos"].toInt();
            int ypos = enemyObject["ypos"].toInt();
            bool defeated = enemyObject["defeated"].toBool();
            float strength = (float)enemyObject["strength"].toDouble();
            auto new_P=make_unique<PEnemy>(xpos,ypos,strength);
            if(defeated)
            {
                updatePosionedTile(xpos,ypos);
            }
            new_P->setDefeated(defeated);
            worldModel->get_model_enemies().push_back(move(new_P));
        }
    }
    if(loadData.contains("enemies") && loadData["enemies"].isArray())
    {
        QJsonArray enemyArray = loadData["enemies"].toArray();
        for(int index = 0; index < enemyArray.size(); index++)
        {
            QJsonObject enemyObject = enemyArray[index].toObject();
            int xpos = enemyObject["xpos"].toInt();
            int ypos = enemyObject["ypos"].toInt();
            bool defeated = enemyObject["defeated"].toBool();
            float strength = (float)enemyObject["strength"].toDouble();
            auto new_E=make_unique<Enemy>(xpos,ypos,strength);
            new_E->setDefeated(defeated);
            worldModel->get_model_enemies().push_back(move(new_E));
        }
    }


    if(loadData.contains("protagonist") && loadData["protagonist"].isArray())
    {
        QJsonArray protagonist = loadData["protagonist"].toArray();
        QJsonObject protagX = protagonist[0].toObject();
        QJsonObject protagY = protagonist[1].toObject();
        QJsonObject protagH = protagonist[2].toObject();
        QJsonObject protagE = protagonist[3].toObject();
        worldModel->get_model_protg()->setPos(protagX["xpos"].toInt(),protagY["ypos"].toInt());
        worldModel->get_model_protg()->setHealth(protagH["health"].toDouble());
        worldModel->get_model_protg()->setEnergy(protagE["energy"].toDouble());
    }

    if(loadData.contains("healthpacks") && loadData["healthpacks"].isArray())
    {
        QJsonArray healthpackArray = loadData["healthpacks"].toArray();
        for(int index = 0; index < healthpackArray.size(); index++)
        {
            QJsonObject healthpack = healthpackArray[index].toObject();
            int xpos = healthpack["xpos"].toInt();
            int ypos = healthpack["ypos"].toInt();
            worldModel->get_model_healthPacks().at(index)->setXPos(xpos);
            worldModel->get_model_healthPacks().at(index)->setYPos(ypos);
        }
    }

    mainWindow->setToggle(!loadData["toggle"].toBool());
    mainWindow->init(worldModel,this);
    mainWindow->on_pushButton_clicked();

    for(auto & e : worldModel->get_model_enemies())
    {
        if(e->getDefeated())
        {
            mainWindow->update_dead_enemy_g(e->getXPos(),e->getYPos());
        }
        if(typeid(*e)==typeid(PEnemy))
        {
            mainWindow->update_poisoned_tiles();

        }
        if(typeid(*e)==typeid(ZEnemy))
        {
            mainWindow->update_blocked_tiles();
        }
    }
    pf=make_shared<pathFinder>(worldModel);
    pf->initPathFinder();
    signal_slot_init();
}



