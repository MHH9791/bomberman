#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <QMainWindow>
#include "world.h"
#include "mainwindow.h"
#include "gamemodel.h"
#include <memory>
#include <iostream>
#include "world_global.h"
#include "pathfinder.h"
#include "xenemy.h"
#include "zenemy.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <string>
#include <math.h>

class MainWindow;
using namespace std;
class controller : public QObject
{
    Q_OBJECT

public:

    controller();
    controller(unsigned int nrOfEnermy, float pRatio, unsigned int nrOfHealthPack, unsigned int nrOfXEnemy, unsigned int nrOfZEnemy);
    ~controller();

    bool attackEnemy(int x_pos, int y_pos);
    void addDataToFile(QJsonObject & JsonData) const;
    void checkMap(QString);
    bool checkIsAlive();
    bool checkEnemyAlive();
    void consumeEnergy(int,int);
    void createWorldModel();
    shared_ptr<pathFinder> getPf() const;
    unsigned int getNrOfXEnemy() const;
    void getPoisoned(int x,int y);
    bool getGameover_flag() const;
    float getMaxH() const;
    float getMaxE() const;
    shared_ptr<MainWindow> getMainWindow() const;
    shared_ptr<gameModel> getWorld() const;
    QString getWorldMap() const;
    void insertNewXEnemy();
    void insertNewZEnemy();
    bool isBlocked(int x,int y) const;
    bool isBlack(int x,int y) const;
    void initSetting();
    void loadGame();
    bool meetHealthPack(int next_X, int next_Y);
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
    bool possibleToWin();
    void rebuildGame(const QJsonObject & loadData);
    void setPosition(int x,int y);
    void saveGame();
    void startGame();
    void signal_slot_init();
    void setMaxE(float value);
    void setMaxH(float value);
    void updatePosionedTile(int x,int y);
    void updateBlockedTile(int x,int y);

private:
    shared_ptr<pathFinder> pf;
    shared_ptr<gameModel> worldModel;
    shared_ptr<MainWindow> mainWindow;
    unsigned int nrOfEnermy;
    QString worldMap;
    float pRatio;
    unsigned int nrOfHealthPack;
    unsigned int nrOfXEnemy;
    unsigned int nrOfZEnemy;
    bool gameover_flag=false;
    float maxE=300.0f;
    float maxH=300.0f;


};

#endif // CONTROLLER_H
