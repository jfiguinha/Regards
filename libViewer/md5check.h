#pragma once

class CThreadMD5
{
public:
	CThreadMD5()
	{
		thread = nullptr;
		mainWindow = nullptr;
	}

	~CThreadMD5()
	{
	}

	wxString filename;
	std::thread* thread;
	wxWindow* mainWindow;
};


class CMd5Check
{
public:
	CMd5Check();
	~CMd5Check();
	static void CheckMD5(void* param);
};