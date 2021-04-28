#pragma once

#include <stdint.h>
#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <Metadata.h>
#include <vector>
using namespace std;

class CRegardsBitmap;
class CExiv;
class CxImage;


class CPictureMetadata
{
public:
	CPictureMetadata(const wxString &filename);
	~CPictureMetadata();

	bool HasExif();
	bool HasThumbnail();
	vector<CMetadata> GetMetadata();
	bool CopyMetadata(const wxString &output);
	CxImage * DecodeThumbnail(wxString &extension);
    int Orientation();
	void ReadVideo(bool & hasGps, bool & hasDataTime, wxString &dateTimeInfos, wxString &latitude, wxString &longitude);
	void ReadPicture(bool & hasGps, bool & hasDataTime, wxString &dateTimeInfos, wxString &latitude, wxString &longitude);

	static void WriteExif(const std::string& fileName, const int& orientation);

private:

	CExiv * exivData;
	bool isExif;
};