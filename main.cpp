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
#include "Unwrap.h"

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
    float * mask = loadCSV(folderPath + fileName + "_mask.csv", h, w);
    
    if(!mask)
    {
        mask = (float *)malloc(h*w*sizeof(float));
        for(int i = 0; i < h; i++)
            for(int j = 0; j < w; j++)
                mask[i*w + j] = 255;
    }
    
    
    /* Sets up the instance variables */
    std::vector<Point> pos_residues, neg_residues;
    unsigned char * bitmap = (unsigned char *)malloc( h * w * sizeof(char));
    SearchForResidues(buffer, bitmap, pos_residues, neg_residues, w, h);
    
    Heuristic * method = new IteratedLocalSearch();
    method->SetInputs(bitmap, pos_residues, neg_residues, w, h);
    method->RunMethod();
    
    SaveBitmap(bitmap, w, h, folderPath+fileName);
    double * solution = (double *)malloc( h*w*sizeof(double));
    
    UnwrapImage(solution, buffer, bitmap, mask, w, h);
    saveCSV(solution, folderPath+fileName+"_UNWRAPPED.csv", h, w);
    
    
    
}