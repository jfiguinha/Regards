#pragma once
#include "GLTexture.h"
using namespace Regards::OpenGL;

extern "C" {
#include <libswscale/swscale.h>
#include <libavcodec/avcodec.h>
}

class CRegardsBitmap;

class CffmpegToBitmap
{
public:
	CffmpegToBitmap(const bool& rgba = false);
	~CffmpegToBitmap();
	int GetVideoWidth();
	int GetVideoHeight();
	int GetThumbnailWidth();
	int GetThumbnailHeight();
	void DeleteData();
	void InitContext(AVFrame* src_frame, const bool& bicubic, const int& thumbnailWidth, const int& thumbnailHeight);
	int GetConvert(cv::Mat& bitmap, AVFrame* src_frame, const int& thumbnailWidth, const int& thumbnailHeight);
	cv::Mat GetConvert(AVFrame* src_frame, const int& thumbnailWidth, const int& thumbnailHeight);
	cv::Mat GetConvert(AVFrame* src_frame);

private:
	SwsContext* scaleContext = nullptr;
	int videoFrameWidth;
	int videoFrameHeight;
	int videoFrameOutputWidth;
	int videoFrameOutputHeight;
};
