//  Created by ngbaanh on 05/05/2019.
//  Copyright © 2019 mac. All rights reserved.
//

#ifndef AbstractSpecies_hpp
#define AbstractSpecies_hpp

#include "GameTypes.hpp"

#define TOTAL_COUNT __TOTAL_COUNT
#define DEAD_COUNT  __DEAD_COUNT
#define ALIVE_COUNT_OF(SPECIES) (SPECIES::TOTAL_COUNT-SPECIES::DEAD_COUNT)

class AbstractSpecies {
    
protected:
    AbstractSpecies();
    
public:
    virtual ~AbstractSpecies();
    static int TOTAL_COUNT;
    static int DEAD_COUNT;
    
    /// Identifier for every new species. It should be unique, starts from 1.
    int identifier;
    
    /// The direction which the species is facing to.
    Direction direction;
    
    /// Check if the species can reproduce new species from it.
    bool isGenerative;
    
    /// Check if that the species can move or not.
    bool isMovable;
    
    /// Check if the species is ready to has turn or not yet. A newly generated
    /// species is not ready until new cycle of the game world;
    bool isReady;
    
    /// The cycle count it has until it dies because of being hungry.
    Word remainingLifeCycles;
    
    /// Determine the species is carrying an egg or not.
    bool hasEgg;
    
    /// Remaining cycles until the species can reproduce a new one
    Word remainingReproductionCycles;
    
    /// Its mother cared the egg, which is not able to pair to reproduce
    AbstractSpecies* mother;
    
    
    // MARK: - Actions
    
    /// Rotate the current direction to the right
    Status rotateRight();
    
    /// Rotate the current direction to the left
    Status rotateLeft();
    
    /// Advance to the next position if possible.
    Status advance();
    
    /// Check if the species is looking at the borders
    bool isFacingBorder();
    
    /// Check if the next position has a species
    bool isFacingSpecies();
    
    /// Return next position of a species if it is not facing the borders.
    /// Notice: This method doesn't check if the next position
    /// is in the game world or not! Check first by using `isFacingBorder()`
    Position nextPosition();
    
    /// Attack an enemy in front of it
    Status attackIfPossible();
    
    /// Return a name of a species for graphic drawing
    string label();
    
    /// Check if 2 species is the same race or not
    bool isFamiliarTo(AbstractSpecies* s) {
        return NULL != s && 0 == tag().compare(s->tag());
    }
    
    /// Check if the species can attack or not
    bool canAttack(AbstractSpecies* aSpecies);
    
    /// Check if the species now can reproduce new species or not
    bool canReproduceNow();
    
    /// Check if there's at least one neighbor which this species is not its mom
    /// param `exceptMother`: avoid its mother in neighbor list if TRUE.
    /// param `count`: inout pointer to get the neighbor number.
    bool hasNeighborPartner(bool exceptMother = true, Byte* count = NULL);
    
    /// Find an empty position around.
    /// Return Position::none if there's no blank position.
    Position findAnEmptyPositionAround();
    
    /// Die, remove the species (and its egg) out of the game.
    Status die();
    
    //--------------------------------------------------------------
    // MARK: - Virtual actions, sub classes MAY override them
    
    
    /// Method for a basic action of a species in a turn, base on a specific
    /// species it will chose what to act.
    virtual Status onTurn();
    
    /// When it attacks a victim succesfully, it becomes `full`
    virtual void restoreLifeCycles();
    
    /// Reset cycle number until it can reproduce again
    virtual void restoreReproductionCycles();
    
    /// Check if this object should be deleted now.
    virtual bool mustDie();
    
    /// Check if this object may die in short-terms future for some reasons.
    virtual bool mayDie();
    
    /// Make a new species object like this species
    virtual AbstractSpecies* copy() { return NULL; };
    
    //--------------------------------------------------------------
    // MARK: - Pure virtual actions, sub classes MUST override them
    
    /// Return the tag of the species to identify its race.
    virtual string tag() PURE_VIRTUAL;
    
    // anything else?
    // ...
};

#endif // AbstractSpecies_hpp
