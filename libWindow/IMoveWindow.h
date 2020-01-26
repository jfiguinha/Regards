#pragma once

namespace Regards
{
	namespace Window
	{
		class CSeparationBar;
	}
}

class IMoveWindow
{
public:
	virtual void SetNewPosition(Regards::Window::CSeparationBar * separationBar) = 0;
	virtual bool OnLButtonDown() = 0;
	virtual void OnLButtonUp() = 0;
};

