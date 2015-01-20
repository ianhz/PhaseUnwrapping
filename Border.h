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
void CreateBorderPoints( std::vector<Point> residues, std::vector<Point>& opp_residues, int w, int h );

#endif /* defined(__PhaseUnwrapping__Border__) */
