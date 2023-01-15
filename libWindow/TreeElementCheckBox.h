#pragma once
#include "TreeElement.h"
#include "TreeElementListBox.h"


namespace Regards::Window
{
	class CTreeElementCheckBox : public CTreeElement
	{
	public:
		CTreeElementCheckBox();
		~CTreeElementCheckBox() override;

		void DrawElement(wxDC* deviceContext, const int& x, const int& y) override;
		void ClickElement(wxWindow* window, const int& x, const int& y) override;
		void SetCheckState(const bool& check);
		bool GetCheckState();
		void SetTheme(CThemeTreeCheckBox* theme);

		CTreeElementCheckBox& operator=(const CTreeElementCheckBox& other);

		void SetZoneSize(const int& width, const int& height) override
		{
			themeTreeCheckBox.SetWidth(width);
			themeTreeCheckBox.SetHeight(height);
		}


		void SetBackgroundColor(const wxColour& color) override
		{
			themeTreeCheckBox.color = color;
		}

		int GetWidth() override
		{
			return themeTreeCheckBox.GetWidth();
		}

		int GetHeight() override
		{
			return themeTreeCheckBox.GetHeight();
		}

	private:
		void DrawBitmap(wxDC* deviceContext, const int& xPos, const int& yPos);
		bool checked;

		wxBitmap checkOn;
		wxBitmap checkOff;
		CThemeTreeCheckBox themeTreeCheckBox;
	};
}
