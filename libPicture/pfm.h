#pragma once

class CRegardsFloatBitmap;

class CPfm
{
public:
	CPfm() {};
	virtual ~CPfm() {};
	static void GetDimensions(const wxString & path, int & width, int & height);
	static cv::Mat ReadFilePFM(const wxString& path, const bool& thumbnail);
	static int WriteFilePFM(const cv::Mat & im, const wxString & path, float scalef);
};