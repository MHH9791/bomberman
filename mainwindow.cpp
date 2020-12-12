#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsScene>
#include "gamemodel.h"
#include <QKeyEvent>

double static WORLDMODELSIZE = 600;
const int text_width {30};
const int text_height {20};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow:: init(std::shared_ptr<gameModel> Model,controller* con)
{

    model=Model;
    ctl=con;
    current_x=0;
    current_y=0;
    initGraphicView();
    initText();
    updateTextView();
    int health=static_cast<int>(model->get_model_protg()->getHealth());
    int energy=static_cast<int>(model->get_model_protg()->getEnergy());
    QString h=QString::number(health);
    QString e=QString::number(energy);
    ui->current_health->setText(h);
    ui->current_energy->setText(e);
    ui->healthBar->setValue(static_cast<int>(health*100/ctl->getMaxH()));
    ui->energyBar->setValue(static_cast<int>(energy*100/ctl->getMaxE()));
    ui->gif->clear();

}

void MainWindow:: initGraphicView()
{
    int row = model->getRows();
    int col = model->getCols();
    scaleFactor = WORLDMODELSIZE/(double)col;

    scene = new QGraphicsScene(0, 0, col*scaleFactor, row*scaleFactor, this);
    scene->addPixmap(QPixmap(ctl->getWorldMap()))->setScale(scaleFactor);

    auto & protgFromModel = model->get_model_protg();
    g_protg * protag = new g_protg(scaleFactor);
    protag->setPos(protgFromModel->getXPos()*scaleFactor,protgFromModel->getYPos()*scaleFactor);
    protag->setZValue(1.3);
    scene->addItem(protag);

    for (auto & it : model->get_model_healthPacks()) {
        g_healthPack * healthpack = new g_healthPack(scaleFactor);
        scene->addItem(healthpack);
        healthpack->setPos(it->getXPos()*scaleFactor, it->getYPos()*scaleFactor);
        healthpack->setZValue(1.2);
    }

//    for (auto & it : model->getPoisoned_tiles()) {
//        g_pTiles * pTiles = new g_pTiles();
//        scene->addItem(pTiles);
//        pTiles->setPos(it->getXPos()*scaleFactor, it->getYPos()*scaleFactor);
//        pTiles->setZValue(1.1);
//    }

//    for (auto & it : model->getBlocked_tiles()) {
//        g_bTiles * bTiles = new g_bTiles();
//        scene->addItem(bTiles);
//        bTiles->setPos(it->getXPos()*scaleFactor, it->getYPos()*scaleFactor);
//        bTiles->setZValue(1.1);
//    }

    for(auto & e: model->get_model_enemies())
    {
        if(typeid(*e)==typeid(PEnemy))
            {
                g_pEnemy * pEnemy = new g_pEnemy(scaleFactor);
                scene->addItem(pEnemy);
                pEnemy->setPos(e->getXPos()*scaleFactor, e->getYPos()*scaleFactor);
                pEnemy->setZValue(1.2);
            }
        else if(typeid(*e)==typeid(XEnemy))
            {
            g_xEnemy * xEnemy = new g_xEnemy(scaleFactor);
            scene->addItem(xEnemy);
            xEnemy->setPos(e->getXPos()*scaleFactor, e->getYPos()*scaleFactor);
            xEnemy->setZValue(1.2);
            }
        else if(typeid(*e)==typeid(ZEnemy))
            {
            g_zEnemy * zEnemy = new g_zEnemy(scaleFactor);
            scene->addItem(zEnemy);
            zEnemy->setPos(e->getXPos()*scaleFactor, e->getYPos()*scaleFactor);
            zEnemy->setZValue(1.2);
            }
        else
            {
            g_enemy * eEnemy = new g_enemy(scaleFactor);
            scene->addItem(eEnemy);
            eEnemy->setPos(e->getXPos()*scaleFactor, e->getYPos()*scaleFactor);
            eEnemy->setZValue(1.2);
            }
            //code for Xenemy could be added later
    }
    ui->graphicsView->setScene(scene);
//    ui->graphicsView->hide();
}

void MainWindow:: showPathOnGraph(shared_ptr<pathFinder::aNode> s)
{
    g_solutionNode * solutionNode = new g_solutionNode(scaleFactor);
    scene->addItem(solutionNode);
    solutionNode->setPos(s->x*scaleFactor, s->y*scaleFactor);
    solutionNode->setZValue(1.1);
}

void MainWindow:: deletePathOnGraph()
{
//    cout<< "enter delete" <<endl;
    for(auto & s : scene->items())
        {
            auto dummy = s;
            if(typeid(g_solutionNode).name()==typeid (*dummy).name())
            {
                scene->removeItem(s);
                delete s;
            }
        }

    updateGraphicView();
}

void MainWindow:: updateGraphicView()
{
    ui->graphicsView->setScene(scene);
}




//This method is used to initialize the text vector based on information stored in gameModel
void MainWindow:: initText()
{
    textVector.clear();
    int row=model->getRows();
    int col=model->getCols();
    vector<unique_ptr<Enemy>>& enemyVector=model->get_model_enemies();
    unique_ptr<Protagonist>& protg=model->get_model_protg();
    vector<unique_ptr<Tile>>& healthPack=model->get_model_healthPacks();
    //Build up the main structure using tile and boundary first
    //(2*row + 1) rows in total
    for(int row_index=0;row_index<=2*row;row_index++)
    {
        for(int col_index=0;col_index<col;col_index++)
        {
            if(row_index%2==0)
            {
                auto boundary=make_shared<t_boundary>();
                textVector.push_back(boundary);
            }
            else
            {
                auto tile=make_shared<t_tile>();
                textVector.push_back(tile);
            }
        }
    }
    //replace tile by protagonist in text vector
    int position=(2*protg->getYPos()+1)*col+protg->getXPos();
    auto prot=make_shared<t_protagonist>();
    textVector[position]=prot;



    //replace tiles by black tiles in text vector
    const float infinity {numeric_limits<float>::infinity()};
    for(auto& a: model->get_model_tiles())
    {
        if(a->getValue()==infinity)
        {
            auto t_bt=make_shared<t_black_tile>();
            textVector[(2*a->getYPos()+1)*col+a->getXPos()]=t_bt;
        }
    }

    //replace tiles by health packs in text vector
    for(auto& hp : healthPack)
    {
        int pos=(2*hp->getYPos()+1)*col+hp->getXPos();
        auto t_hp=make_shared<t_healthPack>();
        textVector[pos]=t_hp;
    }

    //replace tiles by enemy in text vector
    for(auto& em : enemyVector)
    {
        int pos=(2*em->getYPos()+1)*col+em->getXPos();
        if(typeid (*em)==typeid(PEnemy))
        {
            auto t_pe=make_shared<t_Penemy>();
            textVector[pos]=t_pe;
        }
        else if(typeid (*em)==typeid(XEnemy))
        {
            auto t_xe=make_shared<t_Xenemy>();
            textVector[pos]=t_xe;
        }
        else if(typeid (*em)==typeid(ZEnemy))
        {
            auto t_ze=make_shared<t_Zenemy>();
            textVector[pos]=t_ze;
        }
        else
        {
            auto t_e=make_shared<t_enemy>();
            textVector[pos]=t_e;
        }
    }

ui->textBrowser->hide();

}
void MainWindow:: updateTextView()
{
    if(toggle)
    {
        int row=model->getRows();
        int col=model->getCols();
        int row_index,col_index;
        int x_pos=model->get_model_protg()->getXPos();
        int y_pos=model->get_model_protg()->getYPos();
        col_index=x_pos/text_width;
        row_index=y_pos/text_height;
        int row_end=((row_index+1)*2*text_height<2*row) ? (row_index+1)*2*text_height:2*row;
        int col_end=((col_index+1)*text_width<col) ? (col_index+1)*text_width:col;
        QString final_text="";
        for(int r=row_index*2*text_height;r<=row_end;r++)
        {
            for(int c=col_index*text_width;c<=col_end;c++)
            {
                  if(c==col_end)
                  {
                      if(r%2==1)
                      {
                          QString q=QString("|\n");
                          final_text.append(q);}
                      else
                      {
                          QString q=QString("—\n");
                          final_text.append(q);
                      }
                  }
                  else
                  {
                      final_text.append(*textVector[r*col+c]);
                  }

            }
        }
        ui->textBrowser->setText(final_text);

    }
}

//void MainWindow:: updateTextView()
//{
//    if(toggle)
//    {
//        int row=model->getRows();
//        int col=model->getCols();
//        QString final_text="";
//        int index=0;

//        for(int row_index=0;row_index<=2*row;row_index++)
//        {
//            for(int col_index=0;col_index<=col;col_index++)
//            {
//                if(col_index==col)
//                {
//                    if(row_index%2==1)
//                        final_text.append("|\n");
//                    else
//                        final_text.append("—\n");
//                }
//                else
//                {
//                    final_text.append(*textVector[index++]);
//                }
//            }
//        }

//        ui->textBrowser->setText(final_text);
//    }

//}

void MainWindow:: showWholeTextView()
{
    if(toggle)
    {
        int row=model->getRows();
        int col=model->getCols();
        QString final_text="";
        int index=0;

        for(int row_index=0;row_index<=2*row;row_index++)
        {
            for(int col_index=0;col_index<=col;col_index++)
            {
                if(col_index==col)
                {
                    if(row_index%2==1)
                        final_text.append("|\n");
                    else
                        final_text.append("—\n");
                }
                else
                {
                    final_text.append(*textVector[index++]);
                }
            }
        }

        ui->textBrowser->setText(final_text);
    }

}

vector<shared_ptr<textComponent>> MainWindow:: getTextVector()
{
    return textVector;
}


void MainWindow::on_upButton_clicked()
{
    ctl->moveUp();

}

void MainWindow::on_leftButton_clicked()
{
    ctl->moveLeft();

}

void MainWindow::on_downButton_clicked()
{
    ctl->moveDown();

}

void MainWindow::on_rightButton_clicked()
{
//    int isMoving=ctl->moveRight();
//    if(isMoving)
//    {
//        cout<<"aaaaa"<<endl;
//        int col=model->getCols();
//        auto new_tile=make_shared<t_tile>();
//        auto new_protg=make_shared<t_protagonist>();
//        textVector[(2*current_y+1)*col+current_x+1]=new_protg;
//        textVector[(2*current_y+1)*col+current_x]=new_tile;
//        current_x=model->getProtg()->getXPos();
//        current_y=model->getProtg()->getYPos();
//        updateTextView();
//    }
    ctl->moveRight();


}

void MainWindow::updateOldlocation(int x, int y)
{
    current_x=x;
    current_y=y;
}

void MainWindow::update_protg_position(int new_x,int new_y)
{
    int col=model->getCols();

    auto new_protg=make_shared<t_protagonist>();
    textVector[(2*new_y+1)*col+new_x]=new_protg;
    bool poison_flag=false;
    bool blocked_flag=false;
    for(auto& a:model->getPoisoned_tiles())
    {
        if(a->getXPos()==current_x&&a->getYPos()==current_y)
        {
            poison_flag=true;
            break;
        }
    }
    for(auto& a:model->getBlocked_tiles())
    {
        if(a->getXPos()==current_x&&a->getYPos()==current_y)
        {
            blocked_flag=true;
            break;
        }
    }

    if(blocked_flag)
    {
        auto new_blocked_tile=make_shared<t_blocked_tile>();
        textVector[(2*current_y+1)*col+current_x]=new_blocked_tile;
    }
    else if(poison_flag)
    {
        auto new_poisoned_tile=make_shared<t_poisoned_tile>();
        textVector[(2*current_y+1)*col+current_x]=new_poisoned_tile;
    }
    else
    {
        auto new_tile=make_shared<t_tile>();
        textVector[(2*current_y+1)*col+current_x]=new_tile;

    }
    updateTextView();

    for(auto & p : scene->items())
        {
            auto dummy = p;
            if(typeid(g_protg).name()==typeid (*dummy).name())
            {
                dynamic_cast<g_protg*>(dummy)->setPos(new_x*scaleFactor,new_y*scaleFactor);
                break;
            }
        }

    updateGraphicView();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_W){
        on_upButton_clicked();
    }
    else if(event->key()==Qt::Key_D)
    {
        on_rightButton_clicked();
    }
    else if(event->key()==Qt::Key_A)
    {
        on_leftButton_clicked();
    }
    else if(event->key()==Qt::Key_S)
    {
        on_downButton_clicked();
    }
}

void MainWindow::removeHealthPack(int x, int y)
{
    for(auto & h : scene->items())
    {
        auto dummy = h;
        if(h->x()==(x*scaleFactor) && h->y()==(y*scaleFactor) && typeid(g_healthPack).name()==typeid (*dummy).name())
        {
            scene->removeItem(h);
            delete h;
            break;
        }
    }
    updateGraphicView();
}


void MainWindow::update_dead_enemy()
{
    int col=model->getCols();
    for(auto& e:model->get_model_enemies())
    {
        if(e->getDefeated())
        {
//            e->setValue(numeric_limits<float>::infinity());
//            ctl->getPf()->nodes[e->getYPos()*model->getCols()+e->getXPos()]->value=numeric_limits<float>::infinity();
//            ctl->getPf()->nodes[e->getYPos()*model->getCols()+e->getXPos()]->wall=true;
            if(typeid(*e)==typeid(Enemy))
            {
                auto new_dead_enemy=make_shared<t_defeated_enemy>();
                textVector[(2*e->getYPos()+1)*col+e->getXPos()]=new_dead_enemy;
            }
            else if(typeid(*e)==typeid(PEnemy))
            {
                auto new_dead_penemy=make_shared<t_defeated_penemy>();
                textVector[(2*e->getYPos()+1)*col+e->getXPos()]=new_dead_penemy;
            }
            else if(typeid(*e)==typeid(XEnemy))
            {
                auto new_dead_xenemy=make_shared<t_defeated_xenemy>();
                textVector[(2*e->getYPos()+1)*col+e->getXPos()]=new_dead_xenemy;
            }
            else if(typeid(*e)==typeid(ZEnemy))
            {
                auto new_dead_zenemy=make_shared<t_defeated_zenemy>();
                textVector[(2*e->getYPos()+1)*col+e->getXPos()]=new_dead_zenemy;
            }
        }
    }
    updateTextView();
}

void MainWindow::update_XEnemy(XEnemy* xe)
{
    int col=model->getCols();
    auto new_tile=make_shared<t_tile>();
    auto new_XEnemy=make_shared<t_Xenemy>();
    textVector[(2*xe->getYPos()+1)*col+xe->getXPos()]=new_XEnemy;
    textVector[(2*xe->getPrevious_Y()+1)*col+xe->getPrevious_X()]=new_tile;
    updateTextView();

    for(auto & x : scene->items())
        {
//        if(x->x()==(xe->getXPos()*scaleFactor) && x->y()==(xe->getYPos()*scaleFactor))
//        {
//            scene->removeItem(x);
//            delete x;
//            break;
//        }
            auto dummy = x;
            if(typeid(g_xEnemy).name()==typeid (*dummy).name())
            {
                dynamic_cast<g_xEnemy*>(dummy)->setPos(xe->getXPos()*scaleFactor,xe->getYPos()*scaleFactor);
            }
        }
    updateGraphicView();

}

void MainWindow::update_attacking_protg(int x, int y)
{

    auto new_attacking_protg=make_shared<t_attacking_protagonist>();
    textVector[(2*y+1)*model->getCols()+x]=new_attacking_protg;
    for(auto & p : scene->items())
        {
            if(p->x() == (x*scaleFactor) && p->y() == (y*scaleFactor))
            {
                scene->removeItem(p);
                delete p;
                g_protg_under_attack * ua = new g_protg_under_attack(scaleFactor);
                ua->setPos(x*scaleFactor,y*scaleFactor);
                ua->setZValue(1.3);
                scene->addItem(ua);
                msleep(100);
                scene->removeItem(ua);
                delete ua;
                g_protg * protag = new g_protg(scaleFactor);
                protag->setPos(x*scaleFactor,y*scaleFactor);
                protag->setZValue(1.3);
                scene->addItem(protag);
                break;
            }

        }
}

void MainWindow::update_poisoned_protg(int x, int y)
{
        auto new_poisoned_protg=make_shared<t_poisoned_protagonist>();
        textVector[(2*y+1)*model->getCols()+x]=new_poisoned_protg;
        for(auto & p : scene->items())
            {
                if(p->x() == (x*scaleFactor) && p->y() == (y*scaleFactor))
                {
                    scene->removeItem(p);
                    delete p;
                    g_protg_under_posion * up = new g_protg_under_posion(scaleFactor);
                    up->setPos(x*scaleFactor,y*scaleFactor);
                    up->setZValue(1.3);
                    scene->addItem(up);
                    msleep(100);
                    scene->removeItem(up);
                    delete up;
                    g_protg * protag = new g_protg(scaleFactor);
                    protag->setPos(x*scaleFactor,y*scaleFactor);
                    protag->setZValue(1.3);
                    scene->addItem(protag);
                    break;
                }
        }
}

void MainWindow::update_health_protg(int x, int y)
{

    auto new_health_protg=make_shared<t_health_protagonist>();
    textVector[(2*y+1)*model->getCols()+x]=new_health_protg;
    for(auto & p : scene->items())
        {
            if(p->x() == (x*scaleFactor) && p->y() == (y*scaleFactor))
            {
                scene->removeItem(p);
                delete p;
                g_protg_under_health * uh = new g_protg_under_health(scaleFactor);
                uh->setPos(x*scaleFactor,y*scaleFactor);
                uh->setZValue(1.3);
                scene->addItem(uh);
                msleep(100);
                scene->removeItem(uh);
                delete uh;
                g_protg * protag = new g_protg(scaleFactor);
                protag->setPos(x*scaleFactor,y*scaleFactor);
                protag->setZValue(1.3);
                scene->addItem(protag);
                break;
            }
    }
}

//*****************Update poisoned tiles in UI*****************
void MainWindow::update_poisoned_tiles()
{
    int col=model->getCols();
    for(auto& a:model->getPoisoned_tiles())
    {
        g_pTiles * pTiles = new g_pTiles(scaleFactor);
        scene->addItem(pTiles);
        pTiles->setPos(a->getXPos()*scaleFactor, a->getYPos()*scaleFactor);
        pTiles->setZValue(1.1);

        auto new_poisoned_tile=make_shared<t_poisoned_tile>();
        textVector[(2*a->getYPos()+1)*col+a->getXPos()]=new_poisoned_tile;
    }
    update_attacking_protg(model->get_model_protg()->getXPos(),model->get_model_protg()->getYPos());
    updateTextView();
    updateGraphicView();
}

//*****************Update blocked tiles in UI*****************
void MainWindow::update_blocked_tiles()
{
    int col=model->getCols();
    for(auto& a:model->getBlocked_tiles())
    {
        g_bTiles * bTiles = new g_bTiles(scaleFactor);
        scene->addItem(bTiles);
        bTiles->setPos(a->getXPos()*scaleFactor, a->getYPos()*scaleFactor);
        bTiles->setZValue(1.1);
        auto new_blocked_tile=make_shared<t_blocked_tile>();
        textVector[(2*a->getYPos()+1)*col+a->getXPos()]=new_blocked_tile;
    }
    update_attacking_protg(model->get_model_protg()->getXPos(),model->get_model_protg()->getYPos());
    updateTextView();
    updateGraphicView();
}

//*****************Update dead enemy in graphic view*****************
void MainWindow::update_dead_enemy_g(int xpos,int ypos)
{
    for(auto & e : scene->items())
    {
        if(e->x()==(xpos*scaleFactor) && e->y()==(ypos*scaleFactor))
        {
            scene->removeItem(e);
            delete e;
            g_deadEnemy * newDead = new g_deadEnemy(scaleFactor);
            newDead->setPos(xpos*scaleFactor,ypos*scaleFactor);
            newDead->setZValue(1.2);
            scene->addItem(newDead);
            break;
        }
    }
    updateGraphicView();
}


void MainWindow::on_pushButton_clicked()
{
    toggle = !toggle;
    if(toggle)
    {
        showTextView();
    }

    else{
        showGraphicView();
    }

}

void MainWindow::showGraphicView()
{
    ui->graphicsView->show();
    ui->textBrowser->hide();
}

void MainWindow::showTextView()
{
    ui->graphicsView->hide();
    ui->textBrowser->show();
    updateTextView();
}

//*****************Update health bar once health is changed*****************
void MainWindow::update_protg_health(int health)
{
    QString h=QString::number(health);
    ui->current_health->setText(h);
    ui->healthBar->setValue(static_cast<int>(health*100/ctl->getMaxH()));
}

//*****************Update energy bar once energy is changed*****************
void MainWindow::update_protg_energy(int energy)
{
    QString e=QString::number(energy);
    ui->current_energy->setText(e);
    ui->energyBar->setValue(static_cast<int>(energy*100/ctl->getMaxE()));
}

void MainWindow::showWinMsg()
{
    ui->game_message->setText("Congratulation! You win!");
}

void MainWindow::showInsufficientHealthMsg(float required_health)
{
    int r=static_cast<int>(required_health);
    QString s=QString("Health is insufficient! You need ");
    QString a=QString::number(r);
    s.append(a);
    s.append(" health to defeat this enermy");
    ui->game_message->setText(s);
}

void MainWindow::showOutOfBoundaryMsg()
{
    ui->game_message->setText("Out of boundary!");
}

void MainWindow::showBlockedMsg()
{
    ui->game_message->setText("This tile is blocked!");
}

void MainWindow::clearMsg()
{
    ui->game_message->clear();
}

void MainWindow::showWelcomeMsg()
{
    ui->game_message->setText("Welcome! Enjoy the game");
}

void MainWindow::showNoAvailableWayMsg()
{
    ui->game_message->setText("No available path to destination");
}

void MainWindow::showBlockedOverMsg()
{
    ui->game_message->setText("Impossible to win. You are blocked");
}

void MainWindow::showNormalOverMsg()
{
    ui->game_message->setText("Game Over!");
}

void MainWindow::showLoseMsg()
{
    ui->game_message->setText("Game Over! Impossible to win, try again! ");
}

void MainWindow::showInsufficientEnergyMsg()
{
    ui->game_message->setText("Game Over! Energy is consumed up");
}

void MainWindow::showCommandFullMsg()
{
    ui->game_message->setText("Please enter the full command");
}

void MainWindow::showCommandErrorMsg()
{
    ui->game_message->setText("No such a command, you can print help to get all available commands");
}

void MainWindow::showNoEnemy()
{
    ui->game_message->setText("No enemy");
}

void MainWindow::showNoHealthPack()
{
    ui->game_message->setText("No health pack");
}

void MainWindow::printAllCommand()
{
    QString s="";
    for(auto& a:commandList)
    {
        s.append(QString::fromStdString(a));
        s.append("\n");
    }
    ui->game_message->setText(s);
}

//*****************Identify command based on given text*****************
void MainWindow::on_textCommand_returnPressed()
{
    ui->game_message->clear();
    string command=ui->textCommand->text().toStdString();
    cout<<command<<endl;
    int count {0};
    string s;
    int x {0};
    int y {0};
    bool flag {true};
    //goto X Y is considered separatly
    if(command.compare(0,5,"goto ")==0)
    {
        unsigned index=5;
        while(command[index]!=' '&&index<command.length())
        {
            if(isdigit(command[index]))
            {
                x=x*10+command[index]-'0';
                index++;
            }
            else
            {
                flag=false;
                break;
            }
        }
        index++;
        while(command[index]!=' '&&index<command.length())
        {
            if(isdigit(command[index]))
            {
                y=y*10+command[index]-'0';
                index++;
            }
            else
            {
                flag=false;
                break;
            }
        }
        if(index!=command.length())
            flag=false;
        if(flag)
        {
            ui->target_X->setText(QString::number(x));
            ui->target_Y->setText(QString::number(y));
            on_findpath_button_clicked();
            ui->target_X->clear();
            ui->target_Y->clear();
        }
        else
        {
            showCommandErrorMsg();
        }
    }
    //check the consistency of first few characters with the command
    else
    {
        for(unsigned int i=0;i<commandList.size()-1;i++)
        {
            if(commandList[i].compare(0,command.length(),command)==0)
            {
                s=commandList[i];
                count++;
            }
        }
        //count=1 means that given characters could determine only one command, otherwise several commands could be possible
        //and then we require the user to input full command
        if(count==1)
        {
            executeCommand(s);
        }
        else if(count>1)
            showCommandFullMsg();
        else
            showCommandErrorMsg();
    }
    ui->textCommand->clear();

}

//*****************Execute corresponding operation based on given command*****************
void MainWindow::executeCommand(string s)
{

    if(commandList[0].compare(s)==0)
        on_upButton_clicked();
    else if(commandList[1].compare(s)==0)
        on_rightButton_clicked();
    else if(commandList[2].compare(s)==0)
        on_downButton_clicked();
    else if(commandList[3].compare(s)==0)
        on_leftButton_clicked();
    else if(commandList[4].compare(s)==0)
        on_findNeareastEnemy_clicked();
    else if(commandList[5].compare(s)==0)
        on_findNearestHealthPack_clicked();
    else if(commandList[6].compare(s)==0)
        printAllCommand();

}

//*****************Move up, down, left or right by compared current location and destination*****************
void MainWindow::findPath_step(int x, int y)
{
    int current_X=model->get_model_protg()->getXPos();
    int current_Y=model->get_model_protg()->getYPos();
    if(x==current_X&&y==current_Y+1)
        on_downButton_clicked();
    else if(x==current_X&&y==current_Y-1)
        on_upButton_clicked();
    else if(x==current_X+1&&y==current_Y)
        on_rightButton_clicked();
    else if(x==current_X-1&&y==current_Y)
        on_leftButton_clicked();
}

void MainWindow::on_findpath_button_clicked()
{
    restartFlag=false;
    ctl->getPf()->setPath_found(false);
    int target_X=ui->target_X->text().toInt();
    int target_Y=ui->target_Y->text().toInt();
    int speed=ui->animationSpeed->text().toInt();
    if(speed<=100)
        speed=100;
    else if(speed>=1000)
        speed=1000;
    int current_X=model->get_model_protg()->getXPos();
    int current_Y=model->get_model_protg()->getYPos();

    if(target_X<model->getCols()&&target_Y<model->getRows())
    {
        ctl->getPf()->findPath(current_X,current_Y,target_X,target_Y);
        if(ctl->getPf()->getPath_found())
        {
            for(auto& s: ctl->getPf()->getSolution())
            {
                showPathOnGraph(s);
            }
            for(auto& a: ctl->getPf()->getSolution())
            {
                if(!ctl->getGameover_flag())
                {
                    findPath_step(a->x, a->y);
                    msleep(speed);
                }
                else{
                    break;
                }
                //break this loop if restart button is clicked
                if(restartFlag)
                {
                    restartFlag=false;
                    break;
                }
            }
            deletePathOnGraph();
        }
        else
        {
            showNoAvailableWayMsg();
        }
    }
    else
        showOutOfBoundaryMsg();

}

void MainWindow::on_restartButton_clicked()
{
    ctl->startGame();
    setRestartFlag(true);
}

//*****************Set toggle in order to switch view*****************
void MainWindow::setToggle(bool value)
{
    toggle = value;
}

//*****************Get the map chosen by user*****************
QString MainWindow::getMap()
{
    return ui->choosemap->currentText();
}

//*****************Sleep for a period of given time*****************
void MainWindow::msleep(int msec)
{
    QEventLoop loop;

    QTimer::singleShot(msec, &loop, &QEventLoop::quit);

    loop.exec();
}

//*****************Show congratulation gif when user wins this game*****************
void MainWindow::showCongratulationGif()
{
    ui->textBrowser->hide();
    ui->graphicsView->hide();
    QMovie *movie=new QMovie(":/image/congratulation.gif");
    movie->start();
    ui->gif->setMovie(movie);
    ui->gif->setScaledContents(true);

}

//*****************Show gif when user lose this game*****************
void MainWindow::showTryAgainGif()
{
    ui->textBrowser->hide();
    ui->graphicsView->hide();
    QMovie *movie=new QMovie(":/image/tryagain.gif");
    movie->start();
    ui->gif->setMovie(movie);
    ui->gif->setScaledContents(true);

}

int MainWindow::getEnemyNr()
{
    if(!ui->enemy->text().isEmpty()&&ui->enemy->text().toInt()>=0)
        return ui->enemy->text().toInt();
    else
        return -1;
}

float MainWindow::getPratio()
{
    if(!ui->penemy->text().isEmpty()&&ui->penemy->text().toFloat()>=0)
        return ui->penemy->text().toFloat();
    else
        return -1;
}

int MainWindow::getXEnemyNr()
{
    if(!ui->xenemy->text().isEmpty()&&ui->xenemy->text().toInt()>=0)
        return ui->xenemy->text().toInt();
    else
        return -1;
}

int MainWindow::getZEnemyNr()
{
    if(!ui->zenemy->text().isEmpty()&&ui->zenemy->text().toInt()>=0)
        return ui->zenemy->text().toInt();
    else
        return -1;
}

int MainWindow::getHealthPackNr()
{
    if(!ui->healthpack->text().isEmpty()&&ui->healthpack->text().toInt()>=0)
        return ui->healthpack->text().toInt();
    else
        return -1;
}

void MainWindow::wheelEvent(QWheelEvent *event)
{
    double factor = 1.15;
    if(event->angleDelta().y()>0)
    {
        ui->graphicsView->scale(factor,factor);
    }
    else{
        ui->graphicsView->scale(1/factor,1/factor);
    }
}

void MainWindow::on_setPosition_clicked()
{
    int x=ui->set_X->text().toInt();
    int y=ui->set_Y->text().toInt();
    ctl->setPosition(x,y);
}

//*****************Find the nearest health pack*****************
void MainWindow::on_findNearestHealthPack_clicked()
{
    if(ctl->getPf()->nearestPack(model->get_model_protg()->getXPos(),model->get_model_protg()->getYPos())!=nullptr)
    {
        int target_x=ctl->getPf()->nearestPack(model->get_model_protg()->getXPos(),model->get_model_protg()->getYPos())->x;
        int target_y=ctl->getPf()->nearestPack(model->get_model_protg()->getXPos(),model->get_model_protg()->getYPos())->y;
        ui->target_X->setText(QString::number(target_x));
        ui->target_Y->setText(QString::number(target_y));
        on_findpath_button_clicked();
        ui->target_X->clear();
        ui->target_Y->clear();
    }
    else
        showNoHealthPack();

}

//*****************attack the nearest enemy*****************
void MainWindow::on_findNeareastEnemy_clicked()
{
    if(ctl->checkEnemyAlive())
    {
        model->sortEnemy(model->get_model_protg()->getXPos(),model->get_model_protg()->getYPos());
        for(auto& a:model->get_model_enemies())
        {
            if(!a->getDefeated())
            {
                int target_x=a->getXPos();
                int target_y=a->getYPos();
                ui->target_X->setText(QString::number(target_x));
                ui->target_Y->setText(QString::number(target_y));
                on_findpath_button_clicked();
                ui->target_X->clear();
                ui->target_Y->clear();
                break;
            }
        }
    }
    else
        showNoEnemy();
}

//*****************attack the second nearest enemy*****************
void MainWindow::attackSecondNearestEnemy()
{
    int count=0;
    for(auto& a:model->get_model_enemies())
    {
        if(!a->getDefeated())
        {
            count++;
            if(count==2)
            {
                int target_x=a->getXPos();
                int target_y=a->getYPos();
                ui->target_X->setText(QString::number(target_x));
                ui->target_Y->setText(QString::number(target_y));
                on_findpath_button_clicked();
                ui->target_X->clear();
                ui->target_Y->clear();
                break;
            }
        }
    }
}

//*****************attack the third nearest enemy*****************
void MainWindow::attackThirdNearestEnemy()
{
    int count=0;
    for(auto& a:model->get_model_enemies())
    {
        if(!a->getDefeated())
        {
            count++;
            if(count==3)
            {
                int target_x=a->getXPos();
                int target_y=a->getYPos();
                ui->target_X->setText(QString::number(target_x));
                ui->target_Y->setText(QString::number(target_y));
                on_findpath_button_clicked();
                ui->target_X->clear();
                ui->target_Y->clear();
                break;
            }
        }
    }
}

//*****************Autoplay 1 : find neareast pack if health is insufficient to attack the neareast enemy*****************
void MainWindow::on_autoplay_1_clicked()
{
    setEndAutoplay(false);
    while (!ctl->getGameover_flag()&&!endAutoplay) {
      model->sortEnemy(model->get_model_protg()->getXPos(),model->get_model_protg()->getYPos());
      if (model->get_model_protg()->getHealth() > model->findNearestEnemyValue(model->get_model_protg()->getXPos(),model->get_model_protg()->getYPos())){
           on_findNeareastEnemy_clicked();
      }
      else {
          if(model->get_model_healthPacks().size()>0)
              on_findNearestHealthPack_clicked();
          else
              on_findNeareastEnemy_clicked();
      }
      if(!ctl->getPf()->getPath_found())
      {
          //showBlockedOverMsg();
          break;
      }
    }
}

//*****************Autoplay 2: set a threshold, if health is lower than the threshold, then it would find the neareast healthpack*****************
void MainWindow::on_autoplay_2_clicked()
{
    setEndAutoplay(false);
     while (!ctl->getGameover_flag()&&!endAutoplay) {
       model->sortEnemy(model->get_model_protg()->getXPos(),model->get_model_protg()->getYPos());
       if (model->get_model_protg()->getHealth() < 100 || model->get_model_protg()->getHealth() < model->findNearestEnemyValue(model->get_model_protg()->getXPos(),model->get_model_protg()->getYPos()) ){
           if(model->get_model_healthPacks().size()>0)
               on_findNearestHealthPack_clicked();
           else
               on_findNeareastEnemy_clicked();
       }
       else {
           on_findNeareastEnemy_clicked();
       }
       if(!ctl->getPf()->getPath_found())
       {
           //showBlockedOverMsg();
           break;
       }
    }
}

//*****************Autoplay 3: Check the value of the nearest, the second nearest and the third nearest enemy in sequence to maximize the usage of health *****************
void MainWindow::on_autoplay_3_clicked()
{
    setEndAutoplay(false);
    while (!ctl->getGameover_flag()&&!endAutoplay){
        model->sortEnemy(model->get_model_protg()->getXPos(),model->get_model_protg()->getYPos());
        float nearestValue=model->findNearestEnemyValue(model->get_model_protg()->getXPos(),model->get_model_protg()->getYPos());
        float secondNearestValue=model->findSecondNearestEnemyValue(model->get_model_protg()->getXPos(),model->get_model_protg()->getYPos());
        float thirdNearestValue=model->findThirdNearestEnemyValue(model->get_model_protg()->getXPos(),model->get_model_protg()->getYPos());

        if (nearestValue>0 && model->get_model_protg()->getHealth() > nearestValue ){
             on_findNeareastEnemy_clicked();
        }
        else if(secondNearestValue>0 && model->get_model_protg()->getHealth() >secondNearestValue)
        {
            attackSecondNearestEnemy();
        }
        else if(thirdNearestValue>0 && model->get_model_protg()->getHealth() >thirdNearestValue)
        {
            attackThirdNearestEnemy();
        }
        else {
            if(model->get_model_healthPacks().size()>0)
                on_findNearestHealthPack_clicked();
            else
                on_findNeareastEnemy_clicked();
        }
        if(!ctl->getPf()->getPath_found())
        {
            break;
        }
    }
}

void MainWindow::on_save_clicked()
{
    ctl->saveGame();
}

void MainWindow::on_load_clicked()
{
    ctl->loadGame();
}

bool MainWindow::getToggle() const
{
    return toggle;
}

void MainWindow::on_change_Heuristic_clicked()
{
    float heuristic=ui->heuristic->text().toFloat();
    if(heuristic>=0)
    {
        ctl->getPf()->setHeuristicWeight(heuristic);
    }
}

void MainWindow::on_endAutoPlay_clicked()
{
    setEndAutoplay(true);
}

void MainWindow::setRestartFlag(bool value)
{
    restartFlag = value;
}

bool MainWindow::getRestartFlag() const
{
    return restartFlag;
}

void MainWindow::setEndAutoplay(bool value)
{
    endAutoplay = value;
}

//*****************Get the max health defined by user*****************
float MainWindow::getMaxHealth()
{
    if(!ui->maxHealth->text().isEmpty()&&ui->maxHealth->text().toFloat()>0)
        return ui->maxHealth->text().toFloat();
    else
        return -1;
}

//*****************Get the max energy defined by user*****************
float MainWindow::getMaxEnegy()
{
    if(!ui->maxEnergy->text().isEmpty()&&ui->maxEnergy->text().toFloat()>0)
        return ui->maxEnergy->text().toFloat();
    else
        return -1;
}
