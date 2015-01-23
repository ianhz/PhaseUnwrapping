//
//  LocalSearch.cpp
//  PhaseUnwrapping
//
//  Created by Ian Herszterg on 1/17/15.
//  Copyright (c) 2015 Ian Herszterg. All rights reserved.
//

#include "LocalSearch.h"

#define OPT_NEIGHBORHOOD 1
#define MAX_TRIES 10
#define MAX_DIST 30
#define MAX_BORDER_DIST 50

struct Tuple
{
    int g1;
    int g2;
};

/* Clones the given solution */
void CloneSolution(std::vector<Group> s, std::vector<Group>& d)
{
    d.clear();
    
    for( int g = 0; g < s.size(); g++)
    {
        Group copiedGroup;
        
        for( unsigned int p = 0; p < s[g].points.size(); p++ )
            copiedGroup.points.push_back( s[g].points[p] );
        
        for( unsigned int p = 0; p < s[g].sol_edges.size(); p++ )
            copiedGroup.sol_edges.push_back( s[g].sol_edges[p] );
        
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

void LocalSearch::Swap( std::vector<Group>& current, int g1, int g2, int i )
{
    Point  p1 = current[g1].points[i];
    current[g2].points.push_back(p1);
    
    if( p1.type == POS_RESIDUE )
    {
        current[g1].nPos--;
        current[g2].nPos++;
    }
    else
    {
        current[g1].nNeg--;
        current[g2].nNeg++;
    }
    
    current[g1].points.erase( current[g1].points.begin() + i );
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
    
    /* Creates and sets the possible combinations array */
    std::vector<Tuple> combinations;
    
    for( unsigned int i = 0; i < k; i++ )
    {
        for( unsigned int j = 0; j < k; j++ )
        {
            if(i == j)
                continue;
            
            Tuple new_t;
            new_t.g1 = i;
            new_t.g2 = j;
            combinations.push_back(new_t);
        }
    }
    
    int nCombinations = (int)combinations.size();
    
    while(!wasImprooved)
    {
        /* Selects two random groups from the availiable combinations array*/
        int tupleInd = rand() % combinations.size();
        
        int g1 = combinations[ tupleInd ].g1;
        int g2 = combinations[ tupleInd ].g2;
        
        /* Removes the selected combination from the combinations array */
        combinations.erase(combinations.begin() + tupleInd);
        
        if(neighborSolution[g1].points.size() == 0 || neighborSolution[g2].points.size() == 0)
            continue;
        
        /* Tries to swap points from group g1 to group g2 */
        for( unsigned int i = 0; i < neighborSolution[g1].points.size(); i++ )
        {
            /* Doesn't pass border points */
            if( neighborSolution[g1].points[i].isBorder )
                continue;
            
            Swap( neighborSolution, g1, g2, i );
            double neighborCost = currentSolutionCost - gC[g1] - gC[g2];
            double n1, n2;
            
            _cF->setInstance(neighborSolution[g1].points);
            n1 = _cF->ComputeCost();
            _cF->setInstance(neighborSolution[g2].points);
            n2 = _cF->ComputeCost();
            
            neighborCost += n1 + n2;
            
            /* Assigns the better solution else UNDO */
            if(neighborCost < currentSolutionCost)
            {
                Swap( currentSolution, g1, g2, i );
                wasImprooved = true;
                currentSolutionCost = neighborCost;
                gC[g1] = n1;
                gC[g2] = n2;
            }
            else
            {
                Swap( neighborSolution, g2, g1, (int)neighborSolution[g2].points.size()-1 );
            }
            
        }
        
        /* Half of the combinations already computed */
        if( combinations.size() == nCombinations/2 )
            break;
    }
}
