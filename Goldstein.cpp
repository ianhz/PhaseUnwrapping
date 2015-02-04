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

struct coord
{
    int i;
    int j;
}; typedef struct coord Coord;

void Goldstein::RunMethod()
{
    /* Creates a backup to the original bitmap */
    unsigned char * g_bitmap = (unsigned char *)malloc( _h * _w * sizeof(unsigned char));
    memcpy(g_bitmap, _bitmap, _h * _w * sizeof(unsigned char));
    int current_mask_size;
    std::vector<Coord> active;
    
    /* Runs on all the pixels */
    for( unsigned int i = 0; i < _h; i++ )
    {
        for( unsigned int j = 0; j < _w; j++ )
        {
            int charge = 0;
            
            /* Let charge = 1 if the residue is positive, -1 if negative */
            if( g_bitmap[ i * _w + j ] == POS_RESIDUE )
                charge++;
            else if( g_bitmap[ i * _w + j ] == NEG_RESIDUE )
                charge--;
            else
                continue;
            
            int current_i = i;
            int current_j = j;
            
            /* Mark the residue active */
            if( g_bitmap[ current_i * _w + current_j ] == POS_RESIDUE )
                g_bitmap[ current_i * _w + current_j ] = ACTIVE_POS_RESIDUE;
            else if( g_bitmap[ current_i * _w + current_j ] == NEG_RESIDUE )
                g_bitmap[ current_i * _w + current_j ] = ACTIVE_NEG_RESIDUE;
            
            Coord current_active;
            current_active.i = current_i;
            current_active.j = current_j;
            active.push_back(current_active);
        
            
            /* Sets the current mask */
            current_mask_size = 1;
            
            /* For n = 3 to MaxBoxSize -- nxn mask */
            while( current_mask_size <= MAX_MASK_SIZE )
            {
                /* For each active residue */
                for( unsigned int a = 0; a < active.size(); a++ )
                {
                    int m_low_i, m_high_i, m_low_j, m_high_j;
                    current_i = active[a].i;
                    current_j = active[a].j;
                    
                    /* Centers the box at the active pixel */
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
                    
                    /* For each box pixel */
                    for( int m_i = m_low_i; m_i <= m_high_i; m_i++ )
                    {
                        for( int m_j = m_low_j; m_j <= m_high_j; m_j++ )
                        {
                            if( m_i == current_i && m_j == current_j )
                                continue;
                            
                            if( g_bitmap[ m_i * _w + m_j ] == ACTIVE_POS_RESIDUE || g_bitmap[ m_i * _w + m_j ] == ACTIVE_NEG_RESIDUE )
                                continue;
                            if( g_bitmap[ m_i * _w + m_j ] == ACTIVE_BALANCED_POS_RESIDUE || g_bitmap[ m_i * _w + m_j ] == ACTIVE_BALANCED_NEG_RESIDUE )
                                continue;
                            
                            /* If border found, connect the current residue to it */
                            if(m_low_i == 0 || m_high_i == _h-1 || m_low_j == 0 || m_high_j == _w-1)
                            {
                                int bi, bj;
                                if( g_bitmap[ current_i * _w + current_j ] == ACTIVE_POS_RESIDUE )
                                    g_bitmap[ current_i * _w + current_j ] = ACTIVE_BALANCED_POS_RESIDUE;
                                else if( g_bitmap[ current_i * _w + current_j ] == ACTIVE_NEG_RESIDUE )
                                    g_bitmap[ current_i * _w + current_j ] = ACTIVE_BALANCED_NEG_RESIDUE;
                                FindClosestBorderPoint(current_i, current_j, _w, _h, bi, bj);
                                Bresenham(_bitmap, current_i, current_j, bi, bj, _w);
                                charge = 0;
                                break;
                            }
                            /* if the box pixel is inactive  */
                            else if( g_bitmap[ m_i * _w + m_j ] != FREE_PIXEL)
                            {
                                /* If it is unbalanced */
                                if( g_bitmap[ m_i * _w + m_j ] == POS_RESIDUE )
                                {
                                    charge++;
                                    g_bitmap[ m_i * _w + m_j ] = ACTIVE_BALANCED_POS_RESIDUE;
                                    
                                }
                                else if( g_bitmap[ m_i * _w + m_j ] == NEG_RESIDUE )
                                {
                                    charge--;
                                    g_bitmap[ m_i * _w + m_j ] = ACTIVE_BALANCED_NEG_RESIDUE;

                                }
                                /* Inactive but balanced residues */
                                else if( g_bitmap[ m_i * _w + m_j ] == INACTIVE_BALANCED_POS_RESIDUE )
                                {
                                    g_bitmap[ m_i * _w + m_j ] = ACTIVE_BALANCED_POS_RESIDUE;

                                }
                                else if( g_bitmap[ m_i * _w + m_j ] == INACTIVE_BALANCED_NEG_RESIDUE )
                                {
                                    g_bitmap[ m_i * _w + m_j ] = ACTIVE_BALANCED_NEG_RESIDUE;
                                }
                                
                                Bresenham(_bitmap, current_i, current_j, m_i, m_j, _w);
                                
                                Coord current_active;
                                current_active.i = m_i;
                                current_active.j = m_j;
                                active.push_back(current_active);
                                
                                if( charge == 0 )
                                    break;
                            }
                        }
                        
                        if(charge == 0)
                            break;
                    }

                    if(charge == 0)
                        break;
                }
                
                if( charge != 0 )
                    current_mask_size *= 2;
                else
                {
                    for( unsigned int a = 0; a < active.size(); a++ )
                    {
                        if( g_bitmap[ active[a].i * _w + active[a].j ] == ACTIVE_BALANCED_POS_RESIDUE || g_bitmap[ active[a].i * _w + active[a].j ] == ACTIVE_POS_RESIDUE )
                            g_bitmap[ active[a].i * _w + active[a].j ] = INACTIVE_BALANCED_POS_RESIDUE;
                        else if( g_bitmap[ active[a].i * _w + active[a].j ] == ACTIVE_BALANCED_NEG_RESIDUE || g_bitmap[ active[a].i * _w + active[a].j ] == ACTIVE_NEG_RESIDUE)
                            g_bitmap[ active[a].i * _w + active[a].j ] = INACTIVE_BALANCED_NEG_RESIDUE;
                    }
                    
                    active.clear();
                    break;
                }
            }
            
            if( charge != 0 )
            {
                int bi, bj;
                FindClosestBorderPoint(current_i, current_j, _w, _h, bi, bj);
                Bresenham(_bitmap, current_i, current_j, bi, bj, _w);
                for( unsigned int a = 0; a < active.size(); a++ )
                {
                    if( g_bitmap[ active[a].i * _w + active[a].j ] == ACTIVE_BALANCED_POS_RESIDUE || g_bitmap[ active[a].i * _w + active[a].j ] == ACTIVE_POS_RESIDUE )
                        g_bitmap[ active[a].i * _w + active[a].j ] = INACTIVE_BALANCED_POS_RESIDUE;
                    else if( g_bitmap[ active[a].i * _w + active[a].j ] == ACTIVE_BALANCED_NEG_RESIDUE || g_bitmap[ active[a].i * _w + active[a].j ] == ACTIVE_NEG_RESIDUE)
                        g_bitmap[ active[a].i * _w + active[a].j ] = INACTIVE_BALANCED_NEG_RESIDUE;
                }
                
                active.clear();
                break;
            }
        }
    }
    
    free(g_bitmap);
}