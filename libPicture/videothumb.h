#pragma once

class CImageVideoThumbnail;
class CRegardsBitmap;
class CFFmpegDecodeFrame;
class CVideoEffectParameter;

class CThumbnailVideo
{
public:
	CThumbnailVideo(const wxString& fileName, const wxString& decoder = "");
	~CThumbnailVideo();
	bool IsHardwareDecoderCompatible();
	CRegardsBitmap* GetVideoFrame(const int& thumbnailWidth, const int& thumbnailHeight, int& rotation, int percent = 10);
	CRegardsBitmap* GetVideoFrame(const int& timePosition, const int& thumbnailWidth, const int& thumbnailHeight, const bool& copy = false);
	void GetVideoDimensions(int& width, int& height, int& rotation);
	CRegardsBitmap* GetVideoFrame(int& rotation, const int& percent, int& timePosition, const int& thumbnailWidth,
	                              const int& thumbnailHeight);
	vector<CImageVideoThumbnail*> GetVideoListFrame(const int& widthThumbnail, const int& heightThumbnail,
	                                                const bool& compressJpeg);
	int64_t GetMovieDuration();
	bool IsOk();
private:
	//void ApplyEffectToBitmap(CRegardsBitmap* & bitmap, CVideoEffectParameter* videoEffectParameter);
	wxString filename;
	CFFmpegDecodeFrame* decodeFrame;

	CRegardsBitmap* TestIsValidBitmap(CRegardsBitmap* in);
};
