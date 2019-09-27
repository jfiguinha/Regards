//========================================================================
//
// JpegWriter.cc
//
// This file is licensed under the GPLv2 or later
//
// Copyright (C) 2009 Stefan Thomas <thomas@eload24.com>
// Copyright (C) 2010, 2012, 2017 Adrian Johnson <ajohnson@redneon.com>
// Copyright (C) 2010 Harry Roberts <harry.roberts@midnight-labs.org>
// Copyright (C) 2011 Thomas Freitag <Thomas.Freitag@alfa.de>
// Copyright (C) 2013 Peter Breitenlohner <peb@mppmu.mpg.de>
// Copyright (C) 2017, 2018 Albert Astals Cid <aacid@kde.org>
// Copyright (C) 2018 Martin Packman <gzlist@googlemail.com>
// Copyright (C) 2018 Ed Porras <ed@motologic.com>
//
//========================================================================

#include "JpegWriter.h"

#ifdef ENABLE_LIBJPEG

extern "C" {
#include <jpeglib.h>
}

#include "poppler/Error.h"

struct JpegWriterPrivate {
  bool progressive;
  bool optimize;
  int quality;
  JpegWriter::Format format;
  struct jpeg_compress_struct cinfo;
  struct jpeg_error_mgr jerr;
};

static void outputMessage(j_common_ptr cinfo)
{
  char buffer[JMSG_LENGTH_MAX];

  // Create the message
  (*cinfo->err->format_message) (cinfo, buffer);

  // Send it to poppler's error handler
  error(errInternal, -1, "{0:s}", buffer);
}

JpegWriter::JpegWriter(int q, bool p, Format formatA)
{
  priv = new JpegWriterPrivate;
  priv->progressive = p;
  priv->optimize = false;
  priv->quality = q;
  priv->format = formatA;
}

JpegWriter::JpegWriter(Format formatA)
 : JpegWriter(-1, false, formatA)
{
}

JpegWriter::~JpegWriter()
{
  // cleanup
  jpeg_destroy_compress(&priv->cinfo);
  delete priv;
}

void JpegWriter::setQuality(int quality)
{
  priv->quality = quality;
}

void JpegWriter::setProgressive(bool progressive)
{
  priv->progressive = progressive;
}

void JpegWriter::setOptimize(bool optimize)
{
  priv->optimize = optimize;
}

bool JpegWriter::init(FILE *f, int width, int height, int hDPI, int vDPI)
{
  // Setup error handler
  priv->cinfo.err = jpeg_std_error(&priv->jerr);
  priv->jerr.output_message = &outputMessage;

  // Initialize libjpeg
  jpeg_create_compress(&priv->cinfo);

  // First set colorspace and call jpeg_set_defaults() since
  // jpeg_set_defaults() sets default values for all fields in
  // cinfo based on the colorspace.
  switch (priv->format) {
    case RGB:
      priv->cinfo.in_color_space = JCS_RGB;
      break;
    case GRAY:
      priv->cinfo.in_color_space = JCS_GRAYSCALE;
      break;
    case CMYK:
      priv->cinfo.in_color_space = JCS_CMYK;
      break;
    default:
      return false;
  }
  jpeg_set_defaults(&priv->cinfo);

  // Set destination file
  jpeg_stdio_dest(&priv->cinfo, f);

  // Set libjpeg configuration
  priv->cinfo.image_width = width;
  priv->cinfo.image_height = height;
  priv->cinfo.density_unit = 1; // dots per inch
  priv->cinfo.X_density = hDPI;
  priv->cinfo.Y_density = vDPI;
  switch (priv->format) {
    case GRAY:
      priv->cinfo.input_components = 1;
      break;
    case RGB:
      priv->cinfo.input_components = 3;
      break;
    case CMYK:
      priv->cinfo.input_components = 4;
      jpeg_set_colorspace(&priv->cinfo, JCS_YCCK);
      priv->cinfo.write_JFIF_header = TRUE;
      break;
    default:
      return false;
  }

  // Set quality
  if (priv->quality >= 0 && priv->quality <= 100) {
    jpeg_set_quality(&priv->cinfo, priv->quality, TRUE);
  }

  // Use progressive mode
  if (priv->progressive) {
    jpeg_simple_progression(&priv->cinfo);
  }

  // Set whether to compute optimal Huffman coding tables
  priv->cinfo.optimize_coding = static_cast<boolean>(priv->optimize);

  // Get ready for data
  jpeg_start_compress(&priv->cinfo, TRUE);

  return true;
}

bool JpegWriter::writePointers(unsigned char **rowPointers, int rowCount)
{
  if (priv->format == CMYK) {
    for (int y = 0; y < rowCount; y++) {
      unsigned char *row = rowPointers[y];
      for (unsigned int x = 0; x < priv->cinfo.image_width; x++) {
	for (int n = 0; n < 4; n++) {
	  *row = 0xff - *row;
	  row++;
	}
      }
    }
  }
  // Write all rows to the file
  jpeg_write_scanlines(&priv->cinfo, rowPointers, rowCount);

  return true;
}

bool JpegWriter::writeRow(unsigned char **rowPointer)
{
  if (priv->format == CMYK) {
    unsigned char *row = rowPointer[0];
    for (unsigned int x = 0; x < priv->cinfo.image_width; x++) {
      for (int n = 0; n < 4; n++) {
	*row = 0xff - *row;
	row++;
      }
    }
  }
  // Write the row to the file
  jpeg_write_scanlines(&priv->cinfo, rowPointer, 1);

  return true;
}

bool JpegWriter::close()
{
  jpeg_finish_compress(&priv->cinfo);

  return true;
}

bool JpegWriter::supportCMYK()
{
  return priv->format == CMYK;
}


#endif
