// Pane.h: interface for the CPane class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

class CTitleBarInterface
{
public:
	virtual ~CTitleBarInterface() = default;
	virtual void ClosePane() = 0;
	virtual void RefreshPane() = 0;
};
