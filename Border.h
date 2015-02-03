//
//  Border.h
//  PhaseUnwrapping
//
//  Created by Ian Herszterg on 1/18/15.
//  Copyright (c) 2015 Ian Herszterg. All rights reserved.
//

#ifndef __PhaseUnwrapping__Border__
#define __PhaseUnwrapping__Border__

#include <stdio.h>
#include <vector>
#include "Point.h"

/* Create border vertices */
void CreateBorderPoints( std::vector<Point> residues, std::vector<Point>& border, int w, int h );
void CreateClosestBorderPoint( std::vector<Point>& residues, int w, int h );
void FindClosestBorderPoint( int i, int j, int w, int h, int& bi, int& bj );
#endif /* defined(__PhaseUnwrapping__Border__) */
