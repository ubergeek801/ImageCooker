#include <iostream>

#include "GenericException.h"
#include "RawDecoder.h"

int main(int argc, char* argv[]) {
	// Make sure we were called with valid arguments. Currently we
	// require exactly one argument--the name of the MRW file to be
	// processed.
	if (argc != 2) {
		cerr << "Usage: " << argv[0] << " <filename>" << endl;
		return -1;
	}

	try {
		// initialize the RawDecoder
		RawDecoder rawDecoder(argv[1]);

		// tell the RawDecoder to dump its contents
		rawDecoder.dumpRawInfo();
	} catch (GenericException& e) {
		cerr << "Exception: " << e.getCause() << endl;
		return -1;
	}

	// clean up and exit
	return 0;
}
