#pragma once

class CThreadLoadingBitmap
{
public:
	CThreadLoadingBitmap() : percent(0), typeElement(0), photoId(0), timePosition(0), _threadVideo(nullptr)
	{
		_thread = nullptr;
		window = nullptr;
	}
	;

	~CThreadLoadingBitmap()
	{
		if (_thread != nullptr)
		{
			_thread->join();
			delete _thread;
			_thread = nullptr;
		}
		if (_threadVideo != nullptr)
		{
			_threadVideo->join();
			delete _threadVideo;
			_threadVideo = nullptr;
		}
	};

	int percent;
	wxString filename;
	int typeElement;
	int type;
	int photoId;
	int timePosition;
	bool isAnimationOrVideo = false;
	wxImage bitmapIcone;
	thread* _thread;
	thread* _threadVideo;
	wxWindow * window;
};