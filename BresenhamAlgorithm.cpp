//
//  BresenhamAlgorithm.cpp
//  unwrapping
//
//  Created by Ian Herszterg on 10/7/14.
//  Copyright (c) 2014 Ian Herszterg. All rights reserved.
//

#include "BresenhamAlgorithm.h"

#include <cstdlib>

////////////////////////////////////////////////////////////////////////////////
void Bresenham(unsigned char * bitMap, int i1,
               int j1,
               int i2,
               int j2, int w)
{
    
    int x1,x2,y1,y2;
    
    x1 = j1;
    x2 = j2;
    y1 = i1;
    y2 = i2;

    int delta_x(x2 - x1);
    // if x1 == x2, then it does not matter what we set here
    signed char const ix((delta_x > 0) - (delta_x < 0));
    delta_x = std::abs(delta_x) << 1;
    
    int delta_y(y2 - y1);
    // if y1 == y2, then it does not matter what we set here
    signed char const iy((delta_y > 0) - (delta_y < 0));
    delta_y = std::abs(delta_y) << 1;
    

    if (delta_x >= delta_y)
    {
        // error may go below zero
        int error(delta_y - (delta_x >> 1));
        
        while (x1 != x2)
        {
            if ((error >= 0) && (error || (ix > 0)))
            {
                error -= delta_x;
                y1 += iy;
            }
            // else do nothing
            
            error += delta_y;
            x1 += ix;
            
            if( bitMap[ y1 * w + x1] != POS_RESIDUE && bitMap[ y1 * w + x1] != NEG_RESIDUE)
                bitMap[ y1 * w + x1] = BRANCH_CUT;
        }
    }
    else
    {
        // error may go below zero
        int error(delta_x - (delta_y >> 1));
        
        while (y1 != y2)
        {
            if ((error >= 0) && (error || (iy > 0)))
            {
                error -= delta_y;
                x1 += ix;
            }
            // else do nothing
            
            error += delta_x;
            y1 += iy;
            
            if( bitMap[ y1 * w + x1] != POS_RESIDUE && bitMap[ y1 * w + x1] != NEG_RESIDUE)
                bitMap[ y1 * w + x1] = BRANCH_CUT;
        }
    }
}