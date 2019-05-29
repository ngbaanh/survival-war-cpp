//
//  Falcon.cpp
//  SurvivalCpp
//
//  Created by ngbaanh on 07/05/2019.
//  Copyright © 2019 mac. All rights reserved.
//

#include "Falcon.hpp"
#include "GameWorld.hpp"

string Falcon::TAG = "F";
int Falcon::TOTAL_COUNT = 0;
int Falcon::DEAD_COUNT = 0;

Falcon::Falcon(): AbstractAerialAnimal() { Falcon::TOTAL_COUNT++; }

Falcon::~Falcon() { Falcon::DEAD_COUNT++; }

string Falcon::tag() { return Falcon::TAG; };

Falcon* Falcon::copy() {
    Falcon* newOne = new Falcon();
    newOne->direction = direction;
    return newOne;
}

AbstractSpecies* Falcon::findNearestVictim() {
    AbstractSpecies* s;
    vector<Pair> victims;
    bool isVictim;
    for (auto cell : G->getCells()) {
        if (cell.isEmpty()) { continue; }
        s = G->speciesIn(cell);
        isVictim = G->isSpeciesAlive(s) && !this->isFamiliarTo(s);
        if (!isVictim) { continue; }
        victims.push_back(make_pair(pathLengthFrom(SELF_TO, s), s->identifier));
    }
    if (victims.empty()) { return NULL; }
    SORT(victims);
    return G->findSpeciesByID(victims.at(0).second);
}

