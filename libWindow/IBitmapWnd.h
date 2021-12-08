#pragma once
#include "IBitmapRenderInterface.h"

class IBitmapWnd
{
public:
	IBitmapWnd() {};
	~IBitmapWnd() {};

	virtual void SetBitmapRenderInterface(IBitmapRenderInterface* bitmapWndRender) = 0;

	virtual void UpdateRenderInterface(IBitmapRenderInterface* bitmapWndRender) = 0;

	virtual IBitmapRenderInterface* GetWndPt() = 0;

};