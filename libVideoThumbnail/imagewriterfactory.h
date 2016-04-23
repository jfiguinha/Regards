//    Copyright (C) 2010 Dirk Vanden Boer <dirk.vdb@gmail.com>
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

#pragma once

#include "imagewriter.h"
#include "imagetypes.h"
#include <string>
#include <inttypes.h>
#include <stdexcept>
#include "cximagewriter.h"

namespace ffmpegthumbnailer
{

template <typename T>
class ImageWriterFactory
{
public:

	static ImageWriter* createImageWriter(CxImage * image)
	{
		return new CxImageWriter(image);
	}

    static ImageWriter* createImageWriter(ThumbnailerImageType imageType, T output)
    {
        switch (imageType)
        {

            case Png:
#ifdef HAVE_PNG
                return new PngWriter(output);
#else
                throw std::logic_error("ffmpegthumbnailer was not compiled with png support");
#endif
            case Jpeg:
#ifdef HAVE_JPEG
                return new JpegWriter(output);
#else
                throw std::logic_error("ffmpegthumbnailer was not compiled with jpeg support");
#endif

            default:
                throw std::logic_error("ImageWriterFactory::createImageWriter: Invalid image type specified");
        }

    }
};

}


