#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <string>
#include <vector>
using namespace std;

#define DEFAULT_SIZEX 60
#define DEFAULT_SIZEY 60

namespace Regards
{
	namespace Window
	{

		class CToolbarElement
		{
		public:
			CToolbarElement();
			virtual ~CToolbarElement();

			void SetRepeatable(const bool &repeat);
			bool GetRepeatable();

			void SetLibelleTooltip(const wxString &libelle);
			wxString GetLibelleTooltip();

			void SetCommandId(const int &commandId);
			int GetCommandId();

			void SetPosition(const int &x, const int &y);

			void SetVertical(const bool &isVertical);
			bool GetVertical();

			virtual void DrawButton(wxDC * dc) = 0;
			virtual void Resize(const int &tailleX, const int &tailleY) = 0;
			int GetXPos();
			int GetYPos();

			virtual int GetWidth() = 0;
			virtual int GetHeight() = 0;

			bool IsVisible();
			void SetVisible(const bool &value);

			void IsPushActif(const bool &activePush);
			bool SetInactif();
			bool SetActif();
			void SetPush(const bool &push);

			bool FindElement(const int &xPos, const int &yPos);

			virtual bool MouseOver(wxDC * context, const int &x, const int &y)
			{
				::wxSetCursor(wxCursor(wxCURSOR_HAND));
				return false;
			};
            
            void SetBackgroundBitmap(wxImage bmBackground);

			virtual void UnclickElement(wxWindow * window, const int &x, const int &y){};
			virtual void ClickElement(wxWindow * window, const int &x, const int &y){};

		protected:

			int commandId;
			wxString libelleTooltip;
            wxImage bmBackground;
			int x;
			int y;
			bool isActif = false;
			bool isPush = false;
			bool isVisible;
			bool isRepeatable;
			bool isVertical = false;
			bool activePush = true;
		};

		typedef vector<CToolbarElement *> VectorNavigatorElement;
	}
}


