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
        
        for( unsigned int p = 0; p < s[g].border_points.size(); p++ )
            copiedGroup.border_points.push_back( s[g].border_points[p] );
        
        for( unsigned int p = 0; p < s[g].sol_edges.size(); p++ )
            copiedGroup.sol_edges.push_back( s[g].sol_edges[p] );
        
        copiedGroup.nPos = s[g].nPos;
        copiedGroup.nNeg = s[g].nNeg;
        copiedGroup.border = s[g].border;
        copiedGroup.groupId = g;
        copiedGroup.cost = s[g].cost;
        
        d.push_back(copiedGroup);
    }
}

void LocalSearch::setCostFunction(CostFunction * cF)
{
    _cF = cF;
}


void LocalSearch::Run( std::vector<Group>& currentSolution, int k)
{
    double currentSolutionCost = 0.0;
    
    for( unsigned int i = 0; i < k; i++ )
    {
        currentSolutionCost += currentSolution[i].cost;
    }
    
    /* Clones the current solution */
    std::vector<Group> neighborSolution1, neighborSolution2;
    CloneSolution(currentSolution, neighborSolution1);
    CloneSolution(currentSolution, neighborSolution2);
    
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
    
    while( !wasImprooved && combinations.size() > 0 )
    {
        /* Selects two random groups from the availiable combinations array*/
        int tupleInd = rand() % combinations.size();
        
        int g1 = combinations[ tupleInd ].g1;
        int g2 = combinations[ tupleInd ].g2;
        
        /* Removes the selected combination from the combinations array */
        combinations.erase(combinations.begin() + tupleInd);
        
        /* Tries to swap points from group g1 to group g2 */
        double n1 = Relocate(neighborSolution1, g1, g2, currentSolutionCost);
        double n2 = Swap(neighborSolution2, g1, g2, currentSolutionCost);
        
        if( n1 <= n2 )
        {
            if(n1 < currentSolutionCost)
            {
                CloneSolution(neighborSolution1, currentSolution);
                currentSolutionCost = n1;
            }
        }
        else
        {
            if(n2 < currentSolutionCost)
            {
                CloneSolution(neighborSolution2, currentSolution);
                currentSolutionCost = n2;
            }
        }
    }
    
    neighborSolution1.clear();
    neighborSolution2.clear();
}
