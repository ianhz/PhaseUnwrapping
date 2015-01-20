//
//  Util.h
//  unwrapping
//
//  Created by Ian Herszterg on 1/11/15.
//  Copyright (c) 2015 Ian Herszterg. All rights reserved.
//

#ifndef __unwrapping__Util__
#define __unwrapping__Util__

#include <stdio.h>
#include <math.h>
#include "ImageHandler.h"
#include <iostream>
#include "Residue.h"

float * loadCSV( std::string filepath, int h, int w );
unsigned char * loadCSVChar( std::string filepath, int h, int w );
void saveCSVFloat( float * solution, std::string filepath, int h, int w );
void saveCSV( double * solution, std::string filepath, int h, int w );
void saveCSVChar( unsigned char * solution, std::string filepath, int h, int w );
void saveCSVInt( int * solution, std::string filepath, int h, int w );
double EuclideanDistance(int i1, int j1, int i2, int j2);
void SaveBitmap( unsigned char * imgData, int w, int h, std::string filePath );


#endif /* defined(__unwrapping__Util__) */
