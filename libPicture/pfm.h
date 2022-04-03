#pragma once

class CRegardsFloatBitmap;

class CPfm
{
public:
	CPfm() {};
	virtual ~CPfm() {};
	static void GetDimensions(const wxString & path, int & width, int & height);
	static CRegardsFloatBitmap* ReadFilePFM(const wxString& path, const bool& thumbnail);
	static int WriteFilePFM(CRegardsFloatBitmap * im, const wxString & path, float scalef);
};