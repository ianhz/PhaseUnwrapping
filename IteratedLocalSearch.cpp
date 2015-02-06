//
//  IteratedLocalSearch.cpp
//  PhaseUnwrapping
//
//  Created by Ian Herszterg on 1/17/15.
//  Copyright (c) 2015 Ian Herszterg. All rights reserved.
//

#include "IteratedLocalSearch.h"
#define MAX_ITERATIONS 100
#define SHAKE 3
#define NUM_POINTS_CHANGE 4
#define SHAKE_NUM 10
#define MAX_EDGE_COST 4 

void IteratedLocalSearch::Shake( std::vector<Group>& solution  )
{
    printf("Shake operation ---\n");
    CloneSolution(initialSolution, solution);
}

void IteratedLocalSearch::RunMethod()
{
    /* Generates a initial solution */
    GenerateInitialSolution();
    int notimprovedIterations = 0;
    
    std::vector<Group> currentBestSolution;
   
    /* Copies the current solution */
    CloneSolution(bestSolution, currentBestSolution);
    
    LocalSearch * lSearch = new LocalSearch();
    _cF = new MinimumSpanningTree();
    lSearch->setCostFunction(_cF);
    
    srand(time(NULL));
    
    printf("Initial best solution = %lf\n", bestSolutionCost);
    
    for( unsigned int i = 0; i < MAX_ITERATIONS; i++ )
    {
        lSearch->Run(currentBestSolution, _k, _w, _h);
        
        double currentBestSolutionCost = 0;
        
        for( unsigned int i = 0; i < _k; i++ )
            currentBestSolutionCost += currentBestSolution[i].cost;
        
        if( currentBestSolutionCost < bestSolutionCost )
        {
            printf("New BEST solution = %lf\n", currentBestSolutionCost);
            
            /* Copies the current solution */
            CloneSolution(currentBestSolution, bestSolution);
            bestSolutionCost = currentBestSolutionCost;
        }
        else
            notimprovedIterations++;
        
        /* If no improvement happened over SHAKE iterations, shake the solution -- Leaves local minimum */
        if( notimprovedIterations == SHAKE )
        {
            Shake(currentBestSolution);
            notimprovedIterations = 0;
            for( unsigned int i = 0; i < _k; i++ )
                currentBestSolutionCost += currentBestSolution[i].cost;
        }
    }
    
    printf("FINAL best solution = %lf\n", bestSolutionCost);
    
    /* Tries to create balanced sub-groups */
    for( unsigned int i = 0; i < _k; i++ )
    {
        _cF->setInstance( bestSolution[i].points, bestSolution[i].border_points );
        MinimumSpanningTree * mst = (MinimumSpanningTree *)_cF;
        mst->ComputeMST(bestSolution[i].sol_edges);
    }
    
    for(unsigned int i = 0; i < _k; i++)
    {
        for(unsigned int k = 0; k < bestSolution[i].points.size(); k++)
            bestSolution[i].points[k].visited = false;
        for(unsigned int k = 0; k < bestSolution[i].border_points.size(); k++)
            bestSolution[i].border_points[k].visited = false;
        
        for(unsigned int k = 0; k < bestSolution[i].sol_edges.size(); k++)
            bestSolution[i].sol_edges[k].visited = false;
        
        for( unsigned int j = 0; j < bestSolution[i].sol_edges.size(); j++ )
        {
            bestSolution[i].sol_edges[j].valid = false;
            
            int result = FindDisconnectedPieces(bestSolution[i].points, bestSolution[i].sol_edges, true);
            
            for(unsigned int k = 0; k < bestSolution[i].points.size(); k++)
                bestSolution[i].points[k].visited = false;
            for(unsigned int k = 0; k < bestSolution[i].border_points.size(); k++)
                bestSolution[i].border_points[k].visited = false;
            
            for(unsigned int k = 0; k < bestSolution[i].sol_edges.size(); k++)
                bestSolution[i].sol_edges[k].visited = false;
            
            if( result == -1 )
                bestSolution[i].sol_edges[j].valid = true;
        }
    }

    
    /* Draws the opt-branches */
    for( unsigned int i = 0; i < _k; i++ )
    {
        for( unsigned int e = 0; e < bestSolution[i].sol_edges.size(); e++ )
        {
            Edge * edge = &bestSolution[i].sol_edges[e];
            if(edge->valid)
                Bresenham(_bitmap, edge->p1->i, edge->p1->j, edge->p2->i, edge->p2->j, _w);
        }
    }
    
}

void IteratedLocalSearch::GenerateInitialSolution()
{
    /* Joins the list of residues */
    std::vector<Point> residues;
    for( unsigned int i = 0; i < _pos_residues.size(); i++ )
    {
        Point residue = _pos_residues[i];
        residues.push_back(residue);
    }
    for( unsigned int i = 0; i < _neg_residues.size(); i++ )
    {
        Point residue = _neg_residues[i];
        residues.push_back(residue);
    }
    
    /* Finds the MST considering all residues */
    std::vector<Edge> mst_edges;
    std::vector<Point> borders;
    MinimumSpanningTree * mst = new MinimumSpanningTree(residues, borders);
    mst->ComputeMST(mst_edges);
    
    /* Remove edges with cost greater than MAX_EDGE_COST */
    for( unsigned int i = 0; i < mst_edges.size(); i++ )
    {
        if( mst_edges[i].cost > MAX_EDGE_COST )
            mst_edges[i].valid = false;
    }
    
    /* Find the number of groups formed */
    _k = FindDisconnectedPieces(residues, mst_edges, false) + 1;
    
    delete mst;
    mst_edges.clear();
    
    /* Creates the k groups and assigns an initial solution */
    for( unsigned int i = 0; i < _k; i++ )
    {
        Group newGroup;
        initialSolution.push_back(newGroup);
    }
    
    for( unsigned int i = 0; i < residues.size(); ++i )
    {
        int groupId = residues[i].group_n;
        initialSolution[ groupId ].points.push_back(residues[i]);
        
        if(residues[i].type == POS_RESIDUE)
            initialSolution[ groupId ].nPos++;
        else
            initialSolution[ groupId ].nNeg++;
    }
    
    
    std::vector<Point> pos_borders, neg_borders;
    
    /* Creates the closest border vertices for each residue */
    for( unsigned int i = 0; i < _k; i++ )
    {
        CreateBorderPoints(initialSolution[i].points, initialSolution[i].border_points, _w, _h);
    }
    
    /*----- The initial solution is generated, with balanced groups ----- */
    _cF = new MinimumSpanningTree();
    
    for( unsigned int i = 0; i < _k; i++ )
    {
        MinimumSpanningTree * mst = (MinimumSpanningTree *)_cF;
        mst->setInstance(initialSolution[i].points, initialSolution[i].border_points);
        initialSolution[i].cost = mst->ComputeMST(initialSolution[i].sol_edges);
        initialSolutionCost += initialSolution[i].cost;
    }
    
    CloneSolution(initialSolution, bestSolution);
    bestSolutionCost = initialSolutionCost;
    
    MinimumSpanningTree * mst_cf = (MinimumSpanningTree *)_cF;
    delete mst_cf;
}
