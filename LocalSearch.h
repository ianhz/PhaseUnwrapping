//
//  LocalSearch.h
//  PhaseUnwrapping
//
//  Created by Ian Herszterg on 1/17/15.
//  Copyright (c) 2015 Ian Herszterg. All rights reserved.
//

#ifndef __PhaseUnwrapping__LocalSearch__
#define __PhaseUnwrapping__LocalSearch__

#include <stdio.h>
#include <stdlib.h>
#include "Group.h"
#include "Heuristic.h"
#include "Util.h"

void CloneSolution(std::vector<Group> source, std::vector<Group>& destination);

class LocalSearch
{
public:
    void Run(std::vector<Group>& currentSolution, int n, double * groupCosts);
    void setCostFunction(CostFunction * cF);
    void Swap(std::vector<Group>& neighborSolution, int g1, int g2, int i );
    LocalSearch(){};
    ~LocalSearch(){};
private:
    CostFunction * _cF;
    
};


#endif /* defined(__PhaseUnwrapping__LocalSearch__) */
