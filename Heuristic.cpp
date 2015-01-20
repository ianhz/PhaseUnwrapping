//
//  Heuristic.cpp
//  PhaseUnwrapping
//
//  Created by Ian Herszterg on 1/18/15.
//  Copyright (c) 2015 Ian Herszterg. All rights reserved.
//

#include <stdio.h>
#include "Heuristic.h"

void Heuristic::SetInputs( unsigned char * bitmap, std::vector<Point> pos_residues, std::vector<Point> neg_residues, int w, int h)
{
    _bitmap = bitmap;
    _pos_residues = pos_residues;
    _neg_residues = neg_residues;
    _w = w;
    _h = h;
}


unsigned char * Heuristic::getBitmap()
{
    return _bitmap;
}