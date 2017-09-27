#include <iostream>

#include "CcdData.h"
#include "GenericException.h"
#include "InterpolationController.h"
#include "InterpolatorFactory.h"
#include "Pixmap.h"
#include "BayerPpmDecoder.h"
#include "TiffWriter.h"

int main(int argc, char* argv[]) {
	// Make sure we were called with valid arguments. Currently we
	// require exactly three argument--the name of the Bayerized PPM
	// file to be processed, the name of the TIFF file to be produced,
	// and the name of the Bayer interpolator to be used.
	if (argc != 4) {
		cerr << "Usage: " << argv[0] << " <Bayer PPM file> <TIFF file> "
				"<interpolator>" << endl;
		cerr << endl << "Recognized interpolators:" << endl;
		string* interpolatorNames;
	   	uint8_t numInterpolators = InterpolatorFactory::
				getInterpolatorNames(&interpolatorNames);
		for (int i = 0; i < numInterpolators; i++) {
			cerr << "  " << interpolatorNames[i] << endl;
		}
		return -1;
	}

	bool writeSuccess = false;
	try {
		// initialize the BayerPpmDecoder
		BayerPpmDecoder* rawDecoder = new BayerPpmDecoder(argv[1]);

		CcdData* ccdData = rawDecoder->createCcdData();
		// we don't need the BayerPpmDecoder anymore
		delete rawDecoder;
		Interpolator* interpolator = InterpolatorFactory::
				create(argv[3]);
		InterpolationController controller(interpolator);
		Pixmap<uint16_t>* pixmap = controller.interpolate(*ccdData);
		delete ccdData;
		delete interpolator;
		TiffWriter tiffWriter;
		writeSuccess = tiffWriter.write(*pixmap, argv[2], 8);
		delete pixmap;
	} catch (GenericException& e) {
		cerr << "Exception: " << e.getCause() << endl;
		return -1;
	}

	// clean up and exit
	if (!writeSuccess) {
		cout << "Unable to write TIFF file" << endl;
		return 1;
	}
	return 0;
}
