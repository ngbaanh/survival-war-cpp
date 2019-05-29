//  Created by ngbaanh on 05/05/2019.
//  Copyright © 2019 mac. All rights reserved.
//

#include "AbstractMicrobe.hpp"

int AbstractMicrobe::TOTAL_COUNT = 0;
int AbstractMicrobe::DEAD_COUNT = 0;

AbstractMicrobe::AbstractMicrobe() : AbstractSpecies() {
    AbstractMicrobe::TOTAL_COUNT++;
    this->isMovable = false;
}

AbstractMicrobe::~AbstractMicrobe() {
    AbstractMicrobe::DEAD_COUNT++;
}
