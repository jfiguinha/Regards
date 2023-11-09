#pragma once


class CMemBitmap
{
public:
	CMemBitmap(int width, int height)
	{
		memBitmap = wxBitmap(width, height, 32);
		sourceDCContext.SelectObjectAsSource(memBitmap);
	}

	void SetImage(wxImage & image)
	{
		memBitmap = wxBitmap(image);
		sourceDCContext.SelectObjectAsSource(memBitmap);
	}

	void SetBitmap(const wxBitmap & image)
	{
		memBitmap = wxBitmap(image);
		sourceDCContext.SelectObjectAsSource(memBitmap);
	}

	void SetWindowSize(int width, int height)
	{
		memBitmap = wxBitmap(width, height, 32);
		sourceDCContext.SelectObjectAsSource(memBitmap);


	}

	~CMemBitmap()
	{
		sourceDCContext.SelectObject(wxNullBitmap);
	}

	wxBitmap memBitmap;
	wxMemoryDC sourceDCContext;
	int width = 0;
	int height = 0;
};