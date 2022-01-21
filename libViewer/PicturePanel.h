#pragma once
#include "WindowMain.h"

class CRegardsBitmap;

namespace Regards
{
	namespace Window
	{
		class CPicturePanel : public CWindowMain
		{
		public:
			CPicturePanel(wxWindow* parent, wxWindowID id, const CThemeThumbnail& theme);
			~CPicturePanel() override;
			void SetPictureToDisplay(CRegardsBitmap* picture);
			void on_paint(wxPaintEvent& event);
			void UpdateScreenRatio() override;
		private:

			void CalculateHistogram(CRegardsBitmap* pBitmap, CRegardsBitmap* histogram, const int& colorChoice, const wxColour& colorBgnd, const wxColour& colorFont);
			void CreateHistogram();
			void NormalizeHistogram(CRegardsBitmap* pictureData, const int& colorChoice, const int& minValue, const int& maxValue);
			void OnChannelSelect(wxCommandEvent& event);
			//void OnRangeSelect(wxSpinEvent& evt);
			bool refreshPicture = true;
			wxImage* image;
			CRegardsBitmap* histogram = nullptr;
			CRegardsBitmap* pictureOriginal;
			//CRegardsBitmap * pictureBackup;

			int w, h;
			wxColour colorBack;
			wxColour colorFont;
			int marged = 10;
			int channelSelect = 0;
			wxComboBox* choice_control;

			//wxSpinCtrl* minValue;
			//wxSpinCtrl* maxValue;
		};
	}
}
