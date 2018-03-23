includes = -I/usr/include/glib-1.2 -I/usr/lib/glib/include

info_objects = typereader.o MrmBlock.o PadBlock.o PrdBlock.o \
		RifBlock.o TtwBlock.o WbgBlock.o CcdData.o RawDecoder.o \
		BayerPpmDecoder.o

all_objects = $(info_objects) InterpolatorFactory.o \
		InterpolationController.o InterpolationThread.o Interpolator.o \
		NoInterpolator.o NearestNeighborInterpolator.o \
		BilinearInterpolator.o SmoothHueTransitionInterpolator.o \
		SmoothHueTransitionLogInterpolator.o \
		EdgeSensing1Interpolator.o EdgeSensing2Interpolator.o \
		ColorCorrection1Interpolator.o VariableGradientsInterpolator.o \
		PatternRecognitionInterpolator.o PixelGroupingInterpolator.o

tiff_objects = TiffWriter.o

tiff_libs = -ltiff

gimp_libs = -lgimp

libs = -lpthread

CPPFLAGS = $(includes) -D_REENTRANT -O3 -march=pentium3 -g

all: mrwinfo mrw2ppm mrw2tiff bayer2tiff bayerize gimp_mrw

mrwinfo: $(info_objects) mrwinfo_main.o
	g++ -o mrwinfo $(info_objects) mrwinfo_main.o

mrw2ppm: $(all_objects) mrw2ppm_main.o
	g++ -o mrw2ppm $(all_objects) mrw2ppm_main.o $(libs)

mrw2tiff: $(all_objects) $(tiff_objects) mrw2tiff_main.o
	g++ -o mrw2tiff $(all_objects) $(tiff_objects) mrw2tiff_main.o \
			$(tiff_libs) $(libs)

bayer2tiff: $(all_objects) $(tiff_objects) bayer2tiff_main.o
	g++ -o bayer2tiff $(all_objects) $(tiff_objects) bayer2tiff_main.o \
			$(tiff_libs) $(libs)

bayerize: $(info_objects) bayerize_main.o
	g++ -o bayerize $(info_objects) bayerize_main.o

gimp_mrw: $(all_objects) gimp_mrw.o
	g++ -o gimp_mrw $(all_objects) gimp_mrw.o $(gimp_libs) $(libs)

# cleanup
.PHONY: clean
clean:
	-rm mrwinfo mrw2ppm mrw2tiff bayer2tiff bayerize gimp_mrw mrwinfo_main.o \
			mrw2ppm_main.o mrw2tiff_main.o gimp_mrw.o bayer2tiff_main.o \
			bayerize_main.o $(all_objects) $(tiff_objects)
