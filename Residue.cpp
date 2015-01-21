//
//  residueDetection.c
//  phase_unwrapping
//
//  Created by Ian Herszterg on 9/12/14.
//  Copyright (c) 2014 Ian Herszterg. All rights reserved.
//

#include <stdio.h>
#include "pi.h"
#include "Residue.h"

int SearchForResidues( float * imageData, unsigned char * bitMap, std::vector<Point>& pos_residues, std::vector<Point>& neg_residues, int w, int h )
{
    int nPos = 0, nRes = 0;
    
    for( unsigned int i = 1; i < h-1 ; ++i )
    {
        for( unsigned int j = 1; j < w-1; ++j )
        {
            if( bitMap[ i*w + j ] == FREE_PIXEL )
            {
                double difference = 0.0;
                
                double parcel = (double)imageData[ (i+1)*w + j ] - (double)imageData[ i*w + j ];
                
                if( parcel <= -PI )
                    parcel += TWOPI;
                else if(parcel > PI )
                    parcel -= TWOPI;
                
                difference += parcel;
                
                parcel = (double)imageData[ (i+1)*w + (j+1) ] - (double)imageData[ (i+1)*w + j ];
                
                if( parcel <= -PI  )
                    parcel += TWOPI;
                else if(parcel > PI )
                    parcel -= TWOPI;
                
                difference += parcel;
                
                parcel = (double)imageData[ i*w + (j+1) ] - (double)imageData[ (i+1)*w + (j+1) ];
                
                if( parcel <= -PI  )
                    parcel += TWOPI;
                else if(parcel > PI )
                    parcel -= TWOPI;
                
                difference += parcel;
                
                parcel = (double)imageData[ i*w + j ] - (double)imageData[ i*w + (j+1) ];
                
                if( parcel <= -PI  )
                    parcel += TWOPI;
                else if(parcel > PI  )
                    parcel -= TWOPI;
                
                difference += parcel;
                
                if( difference == TWOPI )
                {
                    bitMap[ i*w + j ] = POS_RESIDUE;
                    
                    Point res;
                    res.i = i;
                    res.j = j;
                    res.isBorder = false;
                    res.type = POS_RESIDUE;
                    
                    pos_residues.push_back(res);
                    
                    nPos++;
                }
                else if( difference == -TWOPI )
                {
                    bitMap[ i*w + j ] = NEG_RESIDUE;
                    
                    Point res;
                    res.i = i;
                    res.j = j;
                    res.isBorder = false;
                    res.type = NEG_RESIDUE;
                    
                    neg_residues.push_back(res);
                    
                    nRes++;
                }
            }
        }
    }
    
    return  nPos + nRes;
}