#ifndef _TIFFWRITER_H
#define _TIFFWRITER_H

#include <stdint.h>
#include <string>

#include "Pixmap.h"

class TiffWriter {

public:
	TiffWriter();

	bool write(const Pixmap<uint16_t>& pixmap, const string filename,
			uint8_t bpp) const;

	~TiffWriter(); 
};

#endif
