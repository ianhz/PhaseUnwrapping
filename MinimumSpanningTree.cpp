//
//  MinimumSpanningTree.cpp
//  PhaseUnwrapping
//
//  Created by Ian Herszterg on 1/17/15.
//  Copyright (c) 2015 Ian Herszterg. All rights reserved.
//

#include "MinimumSpanningTree.h"

bool sortFunction( Edge e1, Edge e2 )
{
    return (e1.cost < e2.cost);
}

MinimumSpanningTree::MinimumSpanningTree( std::vector<Point *> vertices )
{
    this->vertices = vertices;
}

void MinimumSpanningTree::CreateEdges()
{
    /* Complete Graph - n^2 edges */
    for( unsigned int i = 0; i < vertices.size(); i++ )
    {
        for( unsigned int j = 1+1; j < vertices.size(); j++ )
        {
            Edge e;
            e.p1 = vertices[i];
            e.p2 = vertices[j];
            e.cost = EuclideanDistance(e.p1->i, e.p1->j, e.p2->i, e.p2->j);
            edges.push_back(e);
        }
    }
}

/* ----- Kruskall's Algorithm for Minimum Spanning Tree ----- */
double MinimumSpanningTree::ComputeMST()
{
    double cost = 0.0;
    
    /* 1. Sorts the edge list */
    std::sort(edges.begin(), edges.end(), sortFunction);
    
    /* 2. Computes the MST  */
    for(unsigned int i = 0; i < edges.size(); i++)
    {
        int g1, g2;
        g1 = edges[i].p1->group_n;
        g2 = edges[i].p2->group_n;
        
        if( g1 != g2 )
        {
            spanningTreeEdges.push_back(&edges[i]);
            cost += edges[i].cost;
            
            for(unsigned int j = 0; j < vertices.size(); j++)
            {
                if(vertices[j]->group_n == g1)
                    vertices[j]->group_n = g2;
            }
        }
    }

    return cost;
}
/* --- END --- */

double MinimumSpanningTree::ComputeCost()
{
    double cost = 0.0;
    
    for( unsigned int i = 0; i < spanningTreeEdges.size(); i++ )
        cost+= spanningTreeEdges[i]->cost;
    
    return cost;
}
