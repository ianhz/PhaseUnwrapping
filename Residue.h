//
//  Residue.h
//  unwrapping
//
//  Created by Ian Herszterg on 9/14/14.
//  Copyright (c) 2014 Ian Herszterg. All rights reserved.
//

#ifndef __unwrapping__Residue__
#define __unwrapping__Residue__

#include <iostream>
#include <vector>
#include "Point.h"

#define POS_RESIDUE 10
#define NEG_RESIDUE 5
#define BRANCH_CUT 1
#define ADJOIN 13
#define FREE_PIXEL 0
#define UNWRAPPED 20
#define ACTIVE_BALANCED_POS_RESIDUE 50
#define ACTIVE_BALANCED_NEG_RESIDUE 25
#define ACTIVE_POS_RESIDUE 55
#define ACTIVE_NEG_RESIDUE 60
#define INACTIVE_BALANCED_POS_RESIDUE 65
#define INACTIVE_BALANCED_NEG_RESIDUE 70

int SearchForResidues( float * imageData, unsigned char * bitMap, std::vector<Point>& pos_residues, std::vector<Point>& neg_residues, int w, int h );

#endif /* defined(__unwrapping__Residue__) */
