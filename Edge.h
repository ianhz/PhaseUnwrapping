//
//  Edge.h
//  PhaseUnwrapping
//
//  Created by Ian Herszterg on 1/17/15.
//  Copyright (c) 2015 Ian Herszterg. All rights reserved.
//

#ifndef __PhaseUnwrapping__Edge__
#define __PhaseUnwrapping__Edge__

#include <stdio.h>
#include "Point.h"
#define MAX_EDGE_COST 4

struct mst_node
{
    Point * p1;
    Point * p2;
    float cost;
    bool valid = true;
    bool visited = false;
}; typedef mst_node Edge;

#endif /* defined(__PhaseUnwrapping__Edge__) */
