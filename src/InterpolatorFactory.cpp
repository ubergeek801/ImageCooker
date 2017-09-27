#include "GenericException.h"
#include "InterpolatorFactory.h"

#include "BilinearInterpolator.h"
#include "ColorCorrection1Interpolator.h"
#include "PixelGroupingInterpolator.h"
#include "EdgeSensing1Interpolator.h"
#include "EdgeSensing2Interpolator.h"
#include "NearestNeighborInterpolator.h"
#include "NoInterpolator.h"
#include "PatternRecognitionInterpolator.h"
#include "SmoothHueTransitionInterpolator.h"
#include "SmoothHueTransitionLogInterpolator.h"
#include "VariableGradientsInterpolator.h"

static string interpolatorNames[] = {
	"Bilinear",
	"ColorCorrection1",
	"EdgeSensing1",
	"EdgeSensing2",
	"NearestNeighbor",
	"None",
	"PatternRecognition",
	"PixelGrouping",
	"SmoothHueTransition",
	"SmoothHueTransitionLog",
	"VariableGradients"
};

InterpolatorFactory::InterpolatorFactory() {
}

Interpolator* InterpolatorFactory::create(string name) {
	if (name == "" || name == "default") {
		// Pixel Grouping is the default
		return new PixelGroupingInterpolator();
	} else if (name == "Bilinear") {
		return new BilinearInterpolator();
	} else if (name == "ColorCorrection1") {
		return new ColorCorrection1Interpolator();
	} else if (name == "EdgeSensing1") {
		return new EdgeSensing1Interpolator();
	} else if (name == "EdgeSensing2") {
		return new EdgeSensing2Interpolator();
	} else if (name == "NearestNeighbor") {
		return new NearestNeighborInterpolator();
	} else if (name == "No" || name == "None") {
		return new NoInterpolator();
	} else if (name == "PatternRecognition") {
		return new PatternRecognitionInterpolator();
	} else if (name == "PixelGrouping") {
		return new PixelGroupingInterpolator();
	} else if (name == "SmoothHueTransition") {
		return new SmoothHueTransitionInterpolator();
	} else if (name == "SmoothHueTransitionLog") {
		return new SmoothHueTransitionLogInterpolator();
	} else if (name == "VariableGradients") {
		return new VariableGradientsInterpolator();
	} else {
		throw GenericException("Unknown interpolator type: " + name);
	}
}

uint8_t InterpolatorFactory::getInterpolatorNames(string** nameBuffer) {
	*nameBuffer = interpolatorNames;
	return 11;
}

InterpolatorFactory::~InterpolatorFactory() {
}
