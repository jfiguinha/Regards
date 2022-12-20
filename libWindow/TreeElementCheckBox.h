#pragma once
#include "TreeElement.h"
#include "TreeElementListBox.h"


namespace Regards
{
	namespace Window
	{
		class CTreeElementCheckBox : public CTreeElement
		{
		public:
			CTreeElementCheckBox();
			virtual ~CTreeElementCheckBox();

			void DrawElement(wxDC * deviceContext, const int &x, const int &y);
			void ClickElement(wxWindow * window, const int &x, const int &y);
			void SetCheckState(const bool &check);
			bool GetCheckState();
			void SetTheme(CThemeTreeCheckBox * theme);

			CTreeElementCheckBox& operator=(const CTreeElementCheckBox &other);

			void SetZoneSize(const int &width, const int &height)
			{
				themeTreeCheckBox.SetWidth(width);
				themeTreeCheckBox.SetHeight(height);
			}


			void SetBackgroundColor(const wxColour &color)
			{
				themeTreeCheckBox.color = color;
			}

			int GetWidth()
			{
				return themeTreeCheckBox.GetWidth();
			}

			int GetHeight()
			{
				return themeTreeCheckBox.GetHeight();
			}


		private:

			void DrawBitmap(wxDC * deviceContext, const int &xPos, const int &yPos);
			bool checked;

			wxBitmap checkOn;
			wxBitmap checkOff;
			CThemeTreeCheckBox themeTreeCheckBox;
		};
	}
}
	