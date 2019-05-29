//
//  Germ.cpp
//  SurvivalCpp
//
//  Created by ngbaanh on 07/05/2019.
//  Copyright © 2019 mac. All rights reserved.
//

#include "Germ.hpp"
#include "GameWorld.hpp"

string Germ::TAG = "G";
int Germ::TOTAL_COUNT = 0;
int Germ::DEAD_COUNT = 0;

Germ::Germ(): AbstractMicrobe() {
    Germ::TOTAL_COUNT++;
    this->restoreReproductionCycles();
}

Germ::~Germ() { Germ::DEAD_COUNT++; }

string Germ::tag() { return Germ::TAG; }

void Germ::restoreReproductionCycles() {
    this->remainingReproductionCycles = 3;
}

Germ* Germ::copy() {
    Germ* newOne = new Germ();
    newOne->direction = direction;
    return newOne;
}

bool Germ::mustDie() {
    Byte germsAround = 0;
    bool alone = !this->hasNeighborPartner(false, &germsAround);
    Position p = G->cellOf(this)->position;
    bool isAtBoundary = p.r == 0 || p.c == 0
            || p.r == G->getRows() - 1
            || p.c == G->getColumns() - 1;
    germsAround += isAtBoundary ? 1 : 0;
    return alone || germsAround == 4;
}

Status Germ::onTurn() {
    Status status = AbstractSpecies::onTurn();
    if (StatusDoNothing != status) { return status; }
    return rand() % 2 ? rotateLeft() : rotateRight();
}
