#pragma once
#pragma once

namespace cv
{
	class Mat;
	class VideoCapture;
}

namespace Regards
{
	namespace Video
	{
		class IVideoPlayer
		{
		public:
			IVideoPlayer(const wxString& filename)
			{
				
			}

			virtual ~IVideoPlayer()
			{
				
			};

			virtual bool isOpened() = 0;
			virtual void SeekToBegin() = 0;

			virtual void SkipFrame(const int& nbFrame) = 0;
			virtual int SeekToPos(const int& sec) = 0;

			virtual int GetDuration() = 0;
			virtual int GetFps() = 0;
			virtual int GetTotalFrame() = 0;
			virtual int GetWidth() = 0;
			virtual int GetHeight() = 0;

			virtual bool IsOk() = 0;

			virtual void GetAspectRatio(int& ascpectNominator, int& ascpectDenominator) = 0;

			virtual int GetOrientation() = 0;
			virtual cv::Mat GetVideoFrame(const bool& applyOrientation = true, const bool& invertRotation = true) = 0;

		};
	}
}
