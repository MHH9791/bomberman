#include "xenemy.h"

XEnemy::XEnemy(int xPosition, int yPosition, float strength,shared_ptr<gameModel> model)
    :Enemy(xPosition,yPosition,strength),model{model}
{

}

bool XEnemy::random_move()
{
    if(attacked_times>=2)
    {
        setDefeated(true);
        emit dead();
        return false;
    }
    else
    {
        bool flag=true;
        bool continue_flag;
        int col=model->getCols();
        int row=model->getRows();
        int random_x;
        int random_y;
        while(flag)
        {
            continue_flag=false;
            random_x=rand()%col;
            random_y=rand()%row;
            for(auto& a:model->get_model_enemies())
            {
                if(random_x==a->getXPos()&&random_y==a->getYPos())
                {
                    continue_flag=true;
                    break;
                }
            }
            if(continue_flag)
                continue;
            for(auto& a:model->get_model_healthPacks())
            {
                if(random_x==a->getXPos()&&random_y==a->getYPos())
                {
                    continue_flag=true;
                    break;
                }
            }
            if(continue_flag)
                continue;
            for(auto& a:model->getBlocked_tiles())
            {
                if(random_x==a->getXPos()&&random_y==a->getYPos())
                {
                    continue_flag=true;
                    break;
                }
            }
            if(continue_flag)
                continue;
            if(random_x==model->get_model_protg()->getXPos()&&random_y==model->get_model_protg()->getYPos())
            {
                continue;
            }
            flag=false;
        }
        previous_X=getXPos();
        previous_Y=getYPos();
        setXPos(random_x);
        setYPos(random_y);
        attacked_times++;
        return true;
    }
}

int XEnemy::getPrevious_Y() const
{
    return previous_Y;
}

int XEnemy::getPrevious_X() const
{
    return previous_X;
}
