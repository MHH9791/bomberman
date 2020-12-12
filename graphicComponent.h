#ifndef PIXITEM_H
#define PIXITEM_H
#include <QGraphicsPixmapItem>
#include <iostream>
#include "world.h"

//double static MapSize;

class graphicComponent : public QGraphicsPixmapItem
{
public:
    graphicComponent(const QPixmap & pixmap)
        :QGraphicsPixmapItem(pixmap)
    {}
    ~graphicComponent(){};
};

class g_protg : public graphicComponent
{
public:
    g_protg(const QPixmap & pixmap)
        :graphicComponent(pixmap)
    {}
    g_protg(const double componentSize)
        :graphicComponent(QPixmap(":/image/protg.png").scaledToWidth((componentSize<5)?5:componentSize)){}
    ~g_protg(){}

};

class g_protg_under_attack : public graphicComponent
{
public:
    g_protg_under_attack(const QPixmap & pixmap)
        :graphicComponent(pixmap)
    {}
    g_protg_under_attack(const double componentSize)
        :graphicComponent(QPixmap(":/image/attack.png").scaledToWidth((componentSize<5)?5:componentSize)){}
    ~g_protg_under_attack(){}

};

class g_protg_under_health : public graphicComponent
{
public:
    g_protg_under_health(const QPixmap & pixmap)
        :graphicComponent(pixmap)
    {}
    g_protg_under_health(const double componentSize)
        :graphicComponent(QPixmap(":/image/meetHealth.png").scaledToWidth((componentSize<5)?5:componentSize)){}
    ~g_protg_under_health(){}

};

class g_protg_under_posion : public graphicComponent
{
public:
    g_protg_under_posion(const QPixmap & pixmap)
        :graphicComponent(pixmap)
    {}
    g_protg_under_posion(const double componentSize)
        :graphicComponent(QPixmap(":/image/getPoisoned.png").scaledToWidth((componentSize<5)?5:componentSize)){}
    ~g_protg_under_posion(){}

};

class g_healthPack : public graphicComponent
{
public:
    g_healthPack(const QPixmap & pixmap)
        :graphicComponent(pixmap)
    {}
    g_healthPack(const double componentSize)
        :graphicComponent(QPixmap(":/image/healthpack.png").scaledToWidth((componentSize<5)?5:componentSize))
    {}
    ~g_healthPack(){}
};

//class PixItem_Poison : public graphicComponent
//{
//public:
//    PixItem_Poison(const QPixmap & pixmap,QGraphicsItem *parent = nullptr):graphicComponent(pixmap, parent){}
//    PixItem_Poison(QGraphicsItem *parent = nullptr):graphicComponent(QPixmap(1,1).scaledToWidth(Psize), parent){}
//    ~PixItem_Poison(){}
//};

class g_pTiles : public graphicComponent
{
public:
    g_pTiles(const QPixmap & pixmap)
        :graphicComponent(pixmap){}
    g_pTiles(const double componentSize)
        :graphicComponent(QPixmap(":/image/posion.png").scaledToWidth((componentSize<5)?5:componentSize)){}
    ~g_pTiles(){}
};

class g_bTiles : public graphicComponent
{
public:
    g_bTiles(const QPixmap & pixmap)
        :graphicComponent(pixmap){}
    g_bTiles(const double componentSize)
        :graphicComponent(QPixmap(":/image/blocked.png").scaledToWidth((componentSize<5)?5:componentSize)){}
    ~g_bTiles(){}
};


class g_enemy : public graphicComponent
{
public:
    g_enemy(const QPixmap & pixmap)
        :graphicComponent(pixmap){}
    g_enemy(const double componentSize)
        :graphicComponent(QPixmap(":/image/enemy.png").scaledToWidth((componentSize<5)?5:componentSize)){}
    ~g_enemy(){}
};

class g_deadEnemy : public g_enemy
{
public:
    g_deadEnemy(const QPixmap & pixmap)
        :g_enemy(pixmap){}
    g_deadEnemy(const double componentSize)
        :g_enemy(QPixmap(":/image/dead.svg").scaledToWidth((componentSize<5)?5:componentSize)){}
    ~g_deadEnemy(){}
};

class g_pEnemy : public g_enemy
{
public:
    g_pEnemy(const QPixmap & pixmap)
        :g_enemy(pixmap){}
    g_pEnemy(const double componentSize)
        :g_enemy(QPixmap(":/image/penemy.png").scaledToWidth((componentSize<5)?5:componentSize)){}
    ~g_pEnemy(){}
};

class g_xEnemy : public g_enemy
{
public:
    g_xEnemy(const QPixmap & pixmap)
        :g_enemy(pixmap){}
    g_xEnemy(const double componentSize)
        :g_enemy(QPixmap(":/image/xenemy.png").scaledToWidth((componentSize<5)?5:componentSize)){}
    ~g_xEnemy(){}
};

class g_zEnemy : public g_enemy
{
public:
    g_zEnemy(const QPixmap & pixmap)
        :g_enemy(pixmap){}
    g_zEnemy(const double componentSize)
        :g_enemy(QPixmap(":/image/zenemy.png").scaledToWidth((componentSize<5)?5:componentSize)){}
    ~g_zEnemy(){}
};

class g_solutionNode : public graphicComponent
{
public:
    g_solutionNode(const QPixmap & pixmap)
        :graphicComponent(pixmap){}
    g_solutionNode(const double componentSize)
        :graphicComponent(QPixmap(":/image/yellow_star.png").scaledToWidth((componentSize<5)?5:componentSize)){}
    ~g_solutionNode(){}
};

#endif // PIXITEM_H


