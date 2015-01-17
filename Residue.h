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

#define POS_RESIDUE 10
#define NEG_RESIDUE 5
#define BRANCH_CUT 1
#define FREE_PIXEL 0
#define BALANCED_POS_RESIDUE 50
#define BALANCED_NEG_RESIDUE 25

int SearchForResidues( float * imageData, unsigned char * bitMap, int w, int h);

#endif /* defined(__unwrapping__Residue__) */
