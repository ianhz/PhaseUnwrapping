//
//  Group.h
//  PhaseUnwrapping
//
//  Created by Ian Herszterg on 1/17/15.
//  Copyright (c) 2015 Ian Herszterg. All rights reserved.
//

#ifndef PhaseUnwrapping_Group_h
#define PhaseUnwrapping_Group_h

#include <vector>
#include "Point.h"
#include "Edge.h"

struct group_info
{
    int groupId;
    int nPos = 0;
    int nNeg = 0;
    bool border = false;
    std::vector<Point> points;
    std::vector<Edge> sol_edges;
    
    void setSolEdges( std::vector<Edge> st)
    {
        sol_edges.clear();
        for( unsigned int i = 0; i < st.size(); i++ )
            sol_edges.push_back(st[i]);
    }
};
typedef group_info Group;

#endif
