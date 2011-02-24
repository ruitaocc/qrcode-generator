#ifndef PAINTCONTEXT_H
#define PAINTCONTEXT_H

#include "util.h"

#include <cairo.h>

typedef struct {
	cairo_t* cr;                /*  cairo context to which te qrcode is painted */
	char* filename;             /* filename to wich the qrcode is written */
	bool writeToFile;           /* indicator wheter the qrcode shall be painted to a cairo surface or to a file */
	
	double x;                      /* x position on the surface  (ignored when writing to file) */
	double y;                      /* y position on the surface  (ignored when writing to file) */
		
	int size;                   /* size in pixels */
	bool bitSize;               /* indicator wheter the size specifies the size of the whole qrcode or  the size of one bit module */
		
	bool quietZone;             /* add a white zone width the size of 4 bits around the qrcode */
	
	bool calculateOptimalSize;  /* calculate a size, so that a bit has  anintegral size */
	
	/* Debug options */
	bool debug;                 /* if debug is False, the options below are ignored */
	int highlightZone;			/* which zones need to be highlighted , can be any combination of QRZones */
	int skipZone;               /* which zones don't need to be painted. can be any combination of QRZones */
	bool drawRaster;            /* draw a raster */
	
	bool noMask;                /* don't mask the data */
	bool noData;                /* ignore data, don't generate error correction code, everything is set to 0 */
} PaintContext;

typedef enum {
	QR_FINDER_PATTERN =     1,
	QR_ALIGNEMENT_PATTERN = 1 << 1,
	QR_TIMING_PATTERN =     1 << 2,
	QR_DATA =               1 << 3,
	QR_EC =                 1 << 4,
	QR_REMAINDER =          1 << 5,
	QR_VERSION_INFO =       1 << 6,
	QR_FORMAT_INFO =        1 << 7,
	QR_QUIET_ZONE =         1 << 8
} QRZones;

PaintContext* create_paint_context(cairo_t* cr);
PaintContext* create_paint_context_for_file(char* filename);

void destroy_paint_context(PaintContext* pc);

bool pc_set_cairo_context(PaintContext* pc, cairo_t* cr);
bool pc_set_filename(PaintContext* pc, char* filename);
bool pc_set_position(PaintContext* pc, double x, double y);

bool pc_set_size(PaintContext* pc, int size, bool isBitSize);
bool pc_set_is_bit_size(PaintContext* pc, bool isBitSize);

bool pc_draw_quiet_zone(PaintContext* pc, bool drawQuietZone);

bool pc_calculate_optimal_size(PaintContext* pc, bool calculateOptimalSize);

bool pc_enable_debug_options(PaintContext* pc, bool enableDebugOptions);

bool pc_set_skipped_zones(PaintContext* pc, int zones);
bool pc_set_highlighted_zones(PaintContext* pc, int zones);
bool pc_set_draw_raster(PaintContext* pc, bool draw_raster);
bool pc_set_dont_mask(PaintContext* pc, bool drawnomask);
bool pc_set_draw_no_data(PaintContext* pc, bool nodata);


#endif //PAINTCONTEXT_H