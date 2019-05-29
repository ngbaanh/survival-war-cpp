//  Created by ngbaanh on 05/05/2019.
//  Copyright © 2019 mac. All rights reserved.
//

#include "GameWorld.hpp"

int main(int argc, const char * argv[]) {
    srand((Word)time(NULL));
    
    cout << "Hello, Survival War!\n";
    
    int status = StatusDoNothing;
    status = G->makeWorldWithSize(11, 11);
    
    AbstractSpecies* aVirus = new Virus();
    status = G->addSpecies(aVirus, Position(0,0));
    status = G->addSpecies(new Virus(), Position(0,1));
    status = G->addSpecies(new Germ(), Position(2,1));
    status = G->addSpecies(new Germ(), Position(2,2));

    status = G->addSpeciesRandomly(new Virus());
    status = G->addSpeciesRandomly(new Virus());
    status = G->addSpeciesRandomly(new Germ());
    status = G->addSpeciesRandomly(new Germ());
    status = G->addSpeciesRandomly(new Dino());
    status = G->addSpeciesRandomly(new Dino());
    status = G->addSpeciesRandomly(new Dino());
    status = G->addSpeciesRandomly(new Dino());
    
    Butterfly* aButterfly = new Butterfly();
    status = G->addSpeciesRandomly(aButterfly);
    status = G->addSpeciesRandomly(new Butterfly());
    status = G->addSpeciesRandomly(new Butterfly());
    status = G->addSpeciesRandomly(new Butterfly());
    status = G->addSpeciesRandomly(new Butterfly());
    status = G->addSpeciesRandomly(new Butterfly());
    status = G->addSpeciesRandomly(new Falcon());
    status = G->addSpeciesRandomly(new Falcon());
    status = G->addSpeciesRandomly(new Falcon());
    status = G->addSpeciesRandomly(new Falcon());
    
    G->start();
    
    
    
    return 0;
}
