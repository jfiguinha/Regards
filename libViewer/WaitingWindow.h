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

			~CWaitingWindow() override;

			void UpdateScreenRatio() override;

			void Resize() override;

			void SetTexte(const wxString& libelle);

		private:
			void on_paint(wxPaintEvent& event);

			wxAnimationCtrl* m_animationCtrl;
			wxString textToShow;
		};
	}
}
