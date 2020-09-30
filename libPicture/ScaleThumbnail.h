#pragma once

class CRegardsBitmap;

namespace Regards
{
	namespace Picture
	{
		class CScaleThumbnail
		{
		public:
			CScaleThumbnail(void);
			~CScaleThumbnail(void);
			static float CalculRatio(CRegardsBitmap * pBitmap, const int &xMax, const int &yMax);
			static void CreateScaleBitmap(CRegardsBitmap * pBitmap, const int &width, const int &height);
			static float CalculRatio(const int &width, const int &height, const int &xMax, const int &yMax);
		};
	}
}

