#pragma once
#include "RegardsBitmap.h"
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

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
				int ligne = 0;
				int nbColor = 0;
				float pourcentage = 0;
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

