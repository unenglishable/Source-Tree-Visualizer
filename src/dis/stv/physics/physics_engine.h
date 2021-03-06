/*
 * physics_engine.h
 *
 *  Created on: Aug 15, 2010
 *      Author: programmer
 */

#ifndef PHYSICS_ENGINE_H_
#define PHYSICS_ENGINE_H_

#include <stdlib.h>
#include <vector>
#include <math.h>
#include "physical_mass.h"
#include "../system/discursive_system.h"


using namespace std;

class PhysicsEngine
{
protected:
	vector<PhysicalMass*> masses;
public:
	virtual void addMass(PhysicalMass* mass) = 0;
	virtual void run() = 0;
};

#endif /* PHYSICS_ENGINE_H_ */
