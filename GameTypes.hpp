//
//  GameTypes.hpp
//  SurvivalCpp
//
//  Created by ngbaanh on 06/05/2019.
//  Copyright © 2019 mac. All rights reserved.
//

#ifndef GameTypes_h
#define GameTypes_h

#include <cstdlib>
#include <iostream>
using namespace std;

#define PURE_VIRTUAL =0
#define kMax         33333
#define kZero        0
#define kDelayMS     50

typedef unsigned char Byte;
typedef unsigned int  Word;

/// Status for every operation in the game.
typedef enum {
    StatusSuccess     = 0,
    StatusError       = 1,
    StatusDoNothing   = 1 << 1,
    StatusFailed      = 1 << 2,
    StatusNotReady    = 1 << 3,
} Status;


/// Definition of 4 directions of a species, where it is facing to.
typedef enum {
    DirectionNorth = 0,
    DirectionEast  = 1,
    DirectionSouth = 2,
    DirectionWest  = 3
} Direction;

/// Definition of 2D coordinate consists indicies of (row, col) of matrix
typedef struct __Position {
    Byte r, c;
    __Position(Byte r, Byte c) {
        this->r = r;
        this->c = c;
    }
    __Position() { *this = none(); }
    bool operator ==(struct __Position p) { return r == p.r && c == p.c; }
    bool operator !=(struct __Position p) { return !(*this == p); }
    bool isValid() { return r >= 0 && c >= 0 && r < 255 && c < 255; }
    Word pathLengthTo(struct __Position p) {
        return !p.isValid() ? kMax : abs(c - p.c) + abs(r - p.r); // maybe wrong calculation because of unsigned type.
    }
    static struct __Position none() { return __Position(255, 255); }
} Position;

/// Definition of a cell in the game world
typedef struct __Cell {
    Position position;
    void* opaque;
    __Cell(void* opaque = NULL, Position position = Position::none()) {
        this->opaque = opaque;
        this->position = position;
    }
    bool isEmpty() { return NULL == opaque; }
    void clean() { opaque = NULL; }
} Cell;

#endif /* GameTypes_h */
