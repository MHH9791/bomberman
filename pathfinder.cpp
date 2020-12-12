#include "pathfinder.h"
#include <math.h>
#include <limits>
#include <memory>
#include <iostream>
#include <string>
#include <algorithm>
#include <iterator>

float pathFinder::getHeuristicWeight() const
{
    return heuristicWeight;
}

void pathFinder::setHeuristicWeight(float newWeight)
{
    heuristicWeight = newWeight;
}

vector<shared_ptr<pathFinder::aNode> > pathFinder::getSolution() const
{
    return solution;
}

void pathFinder::updateBlockedTiles()
{
    for (auto & tile : model->getBlocked_tiles()){
        nodes[tile->getYPos()*model->getCols()+tile->getXPos()]->wall = true;
    }
}

shared_ptr<pathFinder::aNode> pathFinder::nearestPack(int xStart, int yStart)
{
    float distance = 5000;
    shared_ptr<aNode> thisPack=nullptr;
    if(model->get_model_healthPacks().size()>0)
    {
        for (auto & pack : model->get_model_healthPacks()){
            if (distance > getDistance(xStart, yStart, pack->getXPos(), pack->getYPos())){
                distance = getDistance(xStart, yStart, pack->getXPos(), pack->getYPos());
                thisPack = nodes[pack->getYPos()*model->getCols()+pack->getXPos()];
            }
        }
    }
    return thisPack;
}

float pathFinder::getDistance(int xStart, int yStart, int xEnd, int yEnd)
{
    auto distance = sqrtf((xStart - xEnd)*(xStart - xEnd) + (yStart - yEnd)*(yStart - yEnd));
    return distance;
}


void pathFinder::setPath_found(bool value)
{
    path_found = value;
}

bool pathFinder::getPath_found() const
{
    return path_found;
}

pathFinder::pathFinder(shared_ptr<gameModel> m)
    :model{m}
{

}

void pathFinder::initPathFinder()
{
    setPath_found(false);
    while (!openList.empty()) {
        openList.pop();
    }
    nodes.clear();
    solution.clear();       //reset vectors

    int i = 0;
    for (auto & tile : model->get_model_tiles()) {             //get tiles and assign value to every node.
        auto newNode = make_shared<struct aNode>();
        nodes.emplace_back(newNode);
        if(tile->getValue()==INFINITY)
        {
            nodes[i]->value = tile->getValue();
            nodes[i]->wall=true;
        }
        else
            nodes[i]->value=1-tile->getValue();
        nodes[i]->x = tile->getXPos();
        nodes[i]->y = tile->getYPos();
        nodes[i]->parent = nullptr;
        i++;
    }

    for (auto & enemy : model->get_model_enemies()) {          //set every tile where enemy stands to a wall
        nodes[enemy->getYPos()*model->getCols()+enemy->getXPos()]->wall = true;
        nodes[enemy->getYPos()*model->getCols()+enemy->getXPos()]->value=enemy->getValue();
    }


    for (int j = 0; j < i; j++){            //find all neighbours for every node
        if(nodes[j]->x > 0){
            nodes[j]->Neighbours.emplace_back(nodes[((nodes[j]->y)*model->getCols()+(nodes[j]->x-1))]);
        }
        if(nodes[j]->x < model->getCols()-1){
            nodes[j]->Neighbours.emplace_back(nodes[((nodes[j]->y)*model->getCols()+(nodes[j]->x+1))]);
        }
        if(nodes[j]->y > 0){
            nodes[j]->Neighbours.emplace_back(nodes[((nodes[j]->y-1)*model->getCols()+nodes[j]->x)]);
        }
        if(nodes[j]->y < model->getRows()-1){
            nodes[j]->Neighbours.emplace_back(nodes[((nodes[j]->y+1)*model->getCols()+nodes[j]->x)]);
        }
    }

}

void pathFinder::initialize()
{
    while (!openList.empty()) {
        openList.pop();
    }
    solution.clear();

    for(auto & node: nodes){
        node->closed = false;
        node->GivenCost = numeric_limits<float>::infinity();
        node->TotalCost = numeric_limits<float>::infinity();
    }
    path_found = false;
}

//void pathFinder::findPath(pathFinder::aNode *start, pathFinder::aNode *end)
void pathFinder::findPath(int xStart, int yStart, int xEnd, int yEnd)
{
    initialize();
    bool flag = false;
    auto start = nodes[yStart*model->getCols()+xStart];
    auto end = nodes[yEnd*model->getCols()+xEnd];

    for (auto & enemy: model->get_model_enemies()){
        if(xEnd == enemy->getXPos() && yEnd == enemy->getYPos()){
            if (!enemy->getDefeated()){
                nodes[enemy->getYPos()*model->getCols()+enemy->getXPos()]->wall = false;
                flag = true;
            }
        }
    }
    if (end->wall){
        cout << "This target is not reachable:(" << std::endl;
        return;
    }

    auto currentNode = start;
    currentNode->GivenCost = currentNode->value;
    currentNode->TotalCost = currentNode->GivenCost+getHeuristic(start,end)*getHeuristicWeight();
    // create the startNode

    openList.emplace(currentNode);

    while(!openList.empty()){     //stop as soon as we found the 1st path
        while(!openList.empty() && openList.top()->closed){        //The 1st one can be closed! remove it
            openList.pop();
        }

        if (openList.empty()){      //check again if the openlist is empty
            break;
        }

        currentNode = openList.top();      //assign the best one //start assigned twice!!!
        currentNode->closed = true;  //move this to back???
        if(currentNode == end){        //stop as soon as we found the first path.
            retracePath(start, end);   //compare x and y.
            path_found = true;
            if(flag)
                nodes[yEnd*model->getCols()+xEnd]->wall = true;
            //initial in here (time-consuming)
            cout << "Pathfinding succeeded." << std::endl;
            return;
        }

        //start exploring its neighbours
        for(auto& neighbour : currentNode->Neighbours){

            if(!(neighbour->wall)){
                if ((currentNode->GivenCost + neighbour->value) < neighbour->GivenCost){       //update the neighbour if the newGivenCost is better
                    neighbour->parent = currentNode;
                    neighbour->GivenCost = currentNode->GivenCost + neighbour->value;
                    neighbour->TotalCost = neighbour->GivenCost + getHeuristic(neighbour, end)*getHeuristicWeight();
                }

                if (!neighbour->closed){
                    openList.emplace(neighbour);
                    //smartInsert(openList, neighbour);
                }
            }
        }
    }
    if(path_found){
        cout << "Pathfinding succeeded." << std::endl;
    }
    else if (!path_found) {
        cout << "Pathfinding failed." << std::endl;
    }
}

void pathFinder::retracePath(shared_ptr<aNode>& start, shared_ptr<aNode>& end)
{
    auto temp = end;
    solution.emplace_back(temp);
    while (temp != start) {
        solution.emplace_back(temp->parent);
        temp = temp->parent;
    }
    reverse(solution.begin(), solution.end());
}

float pathFinder::getHeuristic(shared_ptr<aNode>& a, shared_ptr<aNode>& b) //estimation
{
    auto distance = sqrtf((a->x - b->x)*(a->x - b->x) + (a->y - b->y)*(a->y - b->y));
    return distance;
}








