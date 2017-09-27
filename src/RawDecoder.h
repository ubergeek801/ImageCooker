#ifndef _RAWDECODER_H
#define _RAWDECODER_H

#include <string>

#include "CcdData.h"
#include "MrmBlock.h"
#include "ProgressListener.h"

using namespace std;

class RawDecoder {
private:
	char* _ccdStreamData;

public:
	RawDecoder(string rawFilename);

	MrmBlock& getMrmBlock() const { return *_mrmBlock; }

	void setProgressListener(ProgressListener* listener) {
			_progressListener = listener; }

	CcdData* createCcdData() const;

	void dumpRawInfo() const;

	virtual ~RawDecoder();

private:
	RawDecoder();

	MrmBlock* _mrmBlock;

	ProgressListener* _progressListener;
};

#endif
