// Crop.h: interface for the CCrop class.
//
//////////////////////////////////////////////////////////////////////

#pragma once
#include "PenDrawInfo.h"
#include "Draw.h"

namespace Regards::FiltreEffet
{
	class CSelection : public CDraw
	{
	public:
		CSelection();
		~CSelection() override;
		void GetPoint(wxPoint& pt) override;
		void GetScreenPoint(wxPoint& pt) override;
		void InitPoint(const long& m_lx, const long& m_ly, const long& m_lHScroll, const long& m_lVScroll,
		               const float& ratio) override;
		void MouseMove(const long& xNewSize, const long& yNewSize, const long& m_lHScroll, const long& m_lVScroll,
		               const float& ratio) override;
		void Dessiner(wxDC* deviceContext, const long& m_lHScroll, const long& m_lVScroll, const float& ratio,
		              const wxColour& rgb, const wxColour& rgbFirst, const wxColour& rgbSecond,
		              const int32_t& style) override;
		void Selection(const int32_t& yNewSize, const int32_t& xNewSize, const long& m_lHScroll, const long& m_lVScroll,
		               const float& ratio) override;

	private:
		CPenDrawInfo* ptSelection;
		int iSelect;
		int marge;
		wxPoint ptScreen;
		wxPoint pt;
	};
}
