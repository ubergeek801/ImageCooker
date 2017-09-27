#ifndef _PADBLOCK_H
#define _PADBLOCK_H

#include <fstream>
#include <stdint.h>

#include "types.h"

using namespace std;

class PadBlock {
private:
	char _name[4];
	uint32_t _length;

public:
	PadBlock(ifstream& mrwFile, int offset);

	const char* getName() const { return _name; }

	uint32_t getLength() const { return _length; }

	void dumpRawInfo() const;

private:
	PadBlock();
};

#endif
