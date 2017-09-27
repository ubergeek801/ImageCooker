#include <iostream>

#include "CcdData.h"
#include "GenericException.h"
#include "InterpolationController.h"
#include "InterpolatorFactory.h"
#include "Pixmap.h"
#include "RawDecoder.h"

int main(int argc, char* argv[]) {
	// Make sure we were called with valid arguments. Currently we
	// require exactly three arguments--the name of the MRW file to be
	// processed, the name of the PPM file to be produced, and the name
	// of the Bayer interpolator to be used.
	if (argc != 4) {
		cerr << "Usage: " << argv[0] << " <MRW file> <PPM file> "
				"<interpolator>" << endl;
		return -1;
	}

	try {
		// initialize the RawDecoder
		RawDecoder* rawDecoder = new RawDecoder(argv[1]);

		CcdData* ccdData = rawDecoder->createCcdData();
		// we don't need the RawDecoder anymore
		delete rawDecoder;
		Interpolator* interpolator = InterpolatorFactory::
				create(argv[3]);
		InterpolationController controller(interpolator);
		Pixmap<uint16_t>* pixmap = controller.interpolate(*ccdData);
		delete ccdData;
		pixmap->writePpm(argv[2]);
		delete pixmap;
		delete interpolator;
	} catch (GenericException& e) {
		cerr << "Exception: " << e.getCause() << endl;
		return -1;
	}

	// clean up and exit
	return 0;
}
