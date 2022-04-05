#pragma once
#include "RGBAQuad.h"
class CRegardsBitmap;

namespace Regards
{
	namespace FiltreEffet
	{
		class CRedEye
		{
		public:
			CRedEye(void){};
			~CRedEye(void){};

			bool RemoveRedEye(CRegardsBitmap * m_CRegardsBitmap);

		private:
			wxRect ConvertPositionWithAngle(CRegardsBitmap * pBitmap, const wxRect & rSelectionBox, int angle);
			CRgbaquad CalculColorOut(CRgbaquad colorIn);
			void RemoveRedEye(CRegardsBitmap * pBitmap, const wxRect & rSelectionBox, int angle);
		};
	}
}

