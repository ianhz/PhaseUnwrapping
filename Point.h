//
//  Point.h
//  unwrapping
//
//  Created by Ian Herszterg on 9/14/14.
//  Copyright (c) 2014 Ian Herszterg. All rights reserved.
//

#ifndef unwrapping_Point_h
#define unwrapping_Point_h

struct Point
{
    int i, j, index;
    int parent_i, parent_j, parent_index;
    char type;
    bool isBalanced = false;
    bool isBorder = false;
    bool isValid = false;
    bool visited = false;
    int group_n = -1;
    bool stolen = false;
    bool connected = false;
    int lastStolenFrom;
    double delta;
};


#endif
