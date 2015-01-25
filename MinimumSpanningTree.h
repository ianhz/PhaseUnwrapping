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
    void CreateEdges();
    double cost = 0.0;
    bool balanced = false;
    std::vector<Edge> edges;
    std::vector<Point *> vertices;
    std::vector<Point *> borders;
};

#endif /* defined(__PhaseUnwrapping__MinimumSpanningTree__) */
