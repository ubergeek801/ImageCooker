#include <unistd.h>

#include "InterpolationController.h"
#include "InterpolationThread.h"

#include "RgbValue.h"

InterpolationController::InterpolationController(Interpolator*
		interpolator) : _interpolator(interpolator),
		_progressListener(NULL) {
	// for some reason we can't initialize a member variable using
	// PTHREAD_MUTEX_INITIALIZER, so we do this instead
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	_scanLineMutex = mutex;

	_numProcessors = sysconf(_SC_NPROCESSORS_ONLN);
	if (_numProcessors <= 0) {
		// there was an error getting the number of processors online,
		// so just assume 1
		_numProcessors = 1;
	}
}

Pixmap<uint16_t>* InterpolationController::interpolate(const CcdData&
		ccdData) {
	_scanLinesCompleted = 0;
	uint16_t width = ccdData.getWidth();
	uint16_t height = ccdData.getHeight();
	Pixmap<uint16_t>* pixmap = new Pixmap<uint16_t>(width, height);

	bool doInterpolateGreen = _interpolator->doInterpolateGreen();
	// If we are doing green interpolation separately, then we have a
	// total of height - 4 (we interpolate green for all but the top 2
	// and bottom 2 rows) plus height - 8 (we interpolate everything
	// for all but the top 4 and bottom 4 rows). If we're not doing
	// green interpolation separately, then we have just the normal
	// height - 8 scan lines to interpolate.
	_totalScanLines = doInterpolateGreen ? (height - 4) + (height - 8) :
			height - 8;

	// Set up the interpolator threads. It seems reasonable to create
	// one thread per CPU.
	InterpolationThread** threads = new InterpolationThread*
			[_numProcessors];
	for (int i = 0; i < _numProcessors; i++) {
		threads[i] = new InterpolationThread(ccdData, *_interpolator,
				*pixmap, *this);
	}

	// if the Interpolator is the type that needs to interpolate green
	// values first, then do that (using only a 2-pixel border on each
	// side)
	if (doInterpolateGreen) {
		// Each thread is assigned 1/n of the scan lines to be
		// processed, where n is the number of CPUs. In case of rounding
		// errors, the nth thread always gets an endLine of the last
		// scan line.
		uint16_t scanLinesPerThread = (uint16_t)((double)(height - 4) /
				(double)_numProcessors);
		for (int i = 0; i < _numProcessors; i++) {
			uint16_t startLine = i * scanLinesPerThread + 2;
			uint16_t endLine = (i == _numProcessors - 1 ? height - 3 :
					startLine + scanLinesPerThread - 1);
			threads[i]->interpolateGreen(startLine, endLine, 1);
		}
		for (int i = 0; i < _numProcessors; i++) {
			threads[i]->join();
		}
		_interpolator->postInterpolateGreen(ccdData, *pixmap);
	}

	RgbValue<uint16_t> rgb;
	// the actual interpolation takes place only in the "viewable area"
	// of the pixmap (4-pixel border on each side)
	uint16_t scanLinesPerThread = (uint16_t)((double)(height - 8) /
			(double)_numProcessors);
	for (int i = 0; i < _numProcessors; i++) {
		uint16_t startLine = i * scanLinesPerThread + 4;
		uint16_t endLine = (i == _numProcessors - 1 ? height - 5 :
				startLine + scanLinesPerThread - 1);
		threads[i]->interpolate(startLine, endLine, 1);
	}
	for (int i = 0; i < _numProcessors; i++) {
		threads[i]->join();
	}
	_interpolator->postInterpolate(ccdData, *pixmap);

	// cleanup
	for (int i = 0; i < _numProcessors; i++) {
		delete threads[i];
	}
	delete threads;

	return pixmap;
}

void InterpolationController::incrementScanLine(uint16_t scanLine) {
	if (_progressListener != NULL) {
		pthread_mutex_lock(&_scanLineMutex);
		_scanLinesCompleted++;
		_progressListener->updateProgress((double)_scanLinesCompleted /
				(double)_totalScanLines);
		pthread_mutex_unlock(&_scanLineMutex);
	}
}

InterpolationController::~InterpolationController() {
	pthread_mutex_destroy(&_scanLineMutex);
}
