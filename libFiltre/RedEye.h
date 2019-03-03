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


            struct level
			{
				wxPoint pt;
				int maxLux;
				int distanceRight;
				int distanceLeft;
			};

            struct ligneColor
			{
				ligneColor()
				{
					ligne = 0;
					nbColor = 0;
					pourcentage = 0;
				};
				int ligne;
				int nbColor;
				float pourcentage;
			};

			CRedEye(void){};
			~CRedEye(void){};

			bool RemoveRedEye(CRegardsBitmap * m_CRegardsBitmap, const CRgbaquad & backcolor, const wxRect& rSelectionBox);

		private:
			wxRect FindIris(wxPoint leftEye, int distanceMax, CRegardsBitmap * bitmap);
			CRgbaquad CalculColorOut(CRgbaquad colorIn);
	
		};
	}
}

