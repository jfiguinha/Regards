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


#include <string>
#include <vector>

#include "imagewriter.h"

class CRegardsBitmap;
class CRgbaquad;

namespace ffmpegthumbnailer
{

	class CxImageWriter : public ImageWriter
	{
	public:

		struct weightX
		{
		public:
			float tabF[4];
		};


		CxImageWriter(std::vector<uint8_t>& outputBuffer);
		CxImageWriter(const std::string& outputFile);
		CxImageWriter(CRegardsBitmap * image);
		~CxImageWriter();
		
		void setText(const std::string& key, const std::string& value);
		void writeFrame(std::vector<uint8_t> & rgbData,int lineSize, int width, int height, int quality, int thumbnailWidth, int thumbnailHeight, int rotation);
		
	private:
		virtual inline void Bicubic(CRgbaquad & data, std::vector<uint8_t> & rgbData, int width, int height, int effwidth, const float &x, const float &y, float * tabF1, float * tabF);
		double Filter(const double &f);
		void init();
		void CalculWeight(const int32_t &width, const int32_t &height, const float &ratioY, const float &ratioX, const float &posTop, const float &posLeft);
		CRegardsBitmap * image;
		weightX * wX;
		weightX * wY;
	};

}

