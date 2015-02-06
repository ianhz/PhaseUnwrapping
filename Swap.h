//
//  Swap.h
//  PhaseUnwrapping
//
//  Created by Ian Herszterg on 1/25/15.
//  Copyright (c) 2015 Ian Herszterg. All rights reserved.
//

#ifndef __PhaseUnwrapping__Swap__
#define __PhaseUnwrapping__Swap__

#include <stdio.h>
#include "Util.h"
#include "LocalSearch.h"
#include "MinimumSpanningTree.h"

#define MAX_DIST 20

double Swap( std::vector<Group>& currentSolution, int g1, int g2, double currentCost );

#endif /* defined(__PhaseUnwrapping__Swap__) */
