#ifndef TEXTCOMPONENT_H
#define TEXTCOMPONENT_H

#include <QString>
class textComponent : public QString{
public:
    textComponent(const QString& text)
        :QString(text)
    {}
    ~textComponent(){};
};

class t_boundary:public textComponent{
public:
    t_boundary()
        :textComponent("——")
    {}
    ~t_boundary(){};
};

class t_tile:public textComponent
{
public:
    t_tile()
        :textComponent("|      ")
    {}
    ~t_tile(){};
};

class t_black_tile:public textComponent
{
public:
    t_black_tile()
        :textComponent("|  :   ")

    {}
    ~t_black_tile(){};
};

class t_enemy:public textComponent
{
public:
    t_enemy()
        :textComponent("|  E  ")
    {}
    ~t_enemy(){};
};

class t_Penemy:public textComponent
{
public:
    t_Penemy()
        :textComponent("|  P  ")
    {}
    ~t_Penemy(){};
};

class t_Xenemy:public textComponent
{
public:
    t_Xenemy()
        :textComponent("|  X  ")
    {}
    ~t_Xenemy(){};
};

class t_Zenemy:public textComponent
{
public:
    t_Zenemy()
        :textComponent("|  Z  ")
    {}
    ~t_Zenemy(){};
};

class t_healthPack:public textComponent
{
public:
    t_healthPack()
        :textComponent("|  H  ")
    {}
    ~t_healthPack(){};
};

class t_protagonist:public textComponent
{
public:
    t_protagonist()
    :textComponent("|  $  ")
    {}
    ~t_protagonist(){};
};

class t_attacking_protagonist:public textComponent
{
public:
    t_attacking_protagonist()
    :textComponent("| A$ ")
    {}
    ~t_attacking_protagonist(){};
};

class t_health_protagonist:public textComponent
{
public:
    t_health_protagonist()
    :textComponent("| H$ ")
    {}
    ~t_health_protagonist(){};
};

class t_poisoned_protagonist:public textComponent
{
public:
    t_poisoned_protagonist()
    :textComponent("| P$ ")
    {}
    ~t_poisoned_protagonist(){};
};



class t_defeated_enemy:public textComponent
{
public:
    t_defeated_enemy()
        :textComponent("| DE ")
    {}
    ~t_defeated_enemy(){};
};

class t_defeated_penemy:public textComponent
{
public:
    t_defeated_penemy()
        :textComponent("| DP ")
    {}
    ~t_defeated_penemy(){};
};

class t_defeated_xenemy:public textComponent
{
public:
    t_defeated_xenemy()
        :textComponent("| DX ")
    {}
    ~t_defeated_xenemy(){};
};

class t_defeated_zenemy:public textComponent
{
public:
    t_defeated_zenemy()
        :textComponent("| DZ ")
    {}
    ~t_defeated_zenemy(){};
};

class t_poisoned_tile:public textComponent
{
public:
    t_poisoned_tile()
        :textComponent("|  ~  ")
    {}
    ~t_poisoned_tile(){};
};

class t_blocked_tile:public textComponent
{
public:
    t_blocked_tile()
        :textComponent("|  #  ")
    {}
    ~t_blocked_tile(){};
};




#endif // TEXTCOMPONENT_H
