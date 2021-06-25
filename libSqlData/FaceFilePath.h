#pragma once

class CFaceFilePath
{
public:
	CFaceFilePath(): numFace(0), numPhoto(0)
	{
	}
	;

	~CFaceFilePath()
	{
	};

	int numFace;
	int numPhoto;
	wxString faceFilePath;
};
