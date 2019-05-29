//
//  AbstractAerialAnimal.hpp
//  SurvivalCpp
//
//  Created by ngbaanh on 07/05/2019.
//  Copyright © 2019 mac. All rights reserved.
//

#ifndef AbstractAerialAnimal_hpp
#define AbstractAerialAnimal_hpp

#include "AbstractSpecies.hpp"
#include <utility>

typedef enum {
    LEFT_TO  = -1,
    SELF_TO  =  0,    NEXT_TO = -kMax,
    RIGHT_TO =  1,
} ANCHOR;

typedef pair<Word, int> Pair;

class AbstractAerialAnimal: public AbstractSpecies {
protected:
    AbstractAerialAnimal();
    virtual ~AbstractAerialAnimal();
    
    /// Default value is NULL if it doesn't have a target to watch.
    AbstractSpecies* target = NULL;
    
public:
    static int TOTAL_COUNT;
    static int DEAD_COUNT;
    
    
    void restoreReproductionCycles() override;
    void restoreLifeCycles() override;
    Status onTurn() override;
    
    /// Return path length from the cell from Left or Right to a species
    /// in the game. Use `anchor` by `LEFT_TO`, `RIGHT_TO` or `SELF_TO`
    Word pathLengthFrom(ANCHOR anchor, AbstractSpecies* aSpecies);
    
    virtual AbstractSpecies* findNearestTarget() PURE_VIRTUAL;
};

#endif /* AbstractAerialAnimal_hpp */
