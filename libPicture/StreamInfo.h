#pragma once

class CStreamInfo
{
public:
	CStreamInfo()
	{
		indexStream = 0;
		typeStream = 0;
		streamLibelle = "";
		selected = false;
	};

	~CStreamInfo()
	{
	};

	int indexStream;
	string streamLibelle;
	int typeStream;
	bool selected;
};

using listStream = vector<CStreamInfo>;
