//
//  MinimumSpanningTree.h
//  PhaseUnwrapping
//
//  Created by Ian Herszterg on 1/17/15.
//  Copyright (c) 2015 Ian Herszterg. All rights reserved.
//

#ifndef __PhaseUnwrapping__MinimumSpanningTree__
#define __PhaseUnwrapping__MinimumSpanningTree__

#include <stdio.h>
#include "Util.h"
#include "Point.h"
#include "Edge.h"
#include "CostFunction.h"
#include <vector>

struct subset
{
    int parentId;
    int rank;
}; typedef struct subset Subset;

bool sortFunction( Edge e1, Edge e2 );

class MinimumSpanningTree : public CostFunction
{
public:
    MinimumSpanningTree( std::vector<Point>& vertices, std::vector<Point>& borders );
    MinimumSpanningTree() {};
    ~MinimumSpanningTree() {};
    void setInstance(std::vector<Point>& v,std::vector<Point>& b);
    double ComputeMST( std::vector<Edge>& spanningTreeEdges );
    double ComputeCost();
    
private:
    int find( int i );
    void Union( int x, int y );
    void CreateEdges();
    double cost = 0.0;
    bool balanced = false;
    std::vector<Edge> edges;
    std::vector<Point *> vertices;
    std::vector<Point *> borders;
    std::vector<Subset> subsets;
};

#endif /* defined(__PhaseUnwrapping__MinimumSpanningTree__) */
