#pragma once
#include <RegardsBitmap.h>
#include <RGBAQuad.h>
#include <wx/gdicmn.h>

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

