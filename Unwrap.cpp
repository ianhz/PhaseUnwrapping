//
//  Unwrap.cpp
//  PhaseUnwrapping
//
//  Created by Ian Herszterg on 1/25/15.
//  Copyright (c) 2015 Ian Herszterg. All rights reserved.
//

#include "Unwrap.h"
#include "pi.h"

void ComputePhase( double * solution, float * wrappedImg, unsigned char * bitMap, int i1, int j1, int i2, int j2, int w)
{
    double delta = (double)wrappedImg[ i1 * w + j1 ] - (double)wrappedImg[ i2 * w + j2 ];
    
    if( delta <= -PI )
    {
        delta += TWOPI;
    }
    else if ( delta > PI )
    {
        delta -= TWOPI;
    }
    
    solution[ i1 * w + j1 ] = solution[i2 * w + j2 ] + delta;
    bitMap[i1 * w + j1] = UNWRAPPED;
}

bool isBranchPixel( int i, int j, unsigned char * bitmap, int w )
{
    if(bitmap[ i * w + j] == POS_RESIDUE || bitmap[ i * w + j] == NEG_RESIDUE || bitmap[ i * w + j] == BRANCH_CUT )
        return true;
    else
        return false;
}

bool isBorderPixel( int i, int j, int w, int h)
{
    if(i > 0 && i < h - 1 && j > 0 && j < w - 1)
        return false;
    else
        return true;
}

bool isAdjoinPixel(int i, int j, unsigned char * bitmap, int w)
{
    if( bitmap[ i*w + j ] == ADJOIN )
        return true;
    
    return false;
}

bool isUnwrappedPixel(int i, int j, unsigned char * bitmap, int w)
{
    if( bitmap[ i*w + j ] == UNWRAPPED)
        return true;
    
    return false;
}

bool AddAdjoinPixels( Point current, unsigned char * bitmap, int w, int h)
{
    if( current.i < 0 || current.i >= h || current.j < 0 || current.j >= w )
        return false;
    
    /* if not a border  or residue/branch_cut pixel... */
    if( !isBranchPixel(current.i, current.j, bitmap, w) && !isBorderPixel(current.i, current.j, w, h))
    {
        /* if not a already on the adjoin list and an unwrapped pixel... */
        if( !isAdjoinPixel(current.i, current.j, bitmap, w) && !isUnwrappedPixel(current.i, current.j, bitmap, w))
        {
            return true;
        }
    }
    
    return false;
}


void UnwrapPixels(double * solution, float * wrappedImg, unsigned char * bitMap, float * mask, int w, int h, int starting_i, int starting_j)
{
    Point startingPixel;
    
    /* Starts with a not unwrapped pixel */
    startingPixel.i = starting_i;
    startingPixel.j = starting_j;
    
    std::list<Point> adjoinPixels;
    
    adjoinPixels.push_back(startingPixel);
    
    /* While there is still pixels to be unwrapped... */
    while( !adjoinPixels.empty() )
    {
        Point current = adjoinPixels.front();
        
        //printf("Unwrapping pixel (%d,%d)\n", current.j, current.i);
        
        Point n1,n2,n3,n4;
        
        n1.i = current.i - 1;
        n1.j = current.j;
        n1.parent_i = current.i;
        n1.parent_j = current.j;
        
        n2.i = current.i;
        n2.j = current.j - 1;
        n2.parent_i = current.i;
        n2.parent_j = current.j;
        
        n3.i = current.i + 1;
        n3.j = current.j;
        n3.parent_i = current.i;
        n3.parent_j = current.j;
        
        n4.i = current.i;
        n4.j = current.j + 1;
        n4.parent_i = current.i;
        n4.parent_j = current.j;
        
        if(AddAdjoinPixels( n1, bitMap, w, h ) && mask[n1.i * w + n1.j] == 255 )
        {
            adjoinPixels.push_back(n1);
            bitMap[ n1.i * w + n1. j ] = ADJOIN;
        }
        if(AddAdjoinPixels( n2, bitMap, w, h ) && mask[n2.i * w + n2.j] == 255 )
        {
            adjoinPixels.push_back(n2);
            bitMap[ n2.i * w + n2. j ] = ADJOIN;
        }
        if(AddAdjoinPixels( n3, bitMap, w, h ) && mask[n3.i * w + n3.j] == 255 )
        {
            adjoinPixels.push_back(n3);
            bitMap[ n3.i * w + n3. j ] = ADJOIN;
        }
        if(AddAdjoinPixels( n4, bitMap, w, h ) && mask[n4.i * w + n4.j] == 255 )
        {
            adjoinPixels.push_back(n4);
            bitMap[ n4.i * w + n4. j ] = ADJOIN;
        }
        
        /* Unwraps the current pixel */
        if( current.i != startingPixel.i || current.j != startingPixel.j )
        {
            ComputePhase(solution, wrappedImg, bitMap, current.i, current.j, current.parent_i, current.parent_j, w);
        }
        
        adjoinPixels.pop_front();
    }
}

void UnwrapPostponedPixel( double * solution, float * wrappedImg, unsigned char * bitMap, int w, int i, int j )
{
   if( bitMap[ i*w + (j-1)] == UNWRAPPED )
   {
       ComputePhase(solution, wrappedImg, bitMap, i, j, i, j-1, w);
   }
   else if( bitMap[ (i-1)*w + j] == UNWRAPPED )
   {
       ComputePhase(solution, wrappedImg, bitMap, i, j, i-1, j, w);
   }
   else if( bitMap[ i*w + (j+1)] == UNWRAPPED )
   {
       ComputePhase(solution, wrappedImg, bitMap, i, j, i, j+1, w);
   }
   else if( bitMap[ (i+1)*w + j] == UNWRAPPED )
   {
       ComputePhase(solution, wrappedImg, bitMap, i, j, i+1, j, w);
   }
}

void UnwrapImage(double * solution, float * wrappedImg, unsigned char * bitMap, float * mask, int w, int h)
{
    /* Unwrap non-branch pixels */
    for(unsigned int i = 1; i < h-1; i++)
    {
        for(unsigned int j = 1; j < w-1; ++j)
        {
            if( bitMap[ i * w + j] == FREE_PIXEL && mask[ i * w + j] == 255 )
            {
                bitMap[ i * w + j ] = UNWRAPPED;
                solution[ i * w + j ] = wrappedImg[ i* w + j ];
                
                UnwrapPixels(solution, wrappedImg, bitMap, mask, w, h, i, j);
                break;
            }
        }
        
        break;
    }
    
    
    /* Unwrap branch and residues pixels */
    for(unsigned int i = 0; i < h; ++i)
    {
        for(unsigned int j = 0; j < w; ++j)
        {
            if( bitMap[ i * w + j] != UNWRAPPED &&  bitMap[ i * w + j] != FREE_PIXEL && mask[ i * w + j] == 255 )
            {
                UnwrapPostponedPixel(solution, wrappedImg, bitMap, w, i, j);
            }
        }
    }
    
    /* Unwrap isolated region pixels */
    for(unsigned int i = 0; i < h; ++i)
    {
        for(unsigned int j = 0; j < w; ++j)
        {
            if( bitMap[ i * w + j] == FREE_PIXEL && mask[ i * w + j] == 255 )
            {
                UnwrapPostponedPixel(solution, wrappedImg, bitMap, w, i, j);
                UnwrapPixels(solution, wrappedImg, bitMap, mask, w, h, i, j);
            }
        }
    }
}