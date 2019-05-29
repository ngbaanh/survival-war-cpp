//
//  AbstractAerialAnimal.cpp
//  SurvivalCpp
//
//  Created by ngbaanh on 07/05/2019.
//  Copyright © 2019 mac. All rights reserved.
//

#include "GameWorld.hpp"
#include "AbstractAerialAnimal.hpp"

int AbstractAerialAnimal::TOTAL_COUNT = 0;
int AbstractAerialAnimal::DEAD_COUNT = 0;

AbstractAerialAnimal::AbstractAerialAnimal() : AbstractSpecies() {
    this->isMovable = true;
    this->isGenerative = true;
    this->restoreReproductionCycles();
    this->restoreLifeCycles();
    AbstractAerialAnimal::TOTAL_COUNT++;
}

AbstractAerialAnimal::~AbstractAerialAnimal() {
    AbstractAerialAnimal::DEAD_COUNT++;
}

void AbstractAerialAnimal::restoreReproductionCycles() {
    this->remainingReproductionCycles = 4;
}

void AbstractAerialAnimal::restoreLifeCycles() {
    this->remainingLifeCycles = 6;
}

Word AbstractAerialAnimal::pathLengthFrom(ANCHOR a, AbstractSpecies* aSpecies) {
    if (!G->isSpeciesAlive(aSpecies)) { return kMax; }
    Position anchorPosition;
    if (a == NEXT_TO) {
        anchorPosition = this->nextPosition();
    } else {
        anchorPosition = G->cellOf(this)->position;
        if (a != SELF_TO) {
            switch (this->direction) {
                case DirectionEast:  anchorPosition.r +=  a; break;
                case DirectionWest:  anchorPosition.r += -a; break;
                case DirectionNorth: anchorPosition.c +=  a; break;
                case DirectionSouth: anchorPosition.c += -a; break;
            }
        }
    }
    if (!G->isPositionInBounds(anchorPosition)) { return kMax; }
    return G->cellOf(aSpecies)->position.pathLengthTo(anchorPosition);
}

Status AbstractAerialAnimal::onTurn() {
    Status result = AbstractSpecies::onTurn();
    if (StatusDoNothing != result) { return result; }
    
    this->target = this->findNearestTarget();
//    bool targetNotAlive = !G->isSpeciesAlive(this->target);
//    if (targetNotAlive) {
//        this->target = this->findNearestTarget();
//    }
    bool canAdvance = !(this->isFacingSpecies() || isFacingBorder());
    bool alone = !G->isSpeciesAlive(this->target);
    if (alone) {
        if (canAdvance) { return advance(); }
        else { return rand() % 2 ? rotateLeft() : rotateRight(); }
    }
    if (canAdvance) {
        Word next = pathLengthFrom(NEXT_TO, this->target);
        Word self = pathLengthFrom(SELF_TO, this->target);
        if (next < self) { return advance(); }
    }
    Word left = pathLengthFrom(LEFT_TO, this->target);
    Word right = pathLengthFrom(RIGHT_TO, this->target);
    return left < right ? rotateLeft() : rotateRight();
}
