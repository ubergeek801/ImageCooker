#ifndef _VARIABLEGRADIENTSINTERPOLATOR_H
#define _VARIABLEGRADIENTSINTERPOLATOR_H

#include "Interpolator.h"

class Gradients;

class VariableGradientsInterpolator : public Interpolator {

public:
	VariableGradientsInterpolator();

	virtual void interpolate(uint16_t x, uint16_t y, const CcdData& ccdData,
			const Pixmap<uint16_t>& pixmap, RgbValue<uint16_t>& rgb);

	virtual ~VariableGradientsInterpolator();

protected:
	bool processGradients(uint16_t x, uint16_t y, bool useGradientsForGreen,
			RgbValue<uint16_t>& rgb, const CcdData& ccdData, long& rSum,
			long& gSum, long& bSum, int& numRegions) const;

	void interpolateRedAndBlueAtGreen(uint16_t x, uint16_t y,
			RgbValue<uint16_t>& rgb, const CcdData& ccdData) const;

	void interpolateRedAndGreenAtBlue(uint16_t x, uint16_t y,
			RgbValue<uint16_t>& rgb, const CcdData& ccdData) const;

	void interpolateGreenAndBlueAtRed(uint16_t x, uint16_t y,
			RgbValue<uint16_t>& rgb, const CcdData& ccdData) const;

	void getGradientsForNonGreen(uint16_t x, uint16_t y, Gradients&
			gradients, const CcdData& ccdData) const;

	void getGradientsForGreen(uint16_t x, uint16_t y, Gradients& gradients,
			const CcdData& ccdData) const;

private:
	double k1, k2;
};

#endif
