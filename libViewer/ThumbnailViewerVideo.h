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
			CThumbnailViewerVideo(wxWindow* parent, wxWindowID id, const CThemeThumbnail& themeThumbnail,
			                      const bool& testValidity);
			~CThumbnailViewerVideo(void) override;

			wxString GetFilename()
			{
				return filename;
			}

			void SetFilename(const wxString& filename)
			{
				this->filename = filename;
			}

			int GetHeight() override
			{
				return GetIconeHeight();
			}

		private:
			void OnPictureClick(CThumbnailData* data) override;
			wxString filename;
		};
	}
}
