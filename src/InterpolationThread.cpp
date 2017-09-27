#include <pthread.h>

#include "InterpolationThread.h"

#include "GenericException.h"
#include "RgbValue.h"

static void* startInterpolateGreen(void* interpolationThread) {
	InterpolationThread* thread = (InterpolationThread*)
			interpolationThread;

	uint16_t width = thread->_ccdData.getWidth();

	for (uint16_t y = thread->_startLine; y <= thread->_endLine; y +=
			thread->_step) {
		for (uint16_t x = 2; x < width - 2; x++) {
			thread->_pixmap.setGreenValue(x, y, thread->_interpolator.
					interpolateGreen(x, y, thread->_ccdData));
		}
		thread->_controller.incrementScanLine(y);
	}

	return NULL;
}

static void* startInterpolate(void* interpolationThread) {
	InterpolationThread* thread = (InterpolationThread*)
			interpolationThread;

	RgbValue<uint16_t> rgb;
	uint16_t width = thread->_ccdData.getWidth();

	for (uint16_t y = thread->_startLine; y <= thread->_endLine; y +=
			thread->_step) {
		for (int x = 4; x < width - 4; x++) {
			thread->_interpolator.interpolate(x, y, thread->_ccdData,
					thread->_pixmap, rgb);
			thread->_pixmap.setRgbValue(x, y, rgb);
		}
		thread->_controller.incrementScanLine(y);
	}

	return NULL;
}

InterpolationThread::InterpolationThread(const CcdData& ccdData,
		Interpolator& interpolator, Pixmap<uint16_t>& pixmap,
		InterpolationController& controller) : _ccdData(ccdData),
		_interpolator(interpolator), _pixmap(pixmap),
		_controller(controller) {
}

void InterpolationThread::interpolateGreen(uint16_t startLine, uint16_t
		endLine, uint16_t step) {
	_startLine = startLine;
	_endLine = endLine;
	_step = step;

	if (pthread_create(&_thread, NULL, &startInterpolateGreen, this) !=
			0) {
		throw GenericException("Could not create thread");
	}
}

void InterpolationThread::interpolate(uint16_t startLine, uint16_t endLine,
		uint16_t step) {
	_startLine = startLine;
	_endLine = endLine;
	_step = step;

	if (pthread_create(&_thread, NULL, &startInterpolate, this) != 0) {
		throw GenericException("Could not create thread");
	}
}

void InterpolationThread::join() {
	if (pthread_join(_thread, NULL) != 0) {
		throw GenericException("Could not join thread");
	}
}

InterpolationThread::~InterpolationThread() {
}
