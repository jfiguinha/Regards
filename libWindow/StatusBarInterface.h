#pragma once
class CImageLoadingFormat;
;

class IStatusBarInterface
{
public:
	virtual ~IStatusBarInterface() = default;
	virtual void SetText(const int& numPos, const wxString& libelle) = 0;
	virtual void SetRangeProgressBar(const int& range) = 0;
	virtual void SetPosProgressBar(const int& position) = 0;
	virtual void SetWindowTitle(const wxString& libelle) = 0;

	virtual void SetFullscreen()
	{
	};

	virtual void SetScreen()
	{
	};
	virtual int ShowScanner() { return -1; };

	virtual void PrintPreview(const wxString& filename)
	{
	};

	virtual void PrintImagePreview(cv::Mat &picture)
	{
	};
	virtual void Exit() = 0;

	virtual void ShowViewer()
	{
	};
	virtual bool AddFSEntry(const wxString& dirPath) { return true; };
	virtual bool RemoveFSEntry(const wxString& dirPath) { return true; };
};
