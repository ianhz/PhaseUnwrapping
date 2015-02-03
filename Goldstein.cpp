//
//  Goldstein.cpp
//  PhaseUnwrapping
//
//  Created by Ian Herszterg on 2/3/15.
//  Copyright (c) 2015 Ian Herszterg. All rights reserved.
//

#include "Goldstein.h"

bool sortFunction_x( Point p1, Point p2 )
{
    return (p1.j < p2.j);
}
bool sortFunction_y( Point p1, Point p2 )
{
    return (p1.i < p2.i);
}

void Goldstein::RunMethod()
{
    /* Creates a backup to the original bitmap */
    unsigned char * g_bitmap = (unsigned char *)malloc( _h * _w * sizeof(unsigned char));
    memcpy(g_bitmap, _bitmap, _h * _w * sizeof(unsigned char));
    int current_mask_size = MASK_SIZE;
    
    /* Runs on all the pixels */
    for( unsigned int i = 0; i < _h; i++ )
    {
        for( unsigned int j = 0; j < _w; j++ )
        {
            if( g_bitmap[i * _w + j] == FREE_PIXEL )
                continue;
            
            int charge = 0;
            
            /* Sets the current charge */
            if( g_bitmap[i * _w + j] == POS_RESIDUE )
                charge++;
            else if( g_bitmap[i * _w + j] == NEG_RESIDUE )
                charge--;
            
            int current_i = i;
            int current_j = j;
            
            while( charge != 0)
            {
                /* Sets the current mask */
                int m_low_i, m_high_i, m_low_j, m_high_j;
                
                m_low_i = current_i - current_mask_size;
                m_high_i = current_i + current_mask_size;
                m_low_j = current_j - current_mask_size;
                m_high_j = current_j + current_mask_size;
                
                if(m_low_i < 0)
                    m_low_i = 0;
                if(m_low_j < 0)
                    m_low_j = 0;
                if(m_high_i > _h-1)
                    m_high_i = _h-1;
                if(m_high_j > _w-1)
                    m_high_j = _w-1;
                
                bool found = false;
                
                for( int m_i = m_low_i; m_i <= m_high_i; m_i++ )
                {
                    for( int m_j = m_low_j; m_j <= m_high_j; m_j++ )
                    {
                        if(m_i == current_i && m_j == current_j)
                            continue;
                        
                        if(g_bitmap[m_i * _w + m_j] != FREE_PIXEL)
                        {
                            if( g_bitmap[m_i * _w + m_j] == POS_RESIDUE )
                                charge++;
                            else if( g_bitmap[m_i * _w + m_j] == NEG_RESIDUE )
                                charge--;
                            
                            Bresenham(_bitmap, current_i, current_j, m_i, m_j, _w);
                            g_bitmap[m_i * _w + m_j] = FREE_PIXEL;
                            g_bitmap[current_i * _w + current_j] = FREE_PIXEL;
                            found = true;
                            
                            /* Sets the new active residue and resets the mask size*/
                            current_i = m_i;
                            current_j = m_j;
                            current_mask_size = MASK_SIZE;
                            break;
                        }
                    }
                    
                    /* Found residue inside mask, else double the mask size */
                    if(found)
                        break;
                }
                
                if(!found)
                {
                    /* If border found, connect the current residue to it */
                    if(m_low_i == 0 || m_high_i == _h-1 || m_low_j == 0 || m_high_j == _w-1)
                    {
                        int bi, bj;
                        FindClosestBorderPoint(current_i, current_j, _w, _h, bi, bj);
                        Bresenham(_bitmap, current_i, current_j, bi, bj, _w);
                        g_bitmap[current_i * _w + current_j] = FREE_PIXEL;
                        charge = 0;
                        found = true;
                    }
                    else
                        current_mask_size *= 2;
                }
            }
        }
    }
}