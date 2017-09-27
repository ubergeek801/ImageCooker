#ifndef _BAYERPPMDECODER_H
#define _BAYERPPMDECODER_H

#include <string>

#include "CcdData.h"
#include "MrmBlock.h"
#include "ProgressListener.h"

using namespace std;

class BayerPpmDecoder {
private:
	string _ppmFilename;

public:
	BayerPpmDecoder(string ppmFilename);

	CcdData* createCcdData() const;

	virtual ~BayerPpmDecoder();

private:
	BayerPpmDecoder();
};

#endif
