//
//  IteratedLocalSearch.h
//  PhaseUnwrapping
//
//  Created by Ian Herszterg on 1/17/15.
//  Copyright (c) 2015 Ian Herszterg. All rights reserved.
//

#ifndef __PhaseUnwrapping__IteratedLocalSearch__
#define __PhaseUnwrapping__IteratedLocalSearch__

#include <stdio.h>
#include "Heuristic.h"
#include "LocalSearch.h"
#include "MinimumSpanningTree.h"
#include "Border.h"
#include "DepthFirstSearch.h"
#include "Util.h"
#include "Group.h"
#include "BresenhamAlgorithm.h"
#include "Residue.h"

class IteratedLocalSearch : public Heuristic
{
public:
    void RunMethod();
    void setCostFunction( CostFunction c );
    IteratedLocalSearch() {};
    ~IteratedLocalSearch() {};
private:
    void Shake( std::vector<Group>& solution );
    void GenerateInitialSolution();
    std::vector<Group> bestSolution;
    std::vector<Group>  initialSolution;
    double bestSolutionCost;
    double initialSolutionCost;
    CostFunction * _cF;
    int _k;
};
#endif /* defined(__PhaseUnwrapping__IteratedLocalSearch__) */
