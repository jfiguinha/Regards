#pragma once
#include <RegardsBitmap.h>
#include <string>
#include <mutex>
using namespace std;

class IStatusBarInterface
{
public:
	virtual void SetText(const int &numPos, const wxString &libelle) = 0;
	virtual void SetRangeProgressBar(const int &range) = 0;
	virtual void SetPosProgressBar(const int &position) = 0;
	virtual void SetWindowTitle(const wxString &libelle) = 0;
	virtual void SetFullscreen() = 0;
	virtual void SetScreen() = 0;
	virtual void PrintPreview(CRegardsBitmap * imageToPrint) = 0;
	virtual void Exit() = 0;
	virtual void ShowViewer() = 0;

};