//  Created by ngbaanh on 05/05/2019.
//  Copyright © 2019 mac. All rights reserved.
//

#include "Virus.hpp"

string Virus::TAG = "V";
int Virus::TOTAL_COUNT = 0;
int Virus::DEAD_COUNT = 0;

Virus::Virus(): AbstractMicrobe() {
    Virus::TOTAL_COUNT++;
    this->isGenerative = false;
}

Virus::~Virus() { Virus::DEAD_COUNT++; }

string Virus::tag() { return Virus::TAG; }

Virus* Virus::copy() {
    Virus* newOne = new Virus();
    newOne->direction = direction;
    return newOne;
}

Status Virus::onTurn() {
    Status status = AbstractSpecies::onTurn();
    if (StatusDoNothing != status) { return status; }
    return rotateRight();
}
