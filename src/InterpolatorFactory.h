#ifndef _INTERPOLATORFACTORY_H
#define _INTERPOLATORFACTORY_H

#include "Interpolator.h"

using namespace std;

class InterpolatorFactory {

public:
	static Interpolator* create(string name);

	static uint8_t getInterpolatorNames(string** nameBuffer);

	virtual ~InterpolatorFactory();

protected:
	InterpolatorFactory();
};

#endif
