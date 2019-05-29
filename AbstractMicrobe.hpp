//  Created by ngbaanh on 05/05/2019.
//  Copyright © 2019 mac. All rights reserved.
//

#ifndef AbstractMicrobe_hpp
#define AbstractMicrobe_hpp

#include "AbstractSpecies.hpp"

class AbstractMicrobe: public AbstractSpecies {
protected:
    AbstractMicrobe();
    virtual ~AbstractMicrobe();
    
public:
    static int TOTAL_COUNT;
    static int DEAD_COUNT;
};

#endif // AbstractMicrobe_hpp
