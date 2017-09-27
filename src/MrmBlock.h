#ifndef _MRMBLOCK_H
#define _MRMBLOCK_H

#include <fstream>
#include <stdint.h>

#include "types.h"

#include "PrdBlock.h"
#include "TtwBlock.h"
#include "WbgBlock.h"
#include "RifBlock.h"
#include "PadBlock.h"

using namespace std;

class MrmBlock {
private:
	char _name[4];
	uint32_t _length;

	PrdBlock* _prdBlock;
	TtwBlock* _ttwBlock;
	WbgBlock* _wbgBlock;
	RifBlock* _rifBlock;
	PadBlock* _padBlock;

public:
	MrmBlock(ifstream& mrwFile);

	const char* getName() const { return _name; }

	uint32_t getLength() const { return _length; }

	PrdBlock& getPrdBlock() const { return *_prdBlock; }

	TtwBlock& getTtwBlock() const { return *_ttwBlock; }

	WbgBlock& getWbgBlock() const { return *_wbgBlock; }

	RifBlock& getRifBlock() const { return *_rifBlock; }

	PadBlock& getPadBlock() const { return *_padBlock; }

	void dumpRawInfo() const;

	virtual ~MrmBlock();

private:
	MrmBlock();
};

#endif
