//
//  Dino.hpp
//  SurvivalCpp
//
//  Created by ngbaanh on 07/05/2019.
//  Copyright © 2019 mac. All rights reserved.
//

#ifndef Dino_hpp
#define Dino_hpp

#include "GameTypes.hpp"
#include "AbstractSpecies.hpp"

class Dino : public AbstractSpecies {
public:
    static string TAG;
    static int TOTAL_COUNT;
    static int DEAD_COUNT;
    Dino();
    ~Dino();
    
    Status onTurn() override;
    string tag() override;
    Dino* copy() override;
    void restoreLifeCycles() override;
    void restoreReproductionCycles() override;
};

#endif /* Dino_hpp */
