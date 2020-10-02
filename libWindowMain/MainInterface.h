#pragma once

class IMainInterface
{
public:

	virtual void ShowViewer() = 0;
	virtual void ShowAbout() = 0;
	virtual void HideAbout() = 0;
	virtual int Close() = 0;
    virtual wxString GetImageFilter() = 0;
    virtual wxString GetAppName() = 0;
	wxFrame * parent = nullptr;
};
