#pragma once
#include <RGBAQuad.h>
#include <wx/gdicmn.h>

class CRegardsBitmap;

namespace Regards
{
	namespace FiltreEffet
	{
		class CRotation
		{
		public:
			CRotation(void);
			~CRotation(void);

			static void Rotate(CRegardsBitmap * pBitmap,const float &angle, const CRgbaquad &clrBack);
		};

	}
}

