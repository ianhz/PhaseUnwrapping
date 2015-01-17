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
#include <iostream>

float * loadCSV( std::string filepath, int h, int w );
unsigned char * loadCSVChar( std::string filepath, int h, int w );
void saveCSVFloat( float * solution, std::string filepath, int h, int w );
void saveCSV( double * solution, std::string filepath, int h, int w );
void saveCSVChar( unsigned char * solution, std::string filepath, int h, int w );
void saveCSVInt( int * solution, std::string filepath, int h, int w );

#endif /* defined(__unwrapping__Util__) */
