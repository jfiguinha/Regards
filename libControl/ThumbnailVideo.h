#pragma once
#include "ThumbnailHorizontal.h"
#include <ImageVideoThumbnail.h>

namespace Regards
{
	namespace Control
	{
		class CThumbnailVideo : public CThumbnailHorizontal
		{
		public:
			CThumbnailVideo(wxWindow* parent, wxWindowID id, const CThemeThumbnail & themeThumbnail, const bool &testValidity);
			virtual ~CThumbnailVideo(void);
			void SetFile(const wxString &videoFile, const int &size = 20);
			void SetVideoPosition(const int64_t &videoPos);

		private:

			void ResizeThumbnail();
			static bool ItemCompFonct(int x, int y, CIcone * icone, CWindowMain * parent);
           virtual void EraseThumbnail(wxCommandEvent& event);
		   void EndVideoThumbnail(wxCommandEvent& event);
			void ProcessThumbnail();
			int FindNumItem(const int &videoPos);
			void InitWithDefaultPicture(const wxString & szFileName, const int &size = 20);
			int numItemSelected;
			bool process_end;
			wxString videoFilename;
		};
	}
}

