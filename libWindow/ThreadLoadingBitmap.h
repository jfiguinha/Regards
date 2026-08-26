#pragma once

class CThreadLoadingBitmap
{
public:
	CThreadLoadingBitmap() : percent(0), typeElement(0), photoId(0), timePosition(0)
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
		}
	};

	int percent;
	wxString filename;
	int typeElement;
	int type;
	long longWindow;
	int photoId;
	int timePosition;
	bool isAnimationOrVideo = false;
	cv::Mat bitmapIcone;
	std::unique_ptr<std::thread> _thread;
	wxWindow * window;

};