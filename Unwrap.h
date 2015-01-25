//
//  Unwrap.h
//  PhaseUnwrapping
//
//  Created by Ian Herszterg on 1/25/15.
//  Copyright (c) 2015 Ian Herszterg. All rights reserved.
//

#ifndef __PhaseUnwrapping__Unwrap__
#define __PhaseUnwrapping__Unwrap__

#include <stdio.h>
#include <vector>
#include <list>
#include "Point.h"
#include "Residue.h"

void UnwrapImage(double * solution, float * wrappedImg, unsigned char * bitMap, float * mask, int w, int h);

#endif /* defined(__PhaseUnwrapping__Unwrap__) */
