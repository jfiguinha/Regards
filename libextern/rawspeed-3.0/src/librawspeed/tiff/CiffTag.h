/*
    RawSpeed - RAW file decoder.

    Copyright (C) 2009-2014 Klaus Post
    Copyright (C) 2014 Pedro Côrte-Real

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#pragma once

#include <initializer_list> // for initializer_list

namespace rawspeed {

enum CiffTag {
  CIFF_NULL         = 0x0000,
  CIFF_MAKEMODEL    = 0x080a,
  CIFF_SHOTINFO     = 0x102a,
  CIFF_WHITEBALANCE = 0x10a9,
  CIFF_SENSORINFO   = 0x1031,
  CIFF_IMAGEINFO    = 0x1810,
  CIFF_DECODERTABLE = 0x1835,
  CIFF_RAWDATA      = 0x2005,
  CIFF_SUBIFD       = 0x300a,
  CIFF_EXIF         = 0x300b,
};

static constexpr std::initializer_list<CiffTag> CiffTagsWeCareAbout = {
    CIFF_DECODERTABLE,
    CIFF_MAKEMODEL,
    CIFF_RAWDATA,
    CIFF_SENSORINFO,
    CIFF_SHOTINFO,
    CIFF_WHITEBALANCE,
    static_cast<CiffTag>(0x0032), // ???
    static_cast<CiffTag>(0x102c), // ???
};

} // namespace rawspeed
