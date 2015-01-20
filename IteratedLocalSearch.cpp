//
//  IteratedLocalSearch.cpp
//  PhaseUnwrapping
//
//  Created by Ian Herszterg on 1/17/15.
//  Copyright (c) 2015 Ian Herszterg. All rights reserved.
//

#include "IteratedLocalSearch.h"
#define MAX_ITERATIONS 30000
#define SHAKE 1
#define NUM_POINTS_CHANGE 2
#define MAX_DIST 10
#define SHAKE_NUM 5

void IteratedLocalSearch::Shake( std::vector<Group>& solution  )
{
    LocalSearch ls;
    
    for( unsigned int i = 0; i < SHAKE_NUM; i++ )
    {
        /* Selects two random groups */
        int g1 = rand() % _k;
        int g2 = rand() % _k;
        
        while( g2 == g1 )
            g2 = rand() % _k;
        
        for( unsigned int j = 0; j < NUM_POINTS_CHANGE; j++ )
        {
            /* Select two random points */
            int i1 = rand() % solution[g1].points.size();
            int i2 = rand() % solution[g2].points.size();
            
            Point p1 = solution[g1].points[i1];
            Point p2 = solution[g2].points[i2];
            
            if( EuclideanDistance( p1.i, p1.j, p2.i, p2.j) > MAX_DIST || (p1.type != p2.type) )
                continue;
            
            /* Swaps */
            ls.Swap(solution, g1, g2, i1, i2);
        }
    }
    
    for( unsigned int i = 0; i < _k; i++ )
    {
        _cF->setInstance(solution[i].points);
        currentGroupCosts[i] = _cF->ComputeCost();
    }
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
    
    currentGroupCosts = (double *)malloc( _k * sizeof(double));
    memcpy(currentGroupCosts, bestSolutionGroupCosts, _k*sizeof(double));
    
    srand(time(NULL));
    
    printf("Current best solution = %lf\n", bestSolutionCost);
    
    for( unsigned int i = 0; i < MAX_ITERATIONS; i++ )
    {
        lSearch->Run(currentBestSolution, _k, currentGroupCosts);
        
        double currentBestSolutionCost = 0;
        
        for( unsigned int i = 0; i < _k; i++ )
            currentBestSolutionCost += currentGroupCosts[i];
        
        if( currentBestSolutionCost < bestSolutionCost )
        {
            printf("New best solution = %lf\n", currentBestSolutionCost);
            
            /* Copies the current solution */
            CloneSolution(currentBestSolution, bestSolution);
            
            memcpy(bestSolutionGroupCosts, currentGroupCosts, _k*sizeof(double));
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
                currentBestSolutionCost += currentGroupCosts[i];
        }
    }
    
    /* Tries to create balanced sub-groups */
    for( unsigned int i = 0; i < _k-1; i++ )
    {
        _cF->setInstance(bestSolution[i].points);
        MinimumSpanningTree * mst = (MinimumSpanningTree *)_cF;
        mst->ComputeMST(bestSolution[i].sol_edges);
        
        for( unsigned int v = 0; v < bestSolution[i].points.size(); v++ )
        {
            bestSolution[i].points[v].visited = false;
        }
        
        for( unsigned int e = 0; e < bestSolution[i].sol_edges.size(); e++ )
        {
            Edge * e1 = &bestSolution[i].sol_edges[e];
            e1->valid = false;
            
            /* After disconnecting the group, if there is a unbalanced subgroups then reconnects the removed edge */
            if(FindDisconnectedPieces(bestSolution[i].points, bestSolution[i].sol_edges, true) == -1)
                e1->valid = true;
        }
    }
    
    /* Draws the opt-branches */
    for( unsigned int i = 0; i < _k-1; i++ )
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
    MinimumSpanningTree * mst = new MinimumSpanningTree(residues);
    mst->ComputeMST(mst_edges);
    
    /* Remove edges with cost greater than MAX_EDGE_COST */
    for( unsigned int i = 0; i < mst_edges.size(); i++ )
    {
        if( mst_edges[i].cost > MAX_EDGE_COST )
            mst_edges[i].valid = false;
    }
    
    /* Find the number of groups formed */
    _k = FindDisconnectedPieces(residues, mst_edges, false) + 2;
    
    delete mst;
    mst_edges.clear();
    
    /* Creates the k groups and assigns an initial solution */
    for( unsigned int i = 0; i < _k; i++ )
    {
        Group newGroup;
        bestSolution.push_back(newGroup);
    }
    
    for( unsigned int i = 0; i < residues.size(); ++i )
    {
        int groupId = residues[i].group_n;
        bestSolution[ groupId ].points.push_back(residues[i]);
        
        if(residues[i].type == POS_RESIDUE)
            bestSolution[ groupId ].nPos++;
        else
            bestSolution[ groupId ].nNeg++;
    }
    
    
    std::vector<Point> pos_borders, neg_borders;
    
    /* Creates the closest border vertices for each residue */
    CreateBorderPoints(_pos_residues, neg_borders, _w, _h);
    CreateBorderPoints(_neg_residues, pos_borders, _w, _h);
    
    /* Creates a group only with borders */
    for( unsigned int i = 0; i < pos_borders.size(); ++i )
    {
        bestSolution[_k-1].points.push_back(pos_borders[i]);
    }
    for( unsigned int i = 0; i < neg_borders.size(); ++i )
    {
        bestSolution[_k-1].points.push_back(neg_borders[i]);
    }
    
    int posBorder = 0, negBorder = 0;
    
    /* Assigns the closest border points to balance unbalanced groups */
    for( unsigned int i = 0; i < _k - 1; ++i )
    {
        if( bestSolution[i].nNeg == bestSolution[i].nPos )
            continue;
        
        double minDistance = INFINITY;
        int border_i = -1;
        
        for( unsigned int j = 0; j < bestSolution[i].points.size(); j++ )
        {
            for( unsigned int k = 0; k < bestSolution[_k-1].points.size(); k++ )
            {
                double currentDist = EuclideanDistance(bestSolution[i].points[j].i, bestSolution[i].points[j].j, bestSolution[_k-1].points[k].i, bestSolution[_k-1].points[k].j);
                if( currentDist < minDistance )
                {
                    minDistance = currentDist;
                    border_i = k;
                }
            }
        }
        
        /* Inserts border point into the unbalanced group and removes it from the borders group */
        bestSolution[i].points.push_back(bestSolution[_k-1].points[border_i]);
        bestSolution[_k-1].points.erase(bestSolution[_k-1].points.begin() + border_i);
    }
    
    /* Assigns additional border points to balance nPos/nNeg -- This won't affect the final solution */
    for( unsigned int i = 0; i < _k - 1; ++i )
    {
        if( bestSolution[i].nNeg == bestSolution[i].nPos )
            continue;
    
        while( bestSolution[i].nPos < bestSolution[i].nNeg )
        {
            int b = 0;
            
            if( bestSolution[_k-1].points[b].type == POS_RESIDUE )
            {
                bestSolution[i].points.push_back(bestSolution[_k-1].points[b]);
                bestSolution[_k-1].points.erase(bestSolution[_k-1].points.begin() + b);
            }
            
            bestSolution[i].nPos++;
            b++;
        }
        while( bestSolution[i].nPos < bestSolution[i].nNeg )
        {
            int b = 0;
            
            if( bestSolution[_k-1].points[b].type == NEG_RESIDUE )
            {
                bestSolution[i].points.push_back(bestSolution[_k-1].points[b]);
                bestSolution[_k-1].points.erase(bestSolution[_k-1].points.begin() + b);
            }
            
            bestSolution[i].nNeg++;
            b++;
        }
    }
    
    /*----- The initial solution is generated, with balanced groups ----- */
    bestSolutionGroupCosts = (double *)malloc( _k * sizeof(double));
    _cF = new MinimumSpanningTree();
    
    for( unsigned int i = 0; i < _k; i++ )
    {
        MinimumSpanningTree * mst = (MinimumSpanningTree *)_cF;
        mst->setInstance(bestSolution[i].points);
        bestSolutionGroupCosts[i] = mst->ComputeMST(bestSolution[i].sol_edges);
        bestSolutionCost += bestSolutionGroupCosts[i];
    }
    
    MinimumSpanningTree * mst_cf = (MinimumSpanningTree *)_cF;
    delete mst_cf;
}