#pragma once

class CStatusText
{
public:
	CStatusText()
	{
		position = 0;
		text = "";
	}

	~CStatusText()
	{
	}

	int position;
	wxString text;
};
