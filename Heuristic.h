//
//  Heuristic.h
//  PhaseUnwrapping
//
//  Created by Ian Herszterg on 1/17/15.
//  Copyright (c) 2015 Ian Herszterg. All rights reserved.
//

#ifndef __PhaseUnwrapping__Heuristic__
#define __PhaseUnwrapping__Heuristic__

#include <stdio.h>
#include <vector>
#include "Point.h"
#include "CostFunction.h"

class Heuristic
{
public:
    virtual void RunMethod() = 0;
    void SetInputs( unsigned char * bitmap, std::vector<Point> pos_residues, std::vector<Point> neg_residues, int w, int h);
    unsigned char * getBitmap();
    
protected:
    unsigned char * _bitmap;
    std::vector<Point> _pos_residues;
    std::vector<Point> _neg_residues;
    int _h,_w;
};

#endif /* defined(__PhaseUnwrapping__Heuristic__) */
