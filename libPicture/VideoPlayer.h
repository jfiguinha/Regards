#pragma once
#include "VideoPlayerInterface.h"

class CVideoPlayerPimpl;

namespace cv
{
	class Mat;
}

namespace Regards
{
	namespace Video
	{
		class CVideoPlayer : public IVideoPlayer
		{
		public:
			CVideoPlayer(const wxString& filename);
			virtual ~CVideoPlayer();

			bool isOpened() override;
			void SeekToBegin() override;

			void SkipFrame(const int& nbFrame) override;
			int SeekToPos(const int& sec) override;

			int GetDuration() override;
			int GetFps() override;
			int GetTotalFrame() override;
			int GetWidth() override;
			int GetHeight() override;

			bool IsOk() override;

			void GetAspectRatio(int& ascpectNominator, int& ascpectDenominator) override;

			int GetOrientation() override;
			cv::Mat GetVideoFrame(const bool& applyOrientation = true, const bool & invertRotation = true) override;
			

		private:
			std::unique_ptr<CVideoPlayerPimpl> pimpl;
			wxString filename;
		};
	}
}
