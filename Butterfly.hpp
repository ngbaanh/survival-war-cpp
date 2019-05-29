//
//  Butterfly.hpp
//  SurvivalCpp
//
//  Created by ngbaanh on 07/05/2019.
//  Copyright © 2019 mac. All rights reserved.
//

#ifndef Butterfly_hpp
#define Butterfly_hpp

#include "GameTypes.hpp"
#include "AbstractAerialAnimal.hpp"

class Butterfly : public AbstractAerialAnimal {
public:
    static string TAG;
    static int TOTAL_COUNT;
    static int DEAD_COUNT;
    Butterfly();
    ~Butterfly();
    Butterfly* findNearestFriend();
    
    string tag() override;
    Butterfly* copy() override;
    AbstractSpecies* findNearestTarget() override {
        return findNearestFriend();
    };
};

#endif /* Butterfly_hpp */
