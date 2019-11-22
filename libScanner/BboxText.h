#pragma once

class BBoxText
{
public:
	int id;
	wxRect rect;
	wxString label;
	int confidence;
	int numLigne;
	bool selected;
};
