#pragma once

#define THUMBNAIL 1
#define CATEGORY 2
#define INFOS 3

class wxWindow;

#include <atomic>
#include <thread>

struct AtomicCounter {
	std::atomic<int> value;

	void increment(){
		++value;
	}

	void decrement(){
		--value;
	}

	int get(){
		return value.load();
	}
};

class CThreadData
{
public:
	CThreadData(){};
	virtual ~CThreadData()
	{
		/*
		if (thread != nullptr)
		{
			if (thread->joinable())
				thread->join();

			delete thread;
			thread = nullptr;
		}
		*/
	};
	virtual void ProcessThread() = 0;
	bool start = false;
	wxWindow * main = nullptr;
	std::thread * thread = nullptr;
	int threadType = 0;
};

