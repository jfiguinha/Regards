// Crop.h: interface for the CCrop class.
//
//////////////////////////////////////////////////////////////////////

#pragma once
#include <PenDrawInfo.h>
#include "Draw.h"

namespace Regards
{
	namespace FiltreEffet
	{
		class CCrop : public CDraw
		{
		public:
			CCrop();
			virtual ~CCrop();
			void GetPos(wxRect &rc);
			void InitPoint(const long &m_lx, const long &m_ly, const long &m_lHScroll, const long &m_lVScroll, const float &ratio);
			void MouseMove(const long &xNewSize, const long &yNewSize, const long &m_lHScroll, const long &m_lVScroll, const float &ratio);
			void Dessiner(wxDC * deviceContext, const long &m_lHScroll, const long &m_lVScroll, const float &ratio, const wxColour &rgb, const wxColour &rgbFirst, const wxColour &rgbSecond, const int32_t &style);
			void Selection(const int32_t &yNewSize, const int32_t &xNewSize, const long &m_lHScroll, const long &m_lVScroll, const float &ratio);
		private:
			CPenDrawInfo * ptSelection;
			int iSelect;
			int marge;
		};
	}
}


