#pragma once
#include <ThumbnailVideo.h>
using namespace Regards::Control;

namespace Regards
{
	namespace Viewer
	{

		class CThumbnailViewerVideo : public CThumbnailVideo
		{
		public:
			CThumbnailViewerVideo(wxWindow* parent, wxWindowID id, IStatusBarInterface * statusBarInterface, const CThemeThumbnail & themeThumbnail);
			~CThumbnailViewerVideo(void);
			wxString GetFilename()
			{
				return filename;
			}

			void SetFilename(const wxString &filename)
			{
				this->filename = filename;
			}
		private:

			void OnPictureClick(CThumbnailData * data);
			wxString filename = L"";
		};
	}
}


