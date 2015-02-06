//
//  Break1Insert1.cpp
//  PhaseUnwrapping
//
//  Created by Ian Herszterg on 1/29/15.
//  Copyright (c) 2015 Ian Herszterg. All rights reserved.
//

#include "Break1Insert1.h"

double Break1Insert1( std::vector<Group>& currentSolution, int g1, int g2, double currentCost, int w, int h )
{
    if( currentSolution[g1].points.size() == 0 || currentSolution[g2].points.size() == 0 )
        return INFINITY;
    
    double newCurrentCost = currentCost;
    
    double neighborCost = currentCost - currentSolution[g1].cost - currentSolution[g2].cost;
    
    std::vector<Group> neighbor;
    CloneSolution(currentSolution, neighbor);
    
    double minDistance = INFINITY;
    
    for( int i = 0; i < currentSolution[g1].points.size(); i++ )
    {
        for( int j = 0; j < currentSolution[g2].points.size(); j++ )
        {
            double cost = EuclideanDistance(currentSolution[g1].points[i].i, currentSolution[g1].points[i].j, currentSolution[g2].points[j].i,
                                            currentSolution[g2].points[j].j);
            if( cost < minDistance )
                minDistance = cost;
        }
    }
    
    if( minDistance > MAX_DIST )
        return INFINITY;
    
    Group testGroup;
    testGroup.nPos = 0;
    testGroup.nNeg = 0;
    
    for( int i = 0; i < currentSolution[g1].points.size(); i++ )
    {
        currentSolution[g1].points[i].visited = false;
        testGroup.points.push_back(currentSolution[g1].points[i]);
        testGroup.border_points.push_back(currentSolution[g1].border_points[i]);
    }
    for( int i = 0; i < currentSolution[g2].points.size(); i++ )
    {
        currentSolution[g2].points[i].visited = false;
        testGroup.points.push_back(currentSolution[g2].points[i]);
        testGroup.border_points.push_back(currentSolution[g2].border_points[i]);
    }
    
    /* Computes the new spanning tree and removes the longer edge */
    MinimumSpanningTree * mst = new MinimumSpanningTree();
    mst->setInstance(testGroup.points, testGroup.border_points);
    mst->ComputeMST(testGroup.sol_edges);
    
    double maxDistance = 0;
    int edge_index = 0;
    
    for( int i = 0; i < testGroup.sol_edges.size(); i++ )
    {
        if( testGroup.sol_edges[i].cost > maxDistance )
        {
            maxDistance = testGroup.sol_edges[i].cost;
            edge_index = i;
        }
    }
    
    testGroup.sol_edges[ edge_index ].valid = false;
    FindDisconnectedPieces(testGroup.points, testGroup.sol_edges, false);
    
    neighbor[g1].points.clear();
    neighbor[g1].border_points.clear();
    neighbor[g1].sol_edges.clear();
    neighbor[g1].nPos = 0;
    neighbor[g1].nNeg = 0;
    
    neighbor[g2].points.clear();
    neighbor[g2].border_points.clear();
    neighbor[g2].sol_edges.clear();
    neighbor[g2].nPos = 0;
    neighbor[g2].nNeg = 0;
    
    for( unsigned int i = 0; i < testGroup.points.size(); i++ )
    {
        if( testGroup.points[i].group_n == 0 )
        {
            neighbor[g1].points.push_back(testGroup.points[i]);
            if(testGroup.points[i].type == POS_RESIDUE)
            {
                neighbor[g1].nPos++;
            }
            else
            {
                neighbor[g1].nNeg++;
            }
        }
        else
        {
            neighbor[g2].points.push_back(testGroup.points[i]);
            if(testGroup.points[i].type == POS_RESIDUE)
            {
                neighbor[g2].nPos++;
            }
            else
            {
                neighbor[g2].nNeg++;
            }
        }
    }
    
    CreateBorderPoints(neighbor[g1].points, neighbor[g1].border_points, w, h);
    CreateBorderPoints(neighbor[g2].points, neighbor[g2].border_points, w, h);
    
    mst->setInstance(neighbor[g1].points, neighbor[g1].border_points);
    neighbor[g1].cost = mst->ComputeMST(neighbor[g1].sol_edges);
    
    mst->setInstance(neighbor[g2].points, neighbor[g2].border_points);
    neighbor[g2].cost = mst->ComputeMST(neighbor[g2].sol_edges);
    
    neighborCost += neighbor[g1].cost + neighbor[g2].cost;
    
    if( neighborCost < currentCost )
    {
        newCurrentCost = neighborCost;
        ClonePartialSolution(neighbor, currentSolution, g1, g2);
        printf("new best Break1_Insert1-Neighbor = %lf\n", neighborCost);
    }
    
    neighbor.clear();
    testGroup.points.clear();
    testGroup.border_points.clear();
    testGroup.sol_edges.clear();
    
    delete mst;
    return newCurrentCost;
}