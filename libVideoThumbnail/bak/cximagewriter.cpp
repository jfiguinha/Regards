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
#include "cximagewriter.h"
#include <stdexcept>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <algorithm>
#include <ximage.h>
using namespace std;

namespace ffmpegthumbnailer
{
	CxImageWriter::CxImageWriter(const std::string& outputFile)
	{

	}

	CxImageWriter::CxImageWriter(std::vector<uint8_t>& outputBuffer)
	{

	}

	CxImageWriter::CxImageWriter(CxImage * image) : ImageWriter()
	{
		init();
		this->image = image;
	}

	CxImageWriter::~CxImageWriter()
	{
	}

	void CxImageWriter::init()
	{
	}

	void CxImageWriter::setText(const string& /*key*/, const string& /*value*/)
	{
	}

	void CxImageWriter::writeFrame(std::vector<uint8_t> & rgbData, int lineSize, int width, int height, int quality)
	{
		//int widthSize = ((((width * 24) + 31) & ~31) >> 3);

		//image->Create(width, height, 24);
		//image->RGBtoBGR(*rgbData, width*height * 4);
		image->CreateFromArray(&rgbData.at(0), rgbData.size(), width, height, 24, lineSize, true);
	}


}
