//
//  BresenhamAlgorithm.h
//  unwrapping
//
//  Created by Ian Herszterg on 10/7/14.
//  Copyright (c) 2014 Ian Herszterg. All rights reserved.
//

#ifndef __unwrapping__BresenhamAlgorithm__
#define __unwrapping__BresenhamAlgorithm__

#include <stdio.h>
#include "Residue.h"
#include <math.h>

void Bresenham(unsigned char * bitMap, int i1,
               int j1,
               int i2,
               int j2, int w);
#endif /* defined(__unwrapping__BresenhamAlgorithm__) */
