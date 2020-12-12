#ifndef TEMPLATE_H
#define TEMPLATE_H
#include <vector>
template <typename T>
T find_object_in_vector(std::vector<T>& v, int x,int y)
{
    for(auto& a:v )
    {
        if(a->getXPos()==x&&a->getXPos()==y)
        {
            return a;
        }
    }
    return NULL;
}
#endif // TEMPLATE_H
