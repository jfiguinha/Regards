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
			~CPicturePanel();
			void SetPictureToDisplay(CRegardsBitmap* picture);
			void OnPaint(wxPaintEvent& event);
			void UpdateScreenRatio();
		private:

			void CreateHistogram();
			void OnChannelSelect(wxCommandEvent& event);
			bool refreshPicture = true;
			wxImage * image;
			CRegardsBitmap* histogram = nullptr;
			CRegardsBitmap * pictureOriginal;

			int w, h;
			wxColour colorBack;
			wxColour colorFont;
			int marged = 10;
			int channelSelect = 0;
			wxComboBox* choice_control;
		};
	}
}