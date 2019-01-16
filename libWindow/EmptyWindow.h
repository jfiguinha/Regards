#pragma once
#include "WindowMain.h"
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

