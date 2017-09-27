#include <string.h>
#include <libgimp/gimp.h>

#include "CcdData.h"
#include "GenericException.h"
#include "InterpolationController.h"
#include "InterpolatorFactory.h"
#include "Pixmap.h"
#include "ProgressListener.h"
#include "RawDecoder.h"

#define PLUGIN_NAME "Load MRW"
#define PLUGIN_BLURB "Load Minolta MRW raw file"
#define PLUGIN_HELP "Load Minolta MRW raw file"
#define PLUGIN_AUTHOR "Jeremy Rosenberger"
#define PLUGIN_COPYRIGHT "Jeremy Rosenberger"
#define PLUGIN_DATE "2016"
#define PLUGIN_MENU_PATH "<Load>/Minolta MRW"

#define INTERPOLATOR "PixelGrouping"

class GimpProgressListener : public ProgressListener {
public:
	GimpProgressListener(unsigned int numPhases) : _numPhases(numPhases),
   			_basePercentage(0.0), _nextReportedProgress(1) {
			_phasePercentage = 1.0 / (double)numPhases; }

	void updateProgress(double percentage) {
		unsigned short progress = (unsigned short)(_basePercentage +
				(percentage * 100.0) * _phasePercentage);
		if (progress >= _nextReportedProgress) {
			gimp_progress_update((double)progress / 100.0);
			_nextReportedProgress++;
		}
	}

	void setPhase(unsigned int phase) { _basePercentage = (double)phase *
			_phasePercentage * 100.0; }

	~GimpProgressListener() { }

private:
	unsigned int _numPhases;
	double _phasePercentage;
	double _basePercentage;
	unsigned short _nextReportedProgress;
};

gint32 loadMrwImage(gchar* filename) {
	char* statusString = g_strdup_printf("Loading %s", filename);
	gimp_progress_init(statusString);
	g_free(statusString);
	GimpProgressListener progressListener(3);
	try {
		// read and decode the file
		RawDecoder* rawDecoder = new RawDecoder(filename);
		rawDecoder->setProgressListener(&progressListener);
		CcdData* ccdData = rawDecoder->createCcdData();
		// we don't need the RawDecoder anymore
		delete rawDecoder;
		Interpolator* interpolator = InterpolatorFactory::
				create(INTERPOLATOR);
		InterpolationController controller(interpolator);
		progressListener.setPhase(1);
		controller.setProgressListener(&progressListener);
		Pixmap<uint16_t>* pixmap = controller.interpolate(*ccdData);
		// we don't need the raw data or the interpolator anymore
		delete ccdData;
		delete interpolator;
		// pixmap now contains the pixmap information in a fairly
		// straightforward format, so convert it to something the GIMP
		// can use...

		// keep in mind that 4 pixels on each side are reserved for
		// interpolation and don't belong in the final image, so the
		// size is 8 pixels smaller in each dimension
		gint32 imageId = gimp_image_new(pixmap->getWidth() - 8, pixmap->
				getHeight() - 8, GIMP_RGB);
		char* filenameString = strdup(filename);
		gimp_image_set_filename(imageId, filenameString);
		g_free(filenameString);
		gint32 layerId = gimp_layer_new(imageId, "Background", pixmap->
				getWidth() - 8, pixmap->getHeight() - 8, GIMP_RGB_IMAGE,
				100, GIMP_NORMAL_MODE);
		gimp_image_add_layer(imageId, layerId, 0);
		GimpDrawable* drawable = gimp_drawable_get(layerId);
		GimpPixelRgn pixelRegion;
		gimp_pixel_rgn_init(&pixelRegion, drawable, 0, 0, drawable->
				width, drawable->height, TRUE, TRUE);
		progressListener.setPhase(2);
		guint numTiles = drawable->ntile_rows * drawable->ntile_cols;
		guint tileNum = 1;
		// Here we employ the "Tile Iterator" pattern: The following for
		// loop iterates over all tiles in the image. Upon each
		// iteration the pixel region data for the current tile is
		// copied into pixelRegion
		for (gpointer tilePointer = gimp_pixel_rgns_register(1,
				&pixelRegion); tilePointer != NULL; tilePointer =
				gimp_pixel_rgns_process(tilePointer), tileNum++) {
			// the buffer is the width * height * 3 bytes per pixel
			guchar* buffer = g_new(guchar, pixelRegion.w * pixelRegion.
					h * 3);
			if (buffer == NULL) {
				throw GenericException("Unable to allocate memory");
			}
			RgbValue<uint16_t> rgb;
			for (gint y = 0; y < pixelRegion.h; y++) {
				for (gint x = 0; x < pixelRegion.w ; x++) {
					// here's that 4-pixel border thing again; we adjust
					// the coordinates by 4
					pixmap->getRgbValue(x + pixelRegion.x + 4, y +
							pixelRegion.y + 4, rgb);
					// convert the 12-bit RGB values to 8-bit
					guchar red = rgb.getRed() >> 4;
					guchar green = rgb.getGreen() >> 4;
					guchar blue = rgb.getBlue() >> 4;
					gint offset = (y * pixelRegion.w + x) * 3;
					buffer[offset] = red;
					buffer[offset + 1] = green;
					buffer[offset + 2] = blue;
				}
			}
			gimp_pixel_rgn_set_rect(&pixelRegion, buffer, pixelRegion.x,
					pixelRegion.y, pixelRegion.w, pixelRegion.h);
			g_free(buffer);
			progressListener.updateProgress((double)tileNum / (double)
					numTiles);
		}
		// all this is necessary for GIMP cleanup
		gimp_drawable_flush(drawable);
		gimp_drawable_merge_shadow(drawable->drawable_id, FALSE);
		gimp_drawable_update(drawable->drawable_id, 0, 0, drawable->width,
				drawable->height);
		gimp_displays_flush();
		gimp_drawable_detach(drawable);

		// we don't need the pixmap anymore
		delete pixmap;
		return imageId;
	} catch (GenericException& e) {
		g_message(e.getCause().data());
	}
	return -1;
}

// method prototypes
static void init();

static void quit();

static void query();

static void run(const gchar* name, gint nparams, const GimpParam* params, gint*
		nreturn_vals, GimpParam** return_vals);

// GIMP plugins are expected to have a global PLUG_IN_INFO which points
// to the various methods implemented by this plugin 
GimpPlugInInfo PLUG_IN_INFO = {
	init,
	quit,
	query,
	run
};

static void init() {
	// we don't have any initialization to do
}

static void quit() {
	// we don't have any cleanup to do
}

static void query() {
	static GimpParamDef loadArgs[] = {
		// these parameters are required of <Load> plugins
		{ GIMP_PDB_INT32, (gchar*)"run_mode", (gchar*)"Interactive/Noninteractive" },
		{ GIMP_PDB_STRING, (gchar*)"filename", (gchar*)"The name of the file to load" },
		{ GIMP_PDB_STRING, (gchar*)"raw_filename", (gchar*)"The raw name of the file to"
				" load" }
	};
	static GimpParamDef loadReturnVals[] = {
		// we only return an image... makes sense huh?
		{ GIMP_PDB_IMAGE, (gchar*)"image", (gchar*)"Output image" }
	};
	// GIMP plugins are expected to call gimp_install_procedure
	gimp_install_procedure(PLUGIN_NAME, PLUGIN_BLURB, PLUGIN_HELP,
			PLUGIN_AUTHOR, PLUGIN_COPYRIGHT, PLUGIN_DATE,
			PLUGIN_MENU_PATH, (char*)NULL, GIMP_PLUGIN, 3, 1, loadArgs,
			loadReturnVals);
	// apparently this can be called to register with the load mechanism
	gimp_register_magic_load_handler(PLUGIN_NAME, "mrw,MRW", "",
			"1,string,MRM");
}

static void run(const gchar* name, gint nparams, const GimpParam* params, gint*
		nreturn_vals, GimpParam** return_vals) {
	// set up the return value so that if we don't explicitly change it
	// later, it will be considered a calling error
	static GimpParam returnVals[2];
	*nreturn_vals = 1;
	*return_vals = returnVals;
	returnVals[0].type = GIMP_PDB_STATUS;
	returnVals[0].data.d_status = GIMP_PDB_CALLING_ERROR;
	// if we don't have exactly three arguments, then there is some sort
	// of problem
	if (nparams != 3) {
		return;
	}
	// The second argument should be the filename. Verify that it is at
	// least a string.
	if (params[1].type != GIMP_PDB_STRING) {
		return;
	}
	gchar* filename = params[1].data.d_string;
	// beyond this point an error is considered an execution error
	returnVals[0].data.d_status = GIMP_PDB_EXECUTION_ERROR;

	gint32 imageId = loadMrwImage(filename);
	if (imageId == -1) {
		return;
	}

	// if we made it this far, we've successfully created an image, so
	// add it to the return values, set the status to ok, and get out
	*nreturn_vals = 2;
	returnVals[0].data.d_status = GIMP_PDB_SUCCESS;
	returnVals[1].type = GIMP_PDB_IMAGE;
	returnVals[1].data.d_image = imageId;
}

// required for GIMP plugins
MAIN()
