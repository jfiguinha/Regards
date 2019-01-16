#pragma once
#include <thread>
using namespace std;

class CWindowMainPimpl
{
public:
	CWindowMainPimpl()
	{
		threadIdle = nullptr;
		width = 0;
		height = 0;
		sleeptime = 50;
	}

	void DeleteThread()
	{
		if (threadIdle != nullptr)
		{
			threadIdle->join();
			delete(threadIdle);
			threadIdle = nullptr;
		}
	}

	thread * threadIdle;
	int width;
	int height;
	unsigned long sleeptime;
};