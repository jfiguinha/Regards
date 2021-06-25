#pragma once

class CFaceName
{
public:
	CFaceName(): numFace(0), isSelectable(0)
	{
	}
	;

	~CFaceName()
	{
	};

	int numFace;
	wxString faceName;
	int isSelectable;
};
