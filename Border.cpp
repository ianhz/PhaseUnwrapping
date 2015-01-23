//
//  Border.cpp
//  PhaseUnwrapping
//
//  Created by Ian Herszterg on 1/18/15.
//  Copyright (c) 2015 Ian Herszterg. All rights reserved.
//

#include "Border.h"

/* Create border vertices */
void CreateBorderPoints( std::vector<Point>& residues, int w, int h )
{
    unsigned int original_size = (unsigned int)residues.size();
    
    for( unsigned int i = 0; i < original_size; ++i )
    {
        int h_distance, v_distance;
        
        if( residues[i].i < (h-1) - residues[i].i )
            v_distance = residues[i].i;
        else
            v_distance = (h-1) - residues[i].i;
        
        if( residues[i].j < (w-1) - residues[i].j )
            h_distance = residues[i].j;
        else
            h_distance = (w-1) - residues[i].j;
        
        Point border;
        
        if( h_distance < v_distance )
        {
            border.i = residues[i].i;
            
            if( h_distance == residues[i].j )
                border.j = 0;
            else
                border.j = w-1;
        }
        else
        {
            border.j = residues[i].j;
            
            if( v_distance == residues[i].i )
                border.i = 0;
            else
                border.i = h-1;
        }
        
        border.isBorder = true;
        border.delta = -5;
        residues.push_back(border);
    }
}
