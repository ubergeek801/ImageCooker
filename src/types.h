#ifndef _TYPES_H
#define _TYPES_H

typedef enum {
	CCD_RED,		// red (always in a GRGR row)
	CCD_GREEN_RED,	// green appearing in a GRGR row
	CCD_GREEN_BLUE,	// green appearing in a BGBG row
	CCD_BLUE		// blue (always in a BGBG row)
} CcdColor;

#ifndef MIN
#define MIN(x, y) (x < y ? (x) : (y))
#endif

#ifndef MAX
#define MAX(x, y) (x > y ? (x) : (y))
#endif

#define CAP12(x) (MAX(MIN((x), 4095), 0))

#endif
