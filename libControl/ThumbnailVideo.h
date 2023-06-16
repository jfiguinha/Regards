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
		
	private:

		void ProcessVideo() override;

		static void ProcessThumbnail(void* param);
		static void LoadMoviePicture(void* param);
		void ResizeThumbnail() override;
		static bool ItemCompFonct(int x, int y, CIcone* icone, CWindowMain* parent);
		void EraseThumbnail(wxCommandEvent& event) override;
		void EndVideoThumbnail(wxCommandEvent& event);
		void EndUpdateVideoThumbnail(wxCommandEvent& event);

		void EndThumbnail(wxCommandEvent& event);

		int FindNumItem(const int& videoPos);
		void InitWithDefaultPicture(const wxString& szFileName, const int& size);
		int numItemSelected;
		bool process_end;

		int64_t oldvideoPos = 0;


		int iFormat = 0;
	};
}
