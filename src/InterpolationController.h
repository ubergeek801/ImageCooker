#ifndef _INTERPOLATIONCONTROLLER_H
#define _INTERPOLATIONCONTROLLER_H

#include <pthread.h>

#include "CcdData.h"
#include "Interpolator.h"
#include "Pixmap.h"
#include "ProgressListener.h"
#include "types.h"

class InterpolationController {
public:
	InterpolationController(Interpolator* interpolator);

	Pixmap<uint16_t>* interpolate(const CcdData& ccdData);

	void setProgressListener(ProgressListener* listener) {
			_progressListener = listener; }

	void incrementScanLine(uint16_t scanLine);

	virtual ~InterpolationController();

private:
	InterpolationController();

	Interpolator* _interpolator;

	ProgressListener* _progressListener;

	long _numProcessors;

	pthread_mutex_t _scanLineMutex;
	uint16_t _totalScanLines;
	uint16_t _scanLinesCompleted;
};

#endif
