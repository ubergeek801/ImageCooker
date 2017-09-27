#include "TiffWriter.h"

#include <tiffio.h>

#include "GenericException.h"

TiffWriter::TiffWriter() {
}

bool TiffWriter::write(const Pixmap<uint16>& pixmap, const string
		filename, uint8 bpp) const {
	if (bpp != 8 && bpp != 16) {
		throw GenericException("bits per pixel must be 8 or 16");
	}
	TIFF* tiff = TIFFOpen(filename.c_str(), "w");
	if (tiff == NULL) {
		// the TIFF library will have printed the details of the error
		// to stderr, so there's not much to do except exit
		return false;
	}

	// recall that we reserve 4 pixels on each side for the "border"
	// (these are not necessarily interpolated, so their values may not
	// be valid), meaning the final image will be 8 pixels smaller in
	// each dimension
	uint32 width = pixmap.getWidth() - 8;
	uint32 height = pixmap.getHeight() - 8;
	// this is pretty much standard TIFF stuff for an RGB image
	TIFFSetField(tiff, TIFFTAG_IMAGEWIDTH, width);
	TIFFSetField(tiff, TIFFTAG_IMAGELENGTH, height);
	TIFFSetField(tiff, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
	TIFFSetField(tiff, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
	TIFFSetField(tiff, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
	TIFFSetField(tiff, TIFFTAG_ROWSPERSTRIP, 1);
	TIFFSetField(tiff, TIFFTAG_BITSPERSAMPLE, bpp);
	TIFFSetField(tiff, TIFFTAG_SAMPLESPERPIXEL, 3);
	// This is a pretty awful way of dealing with 8bpp and 16bpp, but
	// I'll come up with a reasonable solution later. In either case,
	// each strip corresponds to a single scan line, encoded as
	// RGBRGBRGB...
	if (bpp == 8) {
		uint8 strip[width * 3];
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				strip[x * 3] = pixmap.getRedValue(x + 4, y + 4) >> 4;
				strip[x * 3 + 1] = pixmap.getGreenValue(x + 4, y + 4) >>
						4;
				strip[x * 3 + 2] = pixmap.getBlueValue(x + 4, y + 4) >>
						4;
			}
			if (TIFFWriteEncodedStrip(tiff, y, strip, width * 3) == -1)
					{
				return false;
			}
		}
	} else if (bpp == 16) {
		uint16 strip[width * 3];
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				strip[x * 3] = pixmap.getRedValue(x + 4, y + 4) << 4;
				strip[x * 3 + 1] = pixmap.getGreenValue(x + 4, y + 4) <<
						4;
				strip[x * 3 + 2] = pixmap.getBlueValue(x + 4, y + 4) <<
						4;
			}
			if (TIFFWriteEncodedStrip(tiff, y, strip, width * 3 * 2) ==
					-1) {
				return false;
			}
		}
	}

	// we're all done
	TIFFClose(tiff);
	return true;
}

TiffWriter::~TiffWriter() {
}
