# ImageCooker
ImageCooker is a set of open source utilities for processing "raw" image files from digital cameras using Bayer pattern sensors, implemented in C++.

ImageCooker's "front end" is based on the defunct Minolta MRW format, which is not likely to be useful to too many people anymore. However, its processing engine was designed to facilitate developing new "de-mosaic" algorithms, and it has indeed been used for this purpose by a handful of researchers who stumbled upon this project.

I'm not going to lie; ImageCooker hasn't really been actively maintained since 2003. But since I still encounter the occasional individual that finds it useful, I decided that rather than living as a ZIP file on an obscure Web site somewhere, it would find a better home on GitHub. And since I'm now in possession of a Fuji camera eqiupped with an "X-Trans" sensor, an X-Trans version of ImageCooker may eventually come along (but no promises!).

## ImageCooker utilities
Currently there are four utilities for use with Minolta Raw (MRW) files:
* `mrwinfo`: extracts and displays the metadata (camera model, aperture, shutter speed, white balance mode, etc.) of a MRW file
* `mrw2ppm`: interpolates the raw sensor data contained in a MRW file and writes the output in PPM format
* `mrw2tiff`: interpolates the raw sensor data contained in a MRW file and writes the output in TIFF format
* `gimp_mrw`: a GIMP plugin that creates a new image by interpolating the raw sensor data contained in a MRW file

In addition, there are two utilities that perform Bayer-related operations on PPM files (these are useful for experimentation but not much else):
* `bayerize`: "Bayerizes" a PPM file by stripping color data to leave only the information which would be recorded by a Bayer sensor (that is, each pixel in the output image contains only a red, green or blue value, arranged in a Bayer pattern); writes the output in PPM format
* `bayer2tiff`: performs Bayer interpolation on a PPM file, treating the image as if it came from a Bayer sensor, and writes the output in TIFF format

## Why another raw processor?
Even in 2003, there was no shortage of existing apps, even in the open source world, to handle raw images. Many of them were even more full-featured. But I had a few specific goals in mind when developing ImageCooker. I wanted to
* learn the ins and outs of writing certain categories of software that I haven't dabbled in before: GIMP plugins and processing of device-specific output files;
* play around with (by implementing myself) different Bayer interpolation algorithms; and
* produce such a set of utilities with the explicit goal of producing source code that is well-documented enough to be understandable to others (at least to others with knowledge of C++ and/or object-oriented programming).

The Bayer interpolation algorithms I chose to implement were (most of) those described by Ting Chen in his "Study of Spatial Color Interpolation Algorithms for Single-Detector Digital Cameras" (long gone, but accessible on the [Wayback Machine](https://web.archive.org/web/20060821210110/http://www-ise.stanford.edu/~tingchen/)). A number of these (e.g. Nearest Neighbor, Bilinear) aren't particularly sophisticated and probably wouldn't be used in the real world (at least for still image applications), but I implemented them for completeness (part of my purpose, after all, was to play around).

For fun, I added a "No Interpolator" algorithm which doesn't interpolate color values at all, but instead produces each pixel in the output image having only the red, green or blue color component that that corresponds to the (single) color of the corresponding pixel in the camera sensor. It doesn't produce particularly useful images, but when zoomed in, such images reveal exactly what is "seen" by the Bayer sensor.

## ImageCooker limitations
There are a lot. Here are some that come to mind:
* No color space conversion of any kind is performed. That is, the utilities have no knowledge of ICC profiles, sRGB, AdobeRGB, and that sort of thing. In fact, the utilities don't do anything other than interpolate (except for white balance correction); gamma correction, sharpening, etc., if desired, must be done in post processing. Because of this, images from ImageCooker will appear dark and will require some sort of levels adjustment.
* Even though the Minolta DiMAGE cameras recorded 12 bits of color information, the GIMP plugin and mrw2ppm always output 8-bit color. mrw2tiff is capable of producing 8-bit or 16-bit color, although currently the value can only be changed by editing the source code and recompiling. In all cases, internal calculations use all 12 bits of color information; the data is only truncated to 8 bits in the final output stage.
* EXIF tags in the MRW file are not processed. (The MRW file stores some metadata in the file structure itself; other metadata is stored as embedded EXIF tags.) In any case, there's no way to embed this information into a PPM file or into a GIMP image anyway, but the mrw2tiff utility could eventually implement EXIF tags.
* There is no way to override the white balance settings; whatever is encoded in the MRW file is what will be used.
* The GIMP plugin has no configurability at all; it is hard-coded to use the "Pixel Grouping" algorithm (currently the best all-around interpolator available in ImageCooker).
* Very little optimization has been attempted. I have tried to code the interpolation strategies as efficiently as is (naively) possible, but I'm sure there's a whole lot that could be optimized. In particular, the memory requirements are probably higher than they need to be. (At least the GIMP plugin uses tiles for a modicum of efficiency.) However, one important optimization that has been made is that the interpolation routines can take advantage of multiple CPUs. *(Hey, that was pretty impressive in 2003!)*
* Some of the code isn't as "generic" as it could be. Specifically, some of the code makes assumptions that color samples are always 12-bit. As another example, a sensor is assumed to use the "RGGB" pattern; there is no support for "BGGR" pattern sensors. (However, the Bayer interpolation logic is pretty much decoupled from the raw file processing, so the processing utilities could be adapted for use with other cameras without requiring too much cleanup work.)
* I haven't even tried to compile the utilities on anything other than a couple of Linux machines (which are now positively ancient; one of them was Red Hat 7.3 with gcc 2.96; the other was Red Hat 8.0 with gcc 3.2). Things seem to have compiled on Fedora Core 2 and 3 as well. The code is all pretty much standard C++ though, so it should compile with little or no modification almost anywhere (as long as your C++ compiler supports templates). So far one user has had some success compiling and using the utilities under MacOS X, so that's encouraging.

## License
ImageCooker is distributed under the terms of the GNU General Public License, but I haven't yet added the source code comments, etc. to that effect. Chuan-kai Lin has also made his "reference implementation" of Pixel Grouping available under the GNU GPL, and holds the copyright to the Pixel Grouping implementation. Other than the Pixel Grouping implementation, you may also elect to use ImageCooker under the terms of the LGPL.

## Version history
These release notes were transcribed from the original stone tablets:

### 0.4 (8 April 2003)
* Implemented support for multiple CPUs. ImageCooker determines how many CPUs are available, and creates one thread per CPU, dividing the processing work among them. ImageCooker is one of few such utilities to offer multiple CPU support!
* The command line utilities (e.g. `mrw2tiff`) now will output the list of recognized interpolators when invoked with no parameters (or too few parameters). So it's no longer necessary to look in the source code to see what interpolation algorithms are supported. :-)
* Renamed to ImageCooker for the following reasons:
  - to avoid trademark issues that will inevitably arise if the previous name was kept (*DiMAGE* is a Konica Minolta trademark);
  - to reflect the purpose of the application: to process or "cook" raw images;
  - to adopt a camera-agnostic name, as support for raw image formats other than Minolta MRW is expected eventually.
* Implemented Chuan-kai Lin's Pixel Grouping algorithm (actually he provided the implementation) and made it the default (since it's so good).
  - The original Pixel Grouping page is ancient history, although [this one](https://sites.google.com/site/chklin/demosaic) is still alive, or there's always [Wayback Machine to the rescue](https://web.archive.org/web/20061208165337/http://web.cecs.pdx.edu/~cklin/demosaic/).
* Made `COMPRESSION_NONE` the default compression scheme for TIFF files, as a number of applications had trouble with Deflate compression.
* Made 48 bits the default depth for TIFF file generation.
* Fixed divide-by-zero and boundary condition issues with Smooth Hue Transition and Smooth Hue Transition Log interpolators (thanks to Dalibor Jelinek for pointing this one out).
* Fixed incorrect coefficients in Color Correction 1 interpolator (thanks to "Vector" for tracking down this problem, which is actually a bug in Ting Chen's description).

### 0.3 (19 January 2003)
* Added the `mrw2tiff` utility, which does just what its name suggests. The ability exists to create 8- and 16-bit TIFFs, although currently it requires a source code change and recompile to change from 8 to 16 (yuck). Currently only image data is converted; EXIF tags are not added to the generated TIFF file. This does require `libtiff` to build.
* Implemented some optimizations suggested by Frank Driesens to speed up access to CCD data. Simple benchmarking suggests this is good for a 5-10% speed improvement.
* Speaking of optimizations, added `-O3 -march=pentium3` compiler options to the `Makefile`, resulting in significant performance gains. Obviously `pentium3` should be replaced by the appropriate CPU type when compiling on other machines.
* Fixed a bug or two in `RifBlock.cpp` which misreported sharpness values (or possibly segfaulted while attempting to do so). Thanks to Mark Rubin for pointing me on the trail of this one.

### 0.2 (14 January 2003)
* Completed implementing the "Threshold-Based Variable Number of Gradients" interpolation algorithm. Of the algorithms presented in Ting Chen's study, Variable Number of Gradients is considered to produce the best images, but is not suprisingly the most computationally expensive. This is now the default strategy for the GIMP plugin (it still can't be changed without recompiling).
* Added the "Pattern Recognition" interpolation algorithm. Pattern Recognition tends to smooth out noise; it deals with edges subjectively better than the Edge Sensing algorithms while being slightly cheaper computationally. It does, however, produce softer edges and details than Threshold-Based Variable Number of Gradients or Laplacian Color Correction.
* Refactored to reduce duplicated code (particularly bilinear interpolation calculations).

### 0.1 (11 January 2003)
* Released initial version. Implemented basic functionality in `mrwinfo`, `mrw2ppm` and the GIMP plugin, with half a dozen or so Bayer interpolation algorithms available.
