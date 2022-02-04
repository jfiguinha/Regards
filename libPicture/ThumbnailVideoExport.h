#pragma once

class CRegardsBitmap;

class CThumbnailVideoExport
{
public:
	CThumbnailVideoExport();
	~CThumbnailVideoExport();
	static int GenerateVideoFromList(const wxString& outfile, vector<wxString>& listOfFile, int delay, int fps, int width, int height, int effect);

};