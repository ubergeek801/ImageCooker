#ifndef _INTERPOLATIONTHREAD_H
#define _INTERPOLATIONTHREAD_H

#include <pthread.h>

#include "CcdData.h"
#include "InterpolationController.h"
#include "Interpolator.h"
#include "Pixmap.h"
#include "types.h"

class InterpolationThread {

public:
	InterpolationThread(const CcdData& ccdData, Interpolator&
			interpolator, Pixmap<uint16_t>& pixmap,
			InterpolationController& controller);

	void interpolateGreen(uint16_t startLine, uint16_t endLine, uint16_t
			step);

	void interpolate(uint16_t startLine, uint16_t endLine, uint16_t step);

	void join();

	virtual ~InterpolationThread();

	const CcdData& _ccdData;
	Interpolator& _interpolator;
	Pixmap<uint16_t>& _pixmap;
	InterpolationController& _controller;
	uint16_t _startLine;
	uint16_t _endLine;
	uint16_t _step;

private:
	InterpolationThread();

	pthread_t _thread;
};

#endif
