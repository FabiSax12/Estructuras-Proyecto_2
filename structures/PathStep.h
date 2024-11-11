//
// Created by varga on 6/11/2024.
//

#ifndef PATHSTEP_H
#define PATHSTEP_H

#include "Destination.h"

struct PathStep {
	Destination* origin;
	Route* route;
};



#endif //PATHSTEP_H
