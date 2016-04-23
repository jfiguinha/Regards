#pragma once
#include "WindowMain.h"
#include <string>
using namespace std;
using namespace Regards::Window;

class CEmptyWindow : public CWindowMain
{
public:
	CEmptyWindow(wxWindow* parent, wxWindowID id);
	~CEmptyWindow();

	void OnPaint(wxPaintEvent& event);

private:

	void Resize();
};

