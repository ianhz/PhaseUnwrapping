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

int SearchForResidues( float * imageData, unsigned char * bitMap, int w, int h)
{
    int nPos = 0, nRes = 0;
    
    for( unsigned int i = 1; i < h - 1; ++ i )
    {
        for( unsigned int j = 1; j < w - 1; ++j )
        {
            float difference = 0.0;
            
            float parcel = imageData[ (i+1)*w + j ] - imageData[ i*w + j ];
            
            if(parcel < -PI)
                parcel += TWOPI;
            else if(parcel >= PI)
                parcel -= TWOPI;
            
            difference += parcel;
            
            parcel = imageData[ (i+1)*w + (j+1) ] - imageData[ (i+1)*w + j ];
            
            if(parcel < -PI)
                parcel += TWOPI;
            else if(parcel >= PI)
                parcel -= TWOPI;
            
            difference += parcel;
            
            parcel = imageData[ i*w + (j+1) ] - imageData[ (i+1)*w + (j+1) ];
            
            if(parcel < -PI)
                parcel += TWOPI;
            else if(parcel >= PI)
                parcel -= TWOPI;
            
            difference += parcel;
            
            parcel= imageData[ i*w + j ] - imageData[ i*w + (j+1) ];
            
            if(difference >= TWOPI)
            {
                bitMap[ i*w + j ] = POS_RESIDUE;
                nPos++;
            }
            else if( difference <= -TWOPI)
            {
                bitMap[ i*w + j ] = NEG_RESIDUE;
                nRes++;
            }
        }
    }
    
    return  nPos + nRes;
}