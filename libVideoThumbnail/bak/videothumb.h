#pragma once
#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <string>
#include <vector>
using namespace std;

class CImageVideoThumbnail;
class CRegardsBitmap;
class CxImage;

namespace Regards{
	namespace Video{
		class CThumbnailVideo
		{
		public:
			CThumbnailVideo();
			~CThumbnailVideo();

			CxImage * GetVideoFrame(const wxString & fileName, int &rotation);
			void GetVideoDimensions(const wxString & fileName, int & width, int & height, int &rotation);
			CxImage * GetVideoFrame(const wxString & fileName, int &rotation, const int &percent, int &timePosition);
			vector<CImageVideoThumbnail *> GetVideoListFrame(const wxString & fileName, int &width, int &height);
		};
	}
}


