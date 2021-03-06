//
//  Swap.cpp
//  PhaseUnwrapping
//
//  Created by Ian Herszterg on 1/25/15.
//  Copyright (c) 2015 Ian Herszterg. All rights reserved.
//

#include "Swap.h"

void SSwap( std::vector<Group>& current, int g1, int g2, int i, int j )
{
    Point  p1 = current[g1].points[i];
    Point  b1 = current[g1].border_points[i];
    
    Point  p2 = current[g2].points[j];
    Point  b2 = current[g2].border_points[j];
    
    current[g1].points.push_back(p2);
    current[g1].border_points.push_back(b2);
    current[g2].points.push_back(p1);
    current[g2].border_points.push_back(b1);
    
    current[g1].points.erase( current[g1].points.begin() + i );
    current[g1].border_points.erase( current[g1].border_points.begin() + i );
    current[g2].points.erase( current[g2].points.begin() + j );
    current[g2].border_points.erase( current[g2].border_points.begin() + j );
}

double Swap( std::vector<Group>& currentSolution, int g1, int g2, double currentCost )
{
    if( currentSolution[g1].points.size() == 0 || currentSolution[g2].points.size() == 0 )
        return INFINITY;
    
    double newCurrentCost = currentCost;
    
    double neighborCost = currentCost - currentSolution[g1].cost - currentSolution[g2].cost;
    
    std::vector<Group> neighbor;
    CloneSolution(currentSolution, neighbor);
    CostFunction * _cF = new MinimumSpanningTree();
    
    /* Tries to swap points from group g1 to group g2 */
    for( unsigned int i = 0; i < currentSolution[g1].points.size(); i++ )
    {
        for( unsigned int j = 0; j < currentSolution[g2].points.size(); j++ )
        {
            Point * p1 = &currentSolution[g1].points[i];
            Point * p2 = &currentSolution[g2].points[j];
            
            if( p1->type != p2->type )
                continue;
            
            if( EuclideanDistance(p1->i, p1->j, p2->i, p2->j) > MAX_DIST )
                continue;
            
            SSwap( neighbor, g1, g2, i, j );
            double n1, n2;
            
            _cF->setInstance(neighbor[g1].points, neighbor[g1].border_points );
            n1 = _cF->ComputeCost();
            _cF->setInstance(neighbor[g2].points, neighbor[g2].border_points );
            n2 = _cF->ComputeCost();
            
            
            neighborCost += n1 + n2;
            
            /* Assigns the better solution else UNDO */
            if(neighborCost < currentCost)
            {
                SSwap( currentSolution, g1, g2, i, j );
                newCurrentCost = neighborCost;
                currentSolution[g1].cost = n1;
                currentSolution[g2].cost = n2;
                neighbor[g1].cost = n1;
                neighbor[g2].cost = n2;
                printf("new best Swap-Neighbor = %lf\n", neighborCost);
            }
            else
            {
                SSwap( neighbor, g2, g1, (int)neighbor[g2].points.size()-1, (int)neighbor[g1].points.size()-1 );
                neighbor[g1].cost = currentSolution[g1].cost;
                neighbor[g2].cost = currentSolution[g2].cost;
            }
        }
    }
    
    MinimumSpanningTree * mst_cf = (MinimumSpanningTree *)_cF;
    delete mst_cf;
    neighbor.clear();
    
    return newCurrentCost;
}