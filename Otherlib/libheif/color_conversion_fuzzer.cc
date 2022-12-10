/*
 * HEIF codec.
 * Copyright (c) 2019 struktur AG, Joachim Bauch <bauch@struktur.de>
 *
 * This file is part of libheif.
 *
 * libheif is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * libheif is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with libheif.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <assert.h>

#include <sstream>

#include "bitstream.h"
#include "heif_colorconversion.h"
#include "heif_image.h"

static bool is_valid_chroma(uint8_t chroma)
{
  switch (chroma) {
    case heif_chroma_monochrome:
    case heif_chroma_420:
    case heif_chroma_422:
    case heif_chroma_444:
    case heif_chroma_interleaved_RGB:
    case heif_chroma_interleaved_RGBA:
    case heif_chroma_interleaved_RRGGBB_BE:
    case heif_chroma_interleaved_RRGGBBAA_BE:
    case heif_chroma_interleaved_RRGGBB_LE:
    case heif_chroma_interleaved_RRGGBBAA_LE:
      return true;
    default:
      return false;
  }
}

static bool is_valid_colorspace(uint8_t colorspace)
{
  switch (colorspace) {
    case heif_colorspace_YCbCr:
    case heif_colorspace_RGB:
    case heif_colorspace_monochrome:
      return true;
    default:
      return false;
  }
}

static bool read_plane(heif::BitstreamRange* range,
                       std::shared_ptr<heif::HeifPixelImage> image, heif_channel channel,
                       int width, int height, int bit_depth)
{
  if (width <= 0 || height <= 0) {
    return false;
  }
  if (!range->prepare_read(static_cast<uint64_t>(width) * height)) {
    return false;
  }
  if (!image->add_plane(channel, width, height, bit_depth)) {
    return false;
  }
  int stride;
  uint8_t* plane = image->get_plane(channel, &stride);
  assert(stride >= width);
  auto stream = range->get_istream();
  for (int y = 0; y < height; y++, plane += stride) {
    assert(stream->read(plane, width));
  }
  return true;
}

static bool read_plane_interleaved(heif::BitstreamRange* range,
                                   std::shared_ptr<heif::HeifPixelImage> image, heif_channel channel,
                                   int width, int height, int bit_depth, int comps)
{
  if (width <= 0 || height <= 0) {
    return false;
  }
  if (!range->prepare_read(static_cast<uint64_t>(width) * height * comps)) {
    return false;
  }
  if (!image->add_plane(channel, width, height, bit_depth)) {
    return false;
  }
  int stride;
  uint8_t* plane = image->get_plane(channel, &stride);
  assert(stride >= width * comps);
  auto stream = range->get_istream();
  for (int y = 0; y < height; y++, plane += stride) {
    assert(stream->read(plane, width * comps));
  }
  return true;
}
