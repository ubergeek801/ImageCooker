#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string>

using namespace std;

int main(int argc, char** argv) {
	ifstream inputFile(argv[1], ifstream::in);
	ofstream outputFile(argv[2], ofstream::out | ofstream::trunc);
	string ignore;
	inputFile >> ignore; // version
	unsigned short width, height;
	string widthString, heightString;
	inputFile >> widthString;
	inputFile >> heightString;
	inputFile >> ignore; // max value
	cout << "dimensions = " << widthString << "x" << heightString <<
			endl;
	width = atoi(widthString.c_str());
	height = atoi(heightString.c_str());
	outputFile << "P3" << endl << width << " " << height << endl <<
			"255" << endl;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			string redValue, greenValue, blueValue;
			inputFile >> redValue;
			inputFile >> greenValue;
			inputFile >> blueValue;
			if (y & 1) {
				if (x & 1) {
					// blue
					outputFile << "0" << endl;
					outputFile << "0" << endl;
					outputFile << blueValue << endl;
				} else {
					// green
					outputFile << "0" << endl;
					outputFile << greenValue << endl;
					outputFile << "0" << endl;
				}
			} else if (x & 1) {
				// green
				outputFile << "0" << endl;
				outputFile << greenValue << endl;
				outputFile << "0" << endl;
			} else {
				// red
				outputFile << redValue << endl;
				outputFile << "0" << endl;
				outputFile << "0" << endl;
			}
		}
	}
	inputFile.close();
	outputFile.close();
}
