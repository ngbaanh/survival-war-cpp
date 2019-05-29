//  Created by ngbaanh on 05/05/2019.
//  Copyright © 2019 mac. All rights reserved.
//

#ifndef Virus_hpp
#define Virus_hpp

#include "GameTypes.hpp"
#include "AbstractMicrobe.hpp"

class Virus : public AbstractMicrobe {
public:
    static string TAG;
    static int TOTAL_COUNT;
    static int DEAD_COUNT;
    Virus();
    ~Virus();
    
    Status onTurn() override;
    string tag() override;
    Virus* copy() override;
};

#endif // Virus_hpp
