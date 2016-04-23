#pragma once
#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include "ThumbnailData.h"
#include <wx/fs_mem.h>

class CThumbnailDataStorage : public CThumbnailData
{
public:
	CThumbnailDataStorage(const wxString & filename);
	~CThumbnailDataStorage(void);

	int GetType()
	{
		return 1;
	}

	void SetBitmap(CRegardsBitmap * bitmap);
	CRegardsBitmap * GetRegardsBitmap(const int &type = 3);
	wxImage GetwxImage();

private:

	bool TestBitmap();
	CRegardsBitmap * bitmap = nullptr;
};

