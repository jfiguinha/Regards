#pragma once
#include "TreeElementTexte.h"

namespace Regards::Window
{
	class CTreeElementTexteClick : public CTreeElementTexte
	{
	public:
		CTreeElementTexteClick();
		~CTreeElementTexteClick() override;

		void SetTypeLink(const int& typeElement);
		void SetLinkElement(const wxString& linkElement);
		int GetTypeLink();
		wxString GetLinkElement();
		void ClickElement(wxWindow* window, const int& x, const int& y) override;

	private:
		int typeElement;
		wxString linkElement;
	};
}
