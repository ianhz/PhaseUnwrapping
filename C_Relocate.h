//
//  C_Relocate.h
//  PhaseUnwrapping
//
//  Created by Ian Herszterg on 1/29/15.
//  Copyright (c) 2015 Ian Herszterg. All rights reserved.
//

#ifndef __PhaseUnwrapping__C_Relocate__
#define __PhaseUnwrapping__C_Relocate__

#include <stdio.h>
#include "Util.h"
#include "LocalSearch.h"
#include "MinimumSpanningTree.h"

#define MAX_DIST 35

double C_Relocate( std::vector<Group>& currentSolution, int g1, int g2, double currentCost );

#endif /* defined(__PhaseUnwrapping__C_Relocate__) */
