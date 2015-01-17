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
    MinimumSpanningTree( std::vector<Point *> vertices );
    double ComputeMST();
    double ComputeCost();
    std::vector<Edge> getSpanningTree();
    
private:
    void CreateEdges();
    double cost = 0.0;
    std::vector<Edge> edges;
    std::vector<Edge *> spanningTreeEdges;
    std::vector<Point *> vertices;
};

#endif /* defined(__PhaseUnwrapping__MinimumSpanningTree__) */
