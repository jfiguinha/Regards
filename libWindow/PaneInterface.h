// Pane.h: interface for the CPane class.
//
//////////////////////////////////////////////////////////////////////
//tototo
#pragma once

class CPaneInterface
{

public:

	virtual void ClosePane(const int &id, const int &refresh = 1) = 0;
	virtual void RefreshPane(const int& id) = 0;
};


