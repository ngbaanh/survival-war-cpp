//
//  Falcon.hpp
//  SurvivalCpp
//
//  Created by ngbaanh on 07/05/2019.
//  Copyright © 2019 mac. All rights reserved.
//

#ifndef Falcon_hpp
#define Falcon_hpp

#include "GameTypes.hpp"
#include "AbstractAerialAnimal.hpp"

class Falcon : public AbstractAerialAnimal {
public:
    static string TAG;
    static int TOTAL_COUNT;
    static int DEAD_COUNT;
    Falcon();
    ~Falcon();
    AbstractSpecies* findNearestVictim();
    
    string tag() override;
    Falcon* copy() override;
    AbstractSpecies* findNearestTarget() override {
        return findNearestVictim();
    };
};

#endif /* Falcon_hpp */
