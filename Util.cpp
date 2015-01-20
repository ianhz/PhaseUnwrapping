//
//  Util.cpp
//  unwrapping
//
//  Created by Ian Herszterg on 1/11/15.
//  Copyright (c) 2015 Ian Herszterg. All rights reserved.
//

#include "Util.h"

float * loadCSV( std::string filepath, int h, int w )
{
    FILE * fp = fopen(filepath.c_str(), "r");
    
    if( fp == NULL )
        return NULL;
    
    float * buffer = (float *)malloc( h * w * sizeof(float));
    float last;
    
    for( unsigned int i = 0; i < h; ++i )
    {
        for( unsigned j = 0; j < w - 1; ++j )
        {
            fscanf(fp, "%f,", &last);
            buffer[i*w + j] = last;
        }
        
        fscanf(fp, "%f", &last);
        buffer[i*w + w-1] = last;
    }
    
    fclose(fp);
    
    return buffer;
    
}

unsigned char * loadCSVChar( std::string filepath, int h, int w )
{
    FILE * fp = fopen(filepath.c_str(), "r");
    
    if( fp == NULL )
        return NULL;
    
    unsigned char * buffer = (unsigned char *)malloc( h * w * sizeof(unsigned char));
    int last;
    
    for( unsigned int i = 0; i < h; ++i )
    {
        for( unsigned j = 0; j < w - 1; ++j )
        {
            fscanf(fp, "%d,", &last);
            buffer[i*w + j] = (unsigned char)last;
        }
        
        fscanf(fp, "%d", &last);
        buffer[i*w + w-1] = last;
    }
    
    fclose(fp);
    
    return buffer;
    
}

void saveCSVFloat( float * solution, std::string filepath, int h, int w )
{
    FILE * fp = fopen(filepath.c_str(), "w");
    
    for( unsigned int i = 0; i < h; ++i )
    {
        for( unsigned j = 0; j < w - 1; ++j )
        {
            fprintf(fp, "%f,", solution[i*w + j]);
        }
        
        fprintf(fp, "%f\n", solution[i*w + w-1]);
    }
    
    fclose(fp);
}

void saveCSV( double * solution, std::string filepath, int h, int w )
{
    FILE * fp = fopen(filepath.c_str(), "w");
    
    for( unsigned int i = 0; i < h; ++i )
    {
        for( unsigned j = 0; j < w - 1; ++j )
        {
            fprintf(fp, "%lf,", solution[i*w + j]);
        }
        
        fprintf(fp, "%lf\n", solution[i*w + w-1]);
    }
    
    fclose(fp);
}

void saveCSVChar( unsigned char * solution, std::string filepath, int h, int w )
{
    FILE * fp = fopen(filepath.c_str(), "w");
    
    for( unsigned int i = 0; i < h; ++i )
    {
        for( unsigned j = 0; j < w - 1; ++j )
        {
            fprintf(fp, "%u,", solution[i*w + j]);
        }
        
        fprintf(fp, "%u\n", solution[i*w + w-1]);
    }
    
    fclose(fp);
}

void saveCSVInt( int * solution, std::string filepath, int h, int w )
{
    FILE * fp = fopen(filepath.c_str(), "w");
    
    for( unsigned int i = 0; i < h; ++i )
    {
        for( unsigned j = 0; j < w - 1; ++j )
        {
            fprintf(fp, "%d,", solution[i*w + j]);
        }
        
        fprintf(fp, "%d\n", solution[i*w + w-1]);
    }
    
    fclose(fp);
}

double EuclideanDistance(int i1, int j1, int i2, int j2)
{
    return sqrtf( powf(i1-i2, 2) + powf(j1-j2,2) );
}

void SaveBitmap( unsigned char * imgData, int w, int h, std::string filePath )
{
    Image * img = imgCreate(w, h, 3);
    
    for( unsigned int i = 0; i < h; ++i )
    {
        for( unsigned j = 0; j < w; ++j )
        {
            if(imgData[ i*w + j] == BRANCH_CUT)
                imgSetPixel3f(img, j, i, 0, 255, 0);
            else if(imgData[ i*w + j] == POS_RESIDUE )
                imgSetPixel3f(img, j, i, 0, 0, 255);
            else if( imgData[ i*w + j] == NEG_RESIDUE )
                imgSetPixel3f(img, j, i, 255, 0, 0);
            else
                imgSetPixel3f(img, j, i, 255, 255, 255);
        }
    }
    
    std::string path = filePath + "_RESIDUE.bmp";
    imgWriteBMP( (char *)path.c_str(), img);

}