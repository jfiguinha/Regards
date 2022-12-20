#pragma once
#include "TreeElementTexte.h"

namespace Regards
{
	namespace Window
	{

		class CTreeElementTexteClick : public CTreeElementTexte
		{
		public:
			CTreeElementTexteClick();
			~CTreeElementTexteClick();

			void SetTypeLink(const int &typeElement);
			void SetLinkElement(const wxString &linkElement);
			int GetTypeLink();
			wxString GetLinkElement();
			void ClickElement(wxWindow * window, const int &x, const int &y);

		private:

			int typeElement;
			wxString linkElement;
		};
	}
}

