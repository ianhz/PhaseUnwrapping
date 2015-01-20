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
    void InitProcedures();
    void setCostFunction( CostFunction c );
    void Shake( std::vector<Group>& solution );
    IteratedLocalSearch() {};
    ~IteratedLocalSearch() {};
private:
    double * currentGroupCosts;
    double * bestSolutionGroupCosts;
    void GenerateInitialSolution();
    std::vector<Group> bestSolution;
    double bestSolutionCost;
    CostFunction * _cF;
    int _k;
};
#endif /* defined(__PhaseUnwrapping__IteratedLocalSearch__) */
