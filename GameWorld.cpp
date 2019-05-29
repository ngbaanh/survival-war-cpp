//
//  GameWorld.cpp
//  SurvivalCpp
//
//  Created by ngbaanh on 06/05/2019.
//  Copyright © 2019 mac. All rights reserved.
//

#include "GameWorld.hpp"

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

void __clear() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void GameWorld::pause(Word ms) {
#ifdef _WIN32
    Sleep(ms);
#else
    usleep(ms*1000);  // to micro-second
#endif
}

// ------------------------
Status GameWorld::makeWorldWithSize(Byte rows, Byte columns) {
    if (rows <= 1 || columns <= 1 || rows >= 255 || columns >= 255) {
        return StatusError;
    }
    this->rows = rows;
    this->columns = columns;
    Byte r, c;
    for (r = kZero; r < rows; r++) {
        for (c = kZero; c < columns; c++) {
            cells.push_back(Cell(NULL, Position(r,c)));
        }
    }
    return StatusSuccess;
}

bool GameWorld::isReady() {
    return !(cells.empty() || rows <= 1 || columns <= 1
            || rows >= 255 || columns >= 255);
}

Status GameWorld::addSpecies(AbstractSpecies *aSpecies, Position position) {
    if (!isReady()) { return StatusNotReady; }
    Cell *cell = cellAt(position);
    if (!cell->isEmpty()) { return StatusDoNothing; }
    cell->opaque = aSpecies;
    return StatusSuccess;
}

Status GameWorld::addSpeciesRandomly(AbstractSpecies *aSpecies) {
    vector<Word> possibleIndices;
    Word i, count = (Word) cells.size();
    for (i = kZero; i < count; i++) {
        if (cells.at(i).isEmpty()) { possibleIndices.push_back(i); }
    }
    if (possibleIndices.empty()) { return StatusDoNothing; }
    i = possibleIndices.at(rand() % possibleIndices.size());
    cells.at(i).opaque = aSpecies;
    return StatusSuccess;
}

bool GameWorld::isSpeciesWon(string speciesTag) {
    for (auto cell : cells) {
        if (cell.isEmpty()) { continue; }
        if (0 != (G->speciesIn(cell))->tag().compare(speciesTag)) {
            return false;
        } else { continue; }
    }
    return true;
}

bool GameWorld::isSpeciesAlive(AbstractSpecies* aSpecies) {
    if (NULL == aSpecies) { return false; }
    for (auto cell : cells) {
        if (cell.isEmpty()) { continue; }
        if (cell.opaque == aSpecies) { return true; }
    }
    return false;
}

Cell* GameWorld::cellAt(Position position) {
    return cellAt(position.r, position.c);
}

Cell* GameWorld::cellAt(Byte r, Byte c) {
    Word index = c + this->columns * r;
    return &cells.at(index);
}

AbstractSpecies* GameWorld::findSpeciesByID(int identifier) {
    AbstractSpecies* s;
    for (auto cell: cells) {
        s = G->speciesIn(cell);
        if (isSpeciesAlive(s) && s->identifier == identifier) { return s; }
    }
    return NULL;
}

AbstractSpecies* GameWorld::speciesAt(Position position) {
    return speciesAt(position.r, position.c);
}

AbstractSpecies* GameWorld::speciesAt(Byte r, Byte c) {
    Cell* cell = cellAt(r, c);
    return cell->isEmpty() ? NULL : G->speciesIn(*cell);
}

Cell* GameWorld::cellOf(AbstractSpecies *aSpecies) {
    Word i, count = getCellCount();
    Cell* cell = NULL;
    AbstractSpecies* s = NULL;
    for (i = kZero; i < count; i++) {
        cell = &cells.at(i);
        s = G->speciesIn(*cell);
        if (s != NULL && s->identifier == aSpecies->identifier) { break; }
    }
    return cell;
}

AbstractSpecies* GameWorld::speciesIn(Cell cell) {
    if (cell.isEmpty()) { return NULL; }
    return (AbstractSpecies*) cell.opaque;
}

Status GameWorld::start() {
    if (!isReady()) { return StatusNotReady; }
    Cell cell;
    AbstractSpecies* species = NULL;
    vector<int> idVector;
    Word i, count = (Word) cells.size();
    isRunning = true;
    cycles = kZero;
    turns = kZero;
    
    while (isRunning) {
        cycles++;
        idVector.clear();
        for (i = kZero; i < count; i++) {
            cell = cells.at(i);
            if (cell.isEmpty()) { continue; }
            idVector.push_back(G->speciesIn(cell)->identifier);
        }
        SORT(idVector);
        this->activeSpeciesBy(idVector);
        i = kZero;
        while (isRunning && i < idVector.size()) { // [!] dynamic size
            turns++;
            species = findSpeciesByID(idVector.at(i));
            this->repaint(1); // Refresh
            if (isSpeciesAlive(species)) {
                if (isSpeciesWon(species->tag())) {
                    cout << "---------------------------" << endl;
                    cout << "GAME OVER: " << species->tag() << " WIN!" << endl;
                    isRunning = false;
                    break;
                }
                species->onTurn();
            } else {
                species = NULL;
                idVector.erase(idVector.begin() + i--);
            }
            i++;
            this->repaint();
        }
    }
    ERASE(idVector);
    cleanOnEndGame();
    return StatusSuccess;
}

void GameWorld::repaint(Word miliSec) {
    if (!isReady()) {
        cout << "Game not ready!" << endl;
        return;
    }
    __clear();
    
    cout << "SURVIVAL WAR " << getRows() << " x " << getColumns();
    cout << " - Cycle: " << cycles << " - Turn: " << turns << endl;
    cout << "Locked: `.`   Has Egg: `*`   May Die: lowercase" << endl << endl;
    
    Byte rowsPerSpecies = 2;  // `+----``+----`
    Byte colsPerSpecies = 5;  // `| D> ``|*B> `
    Word r, i, iMax = rows * rowsPerSpecies + 1;
    Word c, j, jMax = columns * colsPerSpecies + 1;
    AbstractSpecies* s;
    
    for (i = kZero; i < iMax; i++) {
        for (j = kZero; j < jMax; j++) {
            if (kZero == i % rowsPerSpecies) {
                cout << (j % colsPerSpecies ? "-" : "+");
                continue;
            }
            if (j % colsPerSpecies) { cout << " "; continue; }
            cout << "|";
            r = i / rowsPerSpecies; // floor
            c = j / colsPerSpecies; // floor
            if (r >= rows || c >= columns) { continue; }
            s = speciesAt(r, c);
            if (!isSpeciesAlive(s)) { continue; }
            cout << s->label();
            j += s->label().size();
        }
        cout << endl;
    }
    cout << endl;
    
    printInfo();
    this->pause(miliSec);
}


void GameWorld::printInfo() {
    cout << " ALL SPECIES ------- Alive: " << ALIVE_COUNT_OF(AbstractSpecies);
    cout << ", Dead: "  << AbstractSpecies::DEAD_COUNT;
    cout << ", Total: " << AbstractSpecies::TOTAL_COUNT << endl;
    
    cout << endl << " -{MICROBE}---------";
    cout << " Alive: "  << ALIVE_COUNT_OF(AbstractMicrobe);
    cout << ", Dead: "  << AbstractMicrobe::DEAD_COUNT;
    cout << ", Total: " << AbstractMicrobe::TOTAL_COUNT << " ---" << endl;
    
    cout << "     Virus - " + Virus::TAG + " -----";
    cout << " Alive: "  << ALIVE_COUNT_OF(Virus);
    cout << ", Dead: "  << Virus::DEAD_COUNT;
    cout << ", Total: " << Virus::TOTAL_COUNT << endl;
    
    cout << "     Germ -- " + Germ::TAG  + " -----";
    cout << " Alive: "  << ALIVE_COUNT_OF(Germ);
    cout << ", Dead: "  << Germ::DEAD_COUNT;
    cout << ", Total: " << Germ::TOTAL_COUNT << endl;
    
    cout << endl << " -{AERIAL ANIMAL}---";
    cout << " Alive: "  << ALIVE_COUNT_OF(AbstractAerialAnimal);
    cout << ", Dead: "  << AbstractAerialAnimal::DEAD_COUNT;
    cout << ", Total: " << AbstractAerialAnimal::TOTAL_COUNT << " ---" << endl;
    
    cout << "     Butterfly - " + Butterfly::TAG + " -";
    cout << " Alive: "  << ALIVE_COUNT_OF(Butterfly);
    cout << ", Dead: "  << Butterfly::DEAD_COUNT;
    cout << ", Total: " << Butterfly::TOTAL_COUNT << endl;
    
    cout << "     Falcon ---- " + Falcon::TAG + " -";
    cout << " Alive: "  << ALIVE_COUNT_OF(Falcon);
    cout << ", Dead: "  << Falcon::DEAD_COUNT;
    cout << ", Total: " << Falcon::TOTAL_COUNT << endl;
    
    cout << endl << " - Dinosaur - " + Dino::TAG  + " ----";
    cout << " Alive: "  << ALIVE_COUNT_OF(Dino);
    cout << ", Dead: "  << Dino::DEAD_COUNT;
    cout << ", Total: " << Dino::TOTAL_COUNT << " ---" << endl;
}

void GameWorld::cleanOnEndGame() {
    for (auto cell : cells) {
        if (cell.isEmpty()) { continue; }
        G->speciesIn(cell)->die();
    }
    ERASE(cells);
}

bool GameWorld::isPositionInBounds(Position p) {
    return p.isValid() && p.r < rows && p.c < columns;
}

void GameWorld::activeSpeciesBy(vector<int> speciesIDs) {
    AbstractSpecies* s;
    for (auto id: speciesIDs) {
        s = this->findSpeciesByID(id);
        if (isSpeciesAlive(s)) { s->isReady = true; }
    }
}
