//
//  Butterfly.cpp
//  SurvivalCpp
//
//  Created by ngbaanh on 07/05/2019.
//  Copyright © 2019 mac. All rights reserved.
//

#include "Butterfly.hpp"
#include "GameWorld.hpp"

string Butterfly::TAG = "B";
int Butterfly::TOTAL_COUNT = 0;
int Butterfly::DEAD_COUNT = 0;

Butterfly::Butterfly(): AbstractAerialAnimal() { Butterfly::TOTAL_COUNT++; }

Butterfly::~Butterfly() { Butterfly::DEAD_COUNT++; }

string Butterfly::tag() { return Butterfly::TAG; };

Butterfly* Butterfly::copy() {
    Butterfly* newOne = new Butterfly();
    newOne->direction = direction;
    return newOne;
}

Butterfly* Butterfly::findNearestFriend() {
    AbstractSpecies* s;
    vector<Pair> friends;
    bool isFriend;
    for (auto cell : G->getCells()) {
        if (cell.isEmpty()) { continue; }
        s = G->speciesIn(cell);
        isFriend = G->isSpeciesAlive(s) && s != this && this->isFamiliarTo(s);
        if (!isFriend) { continue; }
        friends.push_back(make_pair(pathLengthFrom(SELF_TO, s), s->identifier));
    }
    if (friends.empty()) { return NULL; }
    SORT(friends);
    return (Butterfly*) G->findSpeciesByID(friends.at(0).second);
}

