#ifndef PATHFINDER_H
#define PATHFINDER_H
#include <memory>
#include <iostream>
#include <string>
#include <algorithm>
#include <memory>
#include <queue>
#include "gamemodel.h"
#include <math.h>
using namespace std;


class pathFinder
{
private:
    shared_ptr<gameModel> model;
    float heuristicWeight {10.0f};
    bool path_found = false;

public:

    struct aNode
        {
            int x;
            int y;
            float value;                      // first three are from tile

            bool wall = false;              // Is it a wall?(from enemy)
            bool closed = false;			// Is it closed?
            float TotalCost = numeric_limits<float>::infinity();
            // = heuristic + given. general assessment for this path's performance
            float GivenCost = numeric_limits<float>::infinity();
            // = accumulative additions of value until this node
            // (what is the cost until this node)
            // heuristic = what will the cost be for the rest
            vector<shared_ptr<aNode>> Neighbours;      // neighbours
            shared_ptr<aNode> parent;					// keep updating to find the shortest path
        };

    struct compareTotal
    {
        bool operator() (const shared_ptr<aNode> a, const shared_ptr<aNode> b)
        {
            return a->TotalCost > b->TotalCost;
        }
    };

    vector<shared_ptr<aNode>> nodes;
    priority_queue<shared_ptr<aNode>, vector<shared_ptr<aNode>>, compareTotal> openList;
    //vector<shared_ptr<aNode>> openList;
    vector<shared_ptr<aNode>> solution;



    pathFinder();
    pathFinder(shared_ptr<gameModel> m);
    void initPathFinder();          //the list of tiles, height and width, enemy
    void initialize();
    void findPath(int xStart, int yStart, int xEnd, int yEnd);                //start and end node
    void retracePath(shared_ptr<aNode>& start, shared_ptr<aNode>& end);             //use parent to find the whole path (same inputs)
    float getHeuristic(shared_ptr<aNode>& a, shared_ptr<aNode>& b);           //distance between two nodes
    float getHeuristicWeight() const;
    void setHeuristicWeight(float);
    vector<shared_ptr<aNode>> getSolution() const;
    void updateBlockedTiles();
    shared_ptr<aNode> nearestEnemy(int xStart, int yStart);
    shared_ptr<aNode> nearestPack(int xStart, int yStart);
    float getDistance(int xStart, int yStart, int xEnd, int yEnd);
    void setPath_found(bool value);
    bool getPath_found() const;
};


#endif // PATHFINDER_H


