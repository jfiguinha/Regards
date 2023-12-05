#pragma once
#include "ThumbnailHorizontal.h"

namespace Regards::Control
{
	class CThumbnailVideo : public CThumbnailHorizontal
	{
	public:
		CThumbnailVideo(wxWindow* parent, wxWindowID id, const CThemeThumbnail& themeThumbnail,
		                const bool& testValidity);
		~CThumbnailVideo(void) override;
		void SetFile(const wxString& videoFile, const int& size);
		void SetVideoPosition(const int64_t& videoPos);
		void EraseThumbnail(long value);

    
	private:

		void ResizeThumbnail() override;
		static bool ItemCompFonct(int x, int y, CIcone* icone, CWindowMain* parent);
		void EraseThumbnail(wxCommandEvent& event) override;
		void UpdateVideoThumbnail(wxCommandEvent& event);
		void UpdateVideoThumbnail();

		int FindNumItem(const int& videoPos);
		void InitWithDefaultPicture(const wxString& szFileName, const int& size);
		int numItemSelected;
		bool process_end;

		int64_t oldvideoPos = 0;

		bool videoThumbnailOk = false;
		int iFormat = 0;
	};
}
