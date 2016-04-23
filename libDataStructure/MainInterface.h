#pragma once

class IMainInterface
{
public:
	virtual void ShowViewer() = 0;
	virtual void UpdateViewer() = 0;
	virtual void HideViewer() = 0;
	virtual void ShowExplorer() = 0;
	virtual void HideExplorer() = 0;
	virtual void ShowAbout() = 0;
	virtual void HideAbout() = 0;
	virtual int Close() = 0;
};