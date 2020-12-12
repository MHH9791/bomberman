#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "world.h"
#include "gamemodel.h"
#include "graphicComponent.h"
#include "textComponent.h"
#include "pathfinder.h"
#include "controller.h"
#include <QObject>
#include "xenemy.h"
#include <string>
#include <QTimer>
#include <QMovie>
class controller;
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void attackSecondNearestEnemy();
    void attackThirdNearestEnemy();
    void clearMsg();
    void deletePathOnGraph();
    void executeCommand(string);
    int getXEnemyNr();
    int getZEnemyNr();
    int getHealthPackNr();
    int getEnemyNr();
    float getPratio();
    vector<shared_ptr<textComponent>> getTextVector();
    float getMaxHealth();
    float getMaxEnegy();
    bool getToggle() const;
    QString getMap();
    bool getRestartFlag() const;
    void init(std::shared_ptr<gameModel> gameModel,controller* con);
    void initGraphicView();
    void initText();
    void keyPressEvent(QKeyEvent *e);
    void msleep(int);
    void printAllCommand();
    void removeHealthPack(int x, int y);
    void showWinMsg();
    void showPathOnGraph(shared_ptr<pathFinder::aNode>);
    void showInsufficientHealthMsg(float);
    void showInsufficientEnergyMsg();
    void showOutOfBoundaryMsg();
    void showWelcomeMsg();
    void showLoseMsg();
    void showBlockedMsg();
    void showNormalOverMsg();
    void setEndAutoplay(bool value);
    void showCommandErrorMsg();
    void showCommandFullMsg();
    void showWholeTextView();
    void showNoHealthPack();
    void showNoEnemy();
    void showGraphicView();
    void showTextView();
    void showCongratulationGif();
    void showTryAgainGif();
    void setToggle(bool value);
    void showBlockedOverMsg();
    void showNoAvailableWayMsg();
    void setRestartFlag(bool value);
    void updateGraphicView();
    void updateTextView();
    void update_dead_enemy_g(int, int);
    void update_poisoned_tiles();
    void update_blocked_tiles();
    void update_XEnemy(XEnemy*);
    void updateOldlocation(int x,int y);
    void update_attacking_protg(int x,int y);
    void update_poisoned_protg(int x,int y);
    void update_health_protg(int x,int y);
    void wheelEvent(QWheelEvent *event);

public slots:

    void on_pushButton_clicked();

private slots:
    void findPath_step(int x,int y);

    void update_protg_position(int new_x,int new_y);

    void update_dead_enemy();

    void on_upButton_clicked();

    void on_leftButton_clicked();

    void on_downButton_clicked();

    void on_rightButton_clicked();

    void update_protg_health(int);

    void update_protg_energy(int);

    void on_textCommand_returnPressed();

    void on_findpath_button_clicked();

    void on_restartButton_clicked();

    void on_setPosition_clicked();

    void on_findNeareastEnemy_clicked();

    void on_findNearestHealthPack_clicked();

    void on_autoplay_1_clicked();

    void on_autoplay_2_clicked();

    void on_autoplay_3_clicked();

    void on_save_clicked();

    void on_load_clicked();

    void on_change_Heuristic_clicked();



    void on_endAutoPlay_clicked();

private:
    Ui::MainWindow *ui;
    vector<shared_ptr<textComponent>> textVector;
    QGraphicsScene * scene;
    shared_ptr<gameModel> model;
    controller* ctl;
    vector<string> commandList={"up","right","down","left","attack nearest enemy", "take nearest health pack", "help","goto X Y"};
    double scaleFactor = 0;
    int current_x=0;
    int current_y=0; //initial position is (0,0)
    bool toggle= false;
    bool endAutoplay=false;
    bool restartFlag=false;


};
#endif // MAINWINDOW_H
