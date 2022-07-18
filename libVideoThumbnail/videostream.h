#pragma once

class CStreamInfo;

namespace Regards{
	namespace Video{
		class CVideoStream
		{
		public:
			CVideoStream();
			~CVideoStream();

			static vector<CStreamInfo> GetVideoStream(const wxString & fileName);
		};
	}
}


