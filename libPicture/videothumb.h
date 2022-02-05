#pragma once

class CImageVideoThumbnail;
class CRegardsBitmap;
class CFFmpegDecodeFrame;
class CVideoEffectParameter;

class CThumbnailVideo
{
public:
	CThumbnailVideo();
	~CThumbnailVideo();
	void SetFilename(const wxString& fileName);
	CRegardsBitmap* GetVideoFrame(const int& timePosition, const int& thumbnailWidth, const int& thumbnailHeight);
	void GetVideoDimensions(int& width, int& height, int& rotation);
	int64_t GetMovieDuration();
	int GetVideoOrientation();
	vector<CImageVideoThumbnail*> GetVideoListFrame(const int& widthThumbnail, const int& heightThumbnail, const bool& compressJpeg);
private:
	wxString filename;

};
