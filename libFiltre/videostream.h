#pragma once

#include <StreamInfo.h>
using namespace std;

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


