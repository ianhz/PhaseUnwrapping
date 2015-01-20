//
//  LocalSearch.cpp
//  PhaseUnwrapping
//
//  Created by Ian Herszterg on 1/17/15.
//  Copyright (c) 2015 Ian Herszterg. All rights reserved.
//

#include "LocalSearch.h"

#define OPT_NEIGHBORHOOD 2
#define MAX_TRIES 10
#define MAX_DIST 30

/* Clones the given solution */
void CloneSolution(std::vector<Group> s, std::vector<Group>& d)
{
    d.clear();
    
    for( int g = 0; g < s.size(); g++)
    {
        Group copiedGroup;
        
        for( unsigned int p = 0; p < s[g].points.size(); p++ )
            copiedGroup.points.push_back( s[g].points[p] );
        
        copiedGroup.nPos = s[g].nPos;
        copiedGroup.nNeg = s[g].nNeg;
        copiedGroup.border = s[g].border;
        copiedGroup.groupId = g;
        
        d.push_back(copiedGroup);
    }
}

void LocalSearch::setCostFunction(CostFunction * cF)
{
    _cF = cF;
}

void LocalSearch::Swap( std::vector<Group>& current, int g1, int g2, int i, int j )
{
    Point  p1 = current[g1].points[i];
    Point  p2 = current[g2].points[j];
    
    current[g1].points.push_back(p2);
    current[g2].points.push_back(p1);
    
    current[g1].points.erase( current[g1].points.begin() + i );
    current[g2].points.erase( current[g2].points.begin() + j );
}


void LocalSearch::Run( std::vector<Group>& currentSolution, int k, double * gC )
{
    double currentSolutionCost = 0.0;
    
    for( unsigned int i = 0; i < k; i++ )
    {
        currentSolutionCost += gC[i];
    }
    
    /* Clones the current solution */
    std::vector<Group> neighborSolution;
    CloneSolution(currentSolution, neighborSolution);
    
    bool wasImprooved = false;
    srand(time(NULL));
    
    int nTries = 0;
    
    while(!wasImprooved)
    {
        /* While there was an improovement */
        for( unsigned int n = 0; n < OPT_NEIGHBORHOOD; n++)
        {
            /* Selects two random groups */
            int g1 = rand() % k;
            int g2 = rand() % k;
            
            while( g2 == g1 )
                g2 = rand() % k;
            
            if(neighborSolution[g1].points.size() == 0 || neighborSolution[g2].points.size() == 0)
                continue;
            
            /* Tries to swap points from group g1 to group g2 */
            for( unsigned int i = 0; i < neighborSolution[g1].points.size(); i++ )
            {
                for( unsigned int j = 0; j < neighborSolution[g2].points.size(); j++ )
                {
                    /* Only swaps ++ or -- residues */
                    if( neighborSolution[g2].points[j].type != neighborSolution[g1].points[i].type )
                        continue;
                    
                    /* Don't consider points whose distance is greater than MAX_DIST */
                    if( EuclideanDistance(neighborSolution[g2].points[j].i, neighborSolution[g2].points[j].j, neighborSolution[g1].points[i].i, neighborSolution[g1].points[i].j) > MAX_DIST )
                        continue;
                    
                    Swap( neighborSolution, g1, g2, i, j );
                    double neighborCost = 0.0;
                    double n1, n2;
                    
                    neighborCost = currentSolutionCost - gC[g1] - gC[g2];
                    
                    if( g1 != k-1)
                    {
                        _cF->setInstance(neighborSolution[g1].points);
                        n1 = _cF->ComputeCost();
                    }
                    else
                        n1 = gC[g1];
                    
                    if( g2 != k-1)
                    {
                        _cF->setInstance(neighborSolution[g2].points);
                        n2 = _cF->ComputeCost();
                    }
                    else
                        n2 = gC[g2];

                    neighborCost += n1 + n2;
                    
                    /* Assigns the better solution else UNDO */
                    if(neighborCost < currentSolutionCost)
                    {
                        Swap( currentSolution, g1, g2, i, j );
                        wasImprooved = true;
                        currentSolutionCost = neighborCost;
                        gC[g1] = n1;
                        gC[g2] = n2;
                    }
                    else
                    {
                        Swap( neighborSolution, g1, g2, (int)neighborSolution[g1].points.size()-1, (int)neighborSolution[g2].points.size()-1 );
                    }
                }
            }
            
            if(!wasImprooved)
                nTries++;
        }
        
        if( nTries > MAX_TRIES )
            break;
    }
}
