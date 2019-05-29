//
//  Germ.hpp
//  SurvivalCpp
//
//  Created by ngbaanh on 07/05/2019.
//  Copyright © 2019 mac. All rights reserved.
//

#ifndef Germ_hpp
#define Germ_hpp

#include "GameTypes.hpp"
#include "AbstractMicrobe.hpp"

class Germ : public AbstractMicrobe {
public:
    static string TAG;
    static int TOTAL_COUNT;
    static int DEAD_COUNT;
    Germ();
    ~Germ();
    
    Status onTurn() override;
    string tag() override;
    Germ* copy() override;
    void restoreReproductionCycles() override;
    bool mustDie() override;
    bool mayDie() override { return mustDie(); };
};

#endif /* Germ_hpp */
