//  Created by ngbaanh on 05/05/2019.
//  Copyright © 2019 mac. All rights reserved.
//

#include "GameWorld.hpp"

int AbstractSpecies::TOTAL_COUNT = 0;
int AbstractSpecies::DEAD_COUNT = 0;

Direction __DirectionRandomly() { return Direction(rand() % 4); };
string __DirectionText(Direction d) {
    switch (d) {
        case DirectionNorth: return "^"; //"▲" "^"
        case DirectionSouth: return "v"; //"▼" "v"
        case DirectionEast:  return ">"; //"▶︎" ">"
        case DirectionWest:  return "<"; //"◀︎" "<"
    }
}

AbstractSpecies::AbstractSpecies() {
    this->identifier = 1 + AbstractSpecies::TOTAL_COUNT++;
    this->hasEgg = false;
    this->direction = __DirectionRandomly();
    this->isGenerative = true;
    this->isReady = false;
    this->restoreLifeCycles();
    this->restoreReproductionCycles();
}

AbstractSpecies::~AbstractSpecies() {
    AbstractSpecies::DEAD_COUNT++;
}

void AbstractSpecies::restoreLifeCycles() {
    this->remainingLifeCycles = kMax;
}

void AbstractSpecies::restoreReproductionCycles() {
    this->remainingReproductionCycles = kMax;
}

Status AbstractSpecies::die() {
    G->cellOf(this)->clean();
    delete this;
    return StatusSuccess;
}

bool AbstractSpecies::mayDie() {
    return this->remainingLifeCycles <= 2;
}

bool AbstractSpecies::mustDie() {
    return this->remainingLifeCycles <= kZero;
}

Status AbstractSpecies::rotateRight() {
    if (!G->isReady()) { return StatusNotReady; }
    int newDirection = this->direction >= 3 ? 0 : this->direction + 1;
    this->direction = Direction(newDirection);
    return StatusSuccess;
}

Status AbstractSpecies::rotateLeft() {
    if (!G->isReady()) { return StatusNotReady; }
    int newDirection = this->direction <= 0 ? 3 : this->direction - 1;
    this->direction = Direction(newDirection);
    return StatusSuccess;
}

bool AbstractSpecies::isFacingBorder() {
    Cell *cell = G->cellOf(this);
    if (cell->isEmpty()) { return true; }
    Position p = cell->position;
    Direction d = G->speciesAt(p)->direction;
    return (d == DirectionNorth && p.r == 0)
        || (d == DirectionEast  && p.c == G->getColumns() - 1)
        || (d == DirectionSouth && p.r == G->getRows() - 1)
        || (d == DirectionWest  && p.c == 0);
}

bool AbstractSpecies::isFacingSpecies() {
    if (this->isFacingBorder()) { return false; }
    return NULL != G->speciesAt(nextPosition());
}

Position AbstractSpecies::nextPosition() {
    Cell *cell = G->cellOf(this);
    Position p = cell->position;
    Direction d = G->speciesAt(p)->direction;
    Word row, col;
    switch (d) {
        case DirectionNorth: row = p.r - 1; col = p.c; break;
        case DirectionSouth: row = p.r + 1; col = p.c; break;
        case DirectionEast:  row = p.r; col = p.c + 1; break;
        case DirectionWest:  row = p.r; col = p.c - 1; break;
    }
    return Position(row, col);
}

Status AbstractSpecies::advance() {
    if (!isMovable) { return StatusDoNothing; }
    if (!G->isReady()) { return StatusNotReady; }
    
    Cell *cell = G->cellOf(this);
    if (cell->isEmpty()) { return StatusError; }
    if (this->isFacingBorder() || isFacingSpecies()) { return StatusFailed; }
    if (this->isFacingSpecies()) { return StatusFailed; }
    
    G->cellAt(nextPosition())->opaque = this;
    cell->clean();
    return StatusSuccess;
}

string AbstractSpecies::label() {
    string t = tag();
    if (this->mayDie()) { for (auto& c: t) { c = tolower(c); } }
    string eggLabel = this->hasEgg ? "*" : " ";
    string rightLabel = this->isReady ? " " : ".";
    return eggLabel + t + __DirectionText(direction) + rightLabel;
}

bool AbstractSpecies::canAttack(AbstractSpecies *aSpecies) {
    return G->isSpeciesAlive(aSpecies) && this->tag() != aSpecies->tag();
}

Status AbstractSpecies::attackIfPossible() {
    if (this->isFacingBorder() || !isFacingSpecies()) { return StatusFailed; }
    AbstractSpecies* victim = G->speciesAt(nextPosition());
    Cell* victimCell = G->cellOf(victim);
    if (!this->canAttack(victim)) { return StatusFailed; }
    victim->die();
    victimCell->clean();
    AbstractSpecies* newSpecies = this->copy();
    if (newSpecies == NULL) { return StatusFailed; }
    G->addSpecies(newSpecies, victimCell->position);
    this->restoreLifeCycles();
    return StatusSuccess;
}

Status AbstractSpecies::onTurn() {
    if (!this->isReady) { return StatusFailed; }
    if (!this->hasNeighborPartner()) { this->mother = NULL; }
    if (this->hasEgg) {
        Position p = this->findAnEmptyPositionAround();
        if (p.isValid()) {
            AbstractSpecies* child = this->copy();
            child->mother = this;
            G->cellAt(p)->opaque = child;
            this->hasEgg = false;
            G->repaint(kDelayMS);
        }
    } else if (this->canReproduceNow()) {
        this->hasEgg = true;
        restoreReproductionCycles();
        G->repaint(kDelayMS);
    }
    if (this->remainingReproductionCycles > 0) {
        remainingReproductionCycles--;
    }
    if (attackIfPossible() == StatusSuccess) {
        return StatusSuccess;
    }
    if (this->remainingLifeCycles > 0) {
        remainingLifeCycles--;
    }
    if (this->mustDie()) {
        return this->die();
    }
    return StatusDoNothing; // Continue in derived class
}

bool AbstractSpecies::canReproduceNow() {
    return this->isGenerative && !this->hasEgg
        && this->remainingReproductionCycles == 0
        && this->hasNeighborPartner();
}

bool AbstractSpecies::hasNeighborPartner(bool exceptMother, Byte* count) {
    AbstractSpecies* s = NULL;
    Word neighborCount = 0;
    Position p = G->cellOf(this)->position;
    bool hasNeighbor = false;
    bool hasMother = false;
    if (p.r > 0) {
        s = G->speciesAt(p.r - 1, p.c);
        if (this->isFamiliarTo(s)) {
            hasNeighbor = true;
            neighborCount++;
            if (this->mother == s || s->mother == this) {
                hasMother = true;
                neighborCount--;
            }
        }
    }
    if (p.r < G->getRows() - 1) {
        s = G->speciesAt(p.r + 1, p.c);
        if (this->isFamiliarTo(s)) {
            hasNeighbor = true;
            neighborCount++;
            if (this->mother == s || s->mother == this) {
                hasMother = true;
                neighborCount--;
            }
        }
    }
    if (p.c > 0) {
        s = G->speciesAt(p.r, p.c - 1);
        if (this->isFamiliarTo(s)) {
            hasNeighbor = true;
            neighborCount++;
            if (this->mother == s || s->mother == this) {
                hasMother = true;
                neighborCount--;
            }
        }
    }
    if (p.c < G->getColumns() - 1) {
        s = G->speciesAt(p.r, p.c + 1);
        if (this->isFamiliarTo(s)) {
            hasNeighbor = true;
            neighborCount++;
            if (this->mother == s || s->mother == this) {
                hasMother = true;
                neighborCount--;
            }
        }
    }
    if (count != NULL) { *count = neighborCount; }
    return hasNeighbor && (exceptMother ? !hasMother : true);
}

Position AbstractSpecies::findAnEmptyPositionAround() {
    Byte r, c;
    vector<Position> v;
    Position p = G->cellOf(this)->position;
    if (p.r > 0) {
        r = p.r - 1; c = p.c;
        if (G->cellAt(r, c)->isEmpty()) { v.push_back((Position(r,c))); }
    }
    if (p.r < G->getRows() - 1) {
        r = p.r + 1; c = p.c;
        if (G->cellAt(r, c)->isEmpty()) { v.push_back((Position(r,c))); }
    }
    if (p.c > 0) {
        r = p.r; c = p.c - 1;
        if (G->cellAt(r, c)->isEmpty()) { v.push_back((Position(r,c))); }
    }
    if (p.c < G->getColumns() - 1) {
        r = p.r; c = p.c + 1;
        if (G->cellAt(r, c)->isEmpty()) { v.push_back((Position(r,c))); }
    }
    return v.empty() ? Position::none() : v.at(rand() % v.size());
}
