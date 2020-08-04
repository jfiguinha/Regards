#pragma once
#include <WindowMain.h>
#include <wx/animate.h>
using namespace Regards::Window;

namespace Regards
{
	namespace Viewer
	{
		class CWaitingWindow : public CWindowMain
		{
		public:
			CWaitingWindow(wxWindow* parent, wxWindowID id);

			~CWaitingWindow();

			void UpdateScreenRatio();

			void Resize();

			void SetTexte(const wxString &libelle);

		private:

			void OnPaint(wxPaintEvent& event);

			wxAnimationCtrl * m_animationCtrl;
			wxString textToShow;
		};
	}
}