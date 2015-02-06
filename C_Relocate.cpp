//
//  C_Relocate.cpp
//  PhaseUnwrapping
//
//  Created by Ian Herszterg on 1/29/15.
//  Copyright (c) 2015 Ian Herszterg. All rights reserved.
//

#include "C_Relocate.h"

//
//  Relocate.cpp
//  PhaseUnwrapping
//
//  Created by Ian Herszterg on 1/25/15.
//  Copyright (c) 2015 Ian Herszterg. All rights reserved.
//

#include "Relocate.h"

void C_RSwap( std::vector<Group>& current, int g1, int g2, int i1, int i2 )
{
    Point  p1 = current[g1].points[i1];
    Point  b1 = current[g1].border_points[i1];
    
    Point  p2 = current[g1].points[i2];
    Point  b2 = current[g1].border_points[i2];
    
    current[g2].points.push_back(p1);
    current[g2].border_points.push_back(b1);
    current[g2].points.push_back(p2);
    current[g2].border_points.push_back(b2);
    
    current[g1].nPos--;
    current[g2].nPos++;
    current[g1].nNeg--;
    current[g2].nNeg++;

    current[g1].points.erase( current[g1].points.begin() + i1 );
    current[g1].border_points.erase( current[g1].border_points.begin() + i1 );
    current[g1].points.erase( current[g1].points.begin() + i2 - 1 );
    current[g1].border_points.erase( current[g1].border_points.begin() + i2 - 1 );
}

double C_Relocate( std::vector<Group>& currentSolution, int g1, int g2, double currentCost )
{
    if( currentSolution[g1].points.size() == 0 || currentSolution[g2].points.size() == 0 )
        return INFINITY;
    
    double newCurrentCost = currentCost;
    
    double neighborCost = currentCost - currentSolution[g1].cost - currentSolution[g2].cost;
    
    std::vector<Group> neighbor;
    CloneSolution(currentSolution, neighbor);
    CostFunction * _cF = new MinimumSpanningTree();
    
    /* Tries to relocate pairs +- from group g1 to group g2 */
    for( unsigned int i = 0; i < currentSolution[g1].points.size(); i++ )
    {
        int p1_type = currentSolution[g1].points[i].type;
        
        for( unsigned int j = 0; j < currentSolution[g1].points.size(); j++ )
        {
           int p2_type = currentSolution[g1].points[j].type;
            
            if( p1_type == p2_type )
                continue;
            
            double minDistance_p1 = INFINITY;
            double minDistance_p2 = INFINITY;
            
            for( unsigned int k = 0; k < currentSolution[g2].points.size(); k++ )
            {
                double currentDist1 = EuclideanDistance(currentSolution[g1].points[i].i, currentSolution[g1].points[i].j, currentSolution[g2].points[k].i,
                                                        currentSolution[g2].points[k].j);
                double currentDist2 = EuclideanDistance(currentSolution[g1].points[j].i, currentSolution[g1].points[j].j, currentSolution[g2].points[k].i,
                                                        currentSolution[g2].points[k].j);
                if(  currentDist1 < minDistance_p1 )
                    minDistance_p1 = currentDist1;
                if( currentDist2 < minDistance_p2 )
                    minDistance_p2 = currentDist2;
            }
            
            if( minDistance_p1 < MAX_DIST && minDistance_p2 < MAX_DIST )
            {
                C_RSwap(neighbor, g1, g2, i, j);
                double n1, n2;
                
                _cF->setInstance(neighbor[g1].points, neighbor[g1].border_points );
                n1 = _cF->ComputeCost();
                _cF->setInstance(neighbor[g2].points, neighbor[g2].border_points );
                n2 = _cF->ComputeCost();
                
                
                neighborCost += n1 + n2;
                
                /* Assigns the better solution else UNDO */
                if(neighborCost < currentCost)
                {
                    C_RSwap( currentSolution, g1, g2, i, j );
                    newCurrentCost = neighborCost;
                    currentSolution[g1].cost = n1;
                    currentSolution[g2].cost = n2;
                    neighbor[g1].cost = n1;
                    neighbor[g2].cost = n2;
                    printf("new best C_Relocate-Neighbor = %lf\n", neighborCost);
                }
                else
                {
                    C_RSwap( neighbor, g2, g1, (int)neighbor[g2].points.size()-2, (int)neighbor[g2].points.size()-1 );
                    neighbor[g1].cost = currentSolution[g1].cost;
                    neighbor[g2].cost = currentSolution[g2].cost;
                }
            }
            
        }
    }
    
    MinimumSpanningTree * mst_cf = (MinimumSpanningTree *)_cF;
    delete mst_cf;
    neighbor.clear();
    
    return newCurrentCost;
}