//
//  DepthFirstSearch.h
//  PhaseUnwrapping
//
//  Created by Ian Herszterg on 1/17/15.
//  Copyright (c) 2015 Ian Herszterg. All rights reserved.
//

#ifndef __PhaseUnwrapping__DepthFirstSearch__
#define __PhaseUnwrapping__DepthFirstSearch__

#include <stdio.h>
#include <vector>
#include "Point.h"
#include "Edge.h"

void DepthFirstSearch(std::vector<Point *>& points, std::vector<Edge>& edges, Point * start, int groupId, int& nPos, int& nNeg);
int FindDisconnectedPieces( std::vector<Point *>& points, std::vector<Edge>& edges, bool disconect );

#endif /* defined(__PhaseUnwrapping__DepthFirstSearch__) */
