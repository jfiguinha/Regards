#ifndef RW2_DECODER_H
#define RW2_DECODER_H

#include "RawDecoder.h"
#include "TiffIFD.h"
#include "BitPumpPlain.h"
#include "TiffParser.h"
/*
    RawSpeed - RAW file decoder.

    Copyright (C) 2009 Klaus Post

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

    http://www.klauspost.com
*/

namespace RawSpeed
{
	class PanaBitpump
	{
	public:
		PanaBitpump(ByteStream* input);
		virtual ~PanaBitpump();
		ByteStream* input;
		uchar8 buf[0x4000];
		int vbits;
		uint32 load_flags;
		uint32 getBits(int nbits);
		void skipBytes(int bytes);
	};

	class Rw2Decoder :
		public RawDecoder
	{
	public:
		Rw2Decoder(TiffIFD* rootIFD, FileMap* file);
		~Rw2Decoder(void) override;
		RawImage decodeRawInternal() override;
		void decodeMetaDataInternal(CameraMetaData* meta) override;
		void checkSupportInternal(CameraMetaData* meta) override;
		TiffIFD* mRootIFD;
		TiffIFD* getRootIFD() override { return mRootIFD; }

	protected:
		void decodeThreaded(RawDecoderThread* t) override;

	private:
		void DecodeRw2();
		std::string guessMode();
		ByteStream* input_start;
		uint32 load_flags;
	};
} // namespace RawSpeed

#endif
