#pragma once
#include "WindowUtility.h"
#include <string>
#include <Theme.h>
#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
using namespace std;

wxDEFINE_EVENT(EVENT_VIDEOPOS, wxCommandEvent);

namespace Regards
{
	namespace Window
	{
		class CWindowMain : public wxWindow
		{
		public:
			CWindowMain(wxWindow* parent, wxWindowID id)
				: wxWindow(parent, id, wxPoint(0, 0), wxSize(0, 0), 0)
			{
				Connect(wxEVT_SIZE, wxSizeEventHandler(CWindowMain::OnSize));
				Connect(wxEVT_ERASE_BACKGROUND, wxEraseEventHandler(CWindowMain::OnEraseBackground));
			}

			virtual ~CWindowMain()
			{

			}
            

			static void FillRect(wxDC * dc, const wxRect &rc, const wxColour &color)
			{
				CWindowUtility winUtility;
				winUtility.FillRect(dc, rc, color);
			}

			static void DrawTexte(wxDC * dc, const wxString &libelle, const int &xPos, const int &yPos, const CThemeFont &font)
			{
				CWindowUtility winUtility;
				winUtility.DrawTexte(dc, libelle, xPos, yPos, font);
			}

			static wxSize GetSizeTexte(wxDC * dc, const wxString &libelle, const CThemeFont &font)
			{
				CWindowUtility winUtility;
				return winUtility.GetSizeTexte(dc, libelle, font);
			}

			virtual void OnEraseBackground(wxEraseEvent& event){};

			void OnSize(wxSizeEvent& event)
			{
				int _width = event.GetSize().GetX();
				int _height = event.GetSize().GetY();
				if (_width <= 20 && _height <= 20)
				{
					//not corrected size
					Resize();
				}
				else
				{
					width = _width;
					height = _height;
					Resize();
				}
			}

			wxRect GetWindowRect()
			{
				wxRect rc;
				rc.x = 0;
				rc.y = 0;
				rc.width = width;
				rc.height = height;
				return rc;
			}


		protected:

			virtual void Resize() = 0;


			int width = 0;
			int height = 0;
		};
	}
}