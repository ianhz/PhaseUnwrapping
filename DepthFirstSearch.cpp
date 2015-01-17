//
//  DepthFirstSearch.cpp
//  PhaseUnwrapping
//
//  Created by Ian Herszterg on 1/17/15.
//  Copyright (c) 2015 Ian Herszterg. All rights reserved.
//

#include "DepthFirstSearch.h"
#include "Residue.h"

void DepthFirstSearch(std::vector<Point *>& points, std::vector<Edge>& edges, Point * start, int groupId, int& nPos, int& nNeg)
{
    start->group_n = groupId;
    start->visited = true;
    
    if(start->type == POS_RESIDUE)
        nPos++;
    else
        nNeg++;
    
    for( unsigned int i = 0; i < edges.size(); i++ )
    {
        if(!edges[i].valid)
            continue;
        
        Point * origin, * destination;
        
        if( edges[i].p1 == start || edges[i].p2 == start)
        {
            if( edges[i].p1 == start)
            {
                origin = edges[i].p1;
                destination = edges[i].p2;
            }
            else
            {
                destination = edges[i].p1;
                origin = edges[i].p2;
            }
            
            if(!destination->visited && !edges[i].visited)
            {
                edges[i].visited = true;
                DepthFirstSearch(points, edges, destination, groupId, nPos, nNeg);
            }
        }
    }
}

int FindDisconnectedPieces( std::vector<Point *>& points, std::vector<Edge>& edges, bool disconect )
{
    int groups = -1;
    
    for( unsigned int i = 0; i < points.size(); i++ )
    {
        if( !points[i]->visited )
        {
            groups++;
            
            int nPos = 0, nNeg = 0;
            DepthFirstSearch( points, edges, points[i], groups, nPos, nNeg);
            
            if(disconect && nPos != nNeg)
                return -1;
        }
    }
    
    return groups;
}
