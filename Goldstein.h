//
//  Goldstein.h
//  PhaseUnwrapping
//
//  Created by Ian Herszterg on 2/3/15.
//  Copyright (c) 2015 Ian Herszterg. All rights reserved.
//

#ifndef __PhaseUnwrapping__Goldstein__
#define __PhaseUnwrapping__Goldstein__

#include <stdio.h>
#include "Heuristic.h"
#include "Util.h"
#include "BresenhamAlgorithm.h"
#include "Residue.h"
#include "Border.h"

#define MASK_SIZE 3

class Goldstein : public Heuristic
{
public:
    void RunMethod();
    void setCostFunction( CostFunction c );
    Goldstein() {};
    ~Goldstein() {};
private:
    CostFunction * _cF;
};


#endif /* defined(__PhaseUnwrapping__Goldstein__) */
