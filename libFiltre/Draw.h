// Dessin.h: interface for the CDessin class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

namespace Regards::FiltreEffet
{
	class CDraw
	{
	public:
		void SetRatio(const float& m_fValue);
		void SetMaxPosition(const wxRect& m_rcPicture);
		CDraw();
		virtual ~CDraw();
		static void DrawARectangle(wxDC* deviceContext, const wxRect& rc, const wxColour& rgb);
		static void DessinerRectangleVide(wxDC* deviceContext, const int32_t& iTaille, const wxRect& rc,
		                                  const wxColour& rgb);
		void DessinerCarre(wxDC* deviceContext, const int32_t& iLargeur, const int32_t& iHauteur, const int32_t& iMarge,
		                   const int32_t& iPosX = 0, const int32_t& iPosY = 0,
		                   const wxColour& rgb = wxColour(0, 0, 0, 0));
		static void DessinerDashRectangle(wxDC* deviceContext, const int32_t& iTaille, const wxRect& rc,
		                                  const wxColour& rgbFirst, const wxColour& rgbSecond);
		static void DessinerDotDashRectangle(wxDC* deviceContext, const int32_t& iTaille, const wxRect& rc,
		                                     const wxColour& rgbFirst, const wxColour& rgbSecond);

		virtual void Dessiner(wxDC* deviceContext, const long& m_lHScroll, const long& m_lVScroll, const float& ratio)
		{
		};

		virtual void Dessiner(wxDC* deviceContext, const long& m_lHScroll, const long& m_lVScroll, const float& ratio,
		                      const wxColour& rgb)
		{
		};

		virtual void Dessiner(wxDC* deviceContext, const long& m_lHScroll, const long& m_lVScroll, const float& ratio,
		                      const wxColour& rgb, const wxColour& rgbFirst, const wxColour& rgbSecond,
		                      const int32_t& style)
		{
		};

		virtual void Selection(const int32_t& xNewSize, const int32_t& yNewSize, const long& m_lHScroll,
		                       const long& m_lVScroll, const float& ratio)
		{
		};

		virtual void MouseMove(const long& m_lx, const long& m_ly, const long& m_lHScroll, const long& m_lVScroll,
		                       const float& ratio)
		{
		};

		virtual void MouseMove(wxDC* deviceContext, const long& m_lx, const long& m_ly, const long& m_lHScroll,
		                       const long& m_lVScroll, const float& ratio, const wxColour& rgb)
		{
		};

		virtual void MouseMove(wxDC* deviceContext, const long& m_lx, const long& m_ly, const long& m_lHScroll,
		                       const long& m_lVScroll, const float& ratio, const wxColour& rgb, const wxColour& rgbBack)
		{
		};

		virtual void InitPoint(const long& m_lx, const long& m_ly, const long& m_lHScroll, const long& m_lVScroll,
		                       const float& ratio)
		{
		};

		virtual void InitPoint(const long& m_lx, const long& m_ly, const long& m_lHScroll, const long& m_lVScroll,
		                       const float& ratio, const wxColour& rgb)
		{
		};

		virtual void GetPos(wxRect& rc)
		{
		};

		virtual void GetPoint(wxPoint& pt)
		{
		};

		virtual void GetScreenPoint(wxPoint& pt)
		{
		};

		virtual void SetScaleFactor(const double& factor)
		{
			this->factor = factor;
		}

		virtual double GetScaleFactor()
		{
			return factor;
		}

	protected:
		wxRect m_rcAffichage;
		float m_fRatioValue;
		wxPoint pt;
		double factor = 1.0;
		bool VerifierValiditerPoint(const wxPoint& pt);
		float XDrawingPosition(const float& m_lx, const long& m_lHScroll, const float& ratio);
		float YDrawingPosition(const float& m_ly, const long& m_lVScroll, const float& ratio);
		float XRealPosition(const float& m_lx, const long& m_lHScroll, const float& ratio);
		float YRealPosition(const float& m_ly, const long& m_lVScroll, const float& ratio);
	};
}
