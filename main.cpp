//
//  main.cpp
//  PhaseUnwrapping
//
//  Created by Ian Herszterg on 1/17/15.
//  Copyright (c) 2015 Ian Herszterg. All rights reserved.
//

#include <stdio.h>
#include "Point.h"
#include "Util.h"
#include "Heuristic.h"
#include "Residue.h"
#include "MinimumSpanningTree.h"
#include "DepthFirstSearch.h"
#include "IteratedLocalSearch.h"

int main(int argc, const char * argv[])
{
    if(argc < 3)
    {
        printf("Too few arguments...Exiting!\n\n");
        return 0;
    }
    
    std::string folderPath = "/Users/ianhz/Documents/PUC/Mestrado/Dissertacao/Resultados/Actual/";
    const char * fileName = argv[1];
    std::string path;
    
    int h = atoi(argv[2]);
    int w = atoi(argv[3]);
    float * buffer = loadCSV(folderPath + fileName + ".csv", h, w);
    
    /* Sets up the instance variables */
    std::vector<Point> pos_residues, neg_residues;
    unsigned char * bitmap = (unsigned char *)malloc( h * w * sizeof(char));
    SearchForResidues(buffer, bitmap, pos_residues, neg_residues, w, h);
    
    Heuristic * method = new IteratedLocalSearch();
    method->SetInputs(bitmap, pos_residues, neg_residues, w, h);
    method->RunMethod();
    
    SaveBitmap(bitmap, w, h, folderPath+fileName);
    
    
}