#pragma once
#include "ThumbnailHorizontal.h"

namespace Regards
{
	namespace Control
	{
		class CThumbnailVideo : public CThumbnailHorizontal
		{
		public:
			CThumbnailVideo(wxWindow* parent, wxWindowID id, const CThemeThumbnail& themeThumbnail,
			                const bool& testValidity);
			~CThumbnailVideo(void) override;
			void SetFile(const wxString& videoFile, const int& size = 20);
			void SetVideoPosition(const int64_t& videoPos);

		private:
			void ResizeThumbnail() override;
			static bool ItemCompFonct(int x, int y, CIcone* icone, CWindowMain* parent);
			void EraseThumbnail(wxCommandEvent& event) override;
			void EndVideoThumbnail(wxCommandEvent& event);
			void ProcessThumbnail();
			int FindNumItem(const int& videoPos);
			void InitWithDefaultPicture(const wxString& szFileName, const int& size = 20);
			int numItemSelected;
			bool process_end;
			wxString videoFilename;
			//const CThemeThumbnail& theme_thumbnail_;
			//const bool& test_validity_;
		};
	}
}
