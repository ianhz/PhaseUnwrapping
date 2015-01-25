//
//  CostFunction.h
//  PhaseUnwrapping
//
//  Created by Ian Herszterg on 1/17/15.
//  Copyright (c) 2015 Ian Herszterg. All rights reserved.
//

#ifndef PhaseUnwrapping_CostFunction_h
#define PhaseUnwrapping_CostFunction_h

#include <vector>
#include "Point.h"

class CostFunction
{
public:
    virtual double ComputeCost() = 0;
    virtual void setInstance( std::vector<Point>& solution, std::vector<Point>& borders ) = 0;
};

#endif
