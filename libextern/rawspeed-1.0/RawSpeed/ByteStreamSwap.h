#ifndef BYTE_STREAM_SWAP_H
#define BYTE_STREAM_SWAP_H

#include "ByteStream.h"

#include "IOException.h"

namespace RawSpeed
{
	class ByteStreamSwap :
		public ByteStream
	{
	public:
		ByteStreamSwap(const uchar8* _buffer, uint32 _size);
		ByteStreamSwap(const ByteStreamSwap* b);
		ushort16 getShort() override;
		int getInt() override;
		~ByteStreamSwap(void) override;
		uint32 getUInt() override;
	};
} // namespace RawSpeed

#endif
