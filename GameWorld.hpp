//
//  GameWorld.hpp
//  SurvivalCpp
//
//  Created by ngbaanh on 06/05/2019.
//  Copyright © 2019 mac. All rights reserved.
//

#ifndef GameWorld_hpp
#define GameWorld_hpp

#include "GameTypes.hpp"
#include "AbstractSpecies.hpp"
#include "AbstractMicrobe.hpp"
#include "AbstractAerialAnimal.hpp"
#include "Virus.hpp"
#include "Germ.hpp"
#include "Dino.hpp"
#include "Butterfly.hpp"
#include "Falcon.hpp"

#include <vector>
#define SORT(VECTOR)  sort(VECTOR.begin(),VECTOR.end())
#define ERASE(VECTOR) VECTOR.erase(VECTOR.begin(),VECTOR.end())

/*
    0
  0 +------------> Col
    | . . . . .
    | . . . . .
    v
    Row
 */

class GameWorld {
private:
    GameWorld() {};
    Byte rows, columns;
    vector<Cell> cells;
    GameWorld(GameWorld const&); // No copy
    GameWorld& operator=(GameWorld const&); // No assign
    
    bool isRunning;
    Word cycles, turns;
    
public:
    /// Singleton object of GameWorld
    static GameWorld& shared() {
        static GameWorld instance;
        instance.isRunning = false;
        instance.cycles = 0;
        instance.turns = 0;
        return instance;
    };
    
    // MARK: -
    vector<Cell> getCells() const { return cells; }
    Word getRows() const { return rows; }
    Word getColumns() const { return columns; }
    Word getCellCount() const { return rows * columns; }
    
    /// Check if the game is ready to start.
    bool isReady();
    
    /// Build the world by using a matrix of rows * columns
    Status makeWorldWithSize(Byte rows, Byte columns);
    
    /// Provide ready state for the species (often in the current turn)
    void activeSpeciesBy(vector<int> speciesIDs);
    
    /// Add a species to an random available position
    Status addSpeciesRandomly(AbstractSpecies* aSpecies);
    
    /// Add a species to a position in the world of game.
    /// Return StatusSuccess or Status::nothing
    Status addSpecies(AbstractSpecies* aSpecies, Position position);
    
    /// Get the Cell by given position.
    /// If the position is not a species then return NULL
    Cell* cellAt(Position position);
    Cell* cellAt(Byte r, Byte c);
    
    /// Get the Species by given position.
    /// If the position is not a species then return NULL
    AbstractSpecies* speciesAt(Position position);
    AbstractSpecies* speciesAt(Byte r, Byte c);
    
    AbstractSpecies* speciesIn(Cell cell);
    
    /// Find a species in the game by its identifier, return NULL if not found
    AbstractSpecies* findSpeciesByID(int identifier);
    
    /// Get the Cell of a given species.
    /// Try to find the species in the game and return its cell if possible
    Cell* cellOf(AbstractSpecies* aSpecies);
    
    /// Start of the game world if it is ready
    Status start();
    
    /// Check if the species is in game world or not
    bool isSpeciesAlive(AbstractSpecies* aSpecies);
    
    /// check if the species win the game
    bool isSpeciesWon(string speciesTag);
    
    /// Clean data before exit
    void cleanOnEndGame();
    
    /// Check if a position is valid in GameWorld
    bool isPositionInBounds(Position p);
    
    // MARK: - Graphic
    
    /// Draw the current state of the game, delay in `miliSec` ms
    void repaint(Word miliSec = kDelayMS);
    
    /// Print game information: species count, living count, dead count.
    void printInfo();
    
    /// Pause the game in mili seconds.
    void pause(Word ms);
};

/// Shorthand of Singleton Gameworld
static GameWorld* G = &GameWorld::shared();

#endif /* GameWorld_hpp */
