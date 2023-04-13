#pragma once

class ISCannerInterface
{
public:
	virtual int Close() = 0;
	virtual void ShowAbout() = 0;
	virtual void HideAbout() = 0;
	virtual void ShowViewer() = 0;
	wxFrame* parent = nullptr;
};


class IMainInterface : public ISCannerInterface
{
public:
	
	virtual wxString GetImageFilter() = 0;
	virtual wxString GetAppName() = 0;
};


class IVideoConverterInterface
{
public:
	virtual int Close() = 0;
	wxFrame* parent = nullptr;
};




