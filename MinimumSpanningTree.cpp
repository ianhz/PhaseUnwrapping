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

MinimumSpanningTree::MinimumSpanningTree( std::vector<Point>& vertices )
{
    setInstance(vertices);
}

void MinimumSpanningTree::CreateEdges()
{
    /* Complete Graph - n^2 edges */
    for( unsigned int i = 0; i < vertices.size(); i++ )
    {
        for( unsigned int j = 0; j < vertices.size(); j++ )
        {
            if( i == j )
                continue;
            
            Edge e;
            e.p1 = vertices[i];
            e.p2 = vertices[j];
            e.cost = EuclideanDistance(e.p1->i, e.p1->j, e.p2->i, e.p2->j);
            edges.push_back(e);
        }
    }
}

/* ----- Kruskall's Algorithm for Minimum Spanning Tree ----- */
double MinimumSpanningTree::ComputeMST( std::vector<Edge>& spanningTreeEdges )
{
    double cost = 0.0;
    spanningTreeEdges.clear();
    edges.clear();
    
    /* Creates all edges */
    CreateEdges();
    
    /* 1. Sorts the edge list */
    std::sort(edges.begin(), edges.end(), sortFunction);
    
    int nBorder = 0;
    
    /* 2. Computes the MST  */
    for(unsigned int i = 0; i < edges.size(); i++)
    {
        if(edges[i].p1->isBorder || edges[i].p2->isBorder)
            nBorder++;
     
        /* Only considers closest border point */
        if(nBorder > 1)
            continue;
        
        int g1, g2;
        g1 = edges[i].p1->group_n;
        g2 = edges[i].p2->group_n;
        
        if( g1 != g2 )
        {
            spanningTreeEdges.push_back(edges[i]);
            cost += edges[i].cost;
            
            for(unsigned int j = 0; j < vertices.size(); j++)
            {
                if(vertices[j]->group_n == g1)
                    vertices[j]->group_n = g2;
            }
        }
        
        if( spanningTreeEdges.size() == vertices.size() - 1)
            break;
    }

    edges.clear();
    return cost;
}
/* --- END --- */

double MinimumSpanningTree::ComputeCost()
{
    std::vector<Edge> sTe;
    return ComputeMST( sTe );
}

void MinimumSpanningTree::setInstance(std::vector<Point>& v)
{
    vertices.clear();
    
    for( unsigned int i = 0; i < v.size(); i++ )
    {
        vertices.push_back(&v[i]);
        vertices[i]->group_n = i;
    }
}