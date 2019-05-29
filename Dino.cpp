//
//  Dino.cpp
//  SurvivalCpp
//
//  Created by ngbaanh on 07/05/2019.
//  Copyright © 2019 mac. All rights reserved.
//

#include "Dino.hpp"

string Dino::TAG = "D";
int Dino::TOTAL_COUNT = 0;
int Dino::DEAD_COUNT = 0;

Dino::Dino(): AbstractSpecies() {
    Dino::TOTAL_COUNT++;
    this->isMovable = true;
    this->restoreLifeCycles();
    this->restoreReproductionCycles();
}

Dino::~Dino() { Dino::DEAD_COUNT++; }

string Dino::tag() { return Dino::TAG; };

Dino* Dino::copy() {
    Dino* newOne = new Dino();
    newOne->direction = direction;
    return newOne;
}

void Dino::restoreLifeCycles() {
    this->remainingLifeCycles = 8;
}

void Dino::restoreReproductionCycles() {
    this->remainingReproductionCycles = 8;
}

Status Dino::onTurn() {
    Status status = AbstractSpecies::onTurn();
    if (StatusDoNothing != status) { return status; }
    if (this->isFacingBorder()) {
        status = rand() % 2 ? rotateLeft() : rotateRight();
    } else if (this->isFacingSpecies()) {
        if (this->attackIfPossible() == StatusSuccess) {
            status = StatusSuccess;
        } else {
            status = rand() % 2 ? rotateLeft() : rotateRight();
        }
    } else { status = advance(); }
    return status;
}
