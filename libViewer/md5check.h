#pragma once

class CThreadMD5
{
public:
	CThreadMD5()
	{
		thread = nullptr;
		mainWindow = nullptr;
	}

	wxString filename;
	std::thread* thread;
	wxWindow* mainWindow;
};


class CMd5Check
{
public:
	static void CheckMD5(void* param);
};