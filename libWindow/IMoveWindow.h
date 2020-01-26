#pragma once

namespace Regards
{
	namespace Window
	{
		class CSeparationBar;
	}
}

using namespace Regards::Window;

class IMoveWindow
{
public:
	virtual void SetNewPosition(CSeparationBar * separtionBar) = 0;
	virtual void UpdatePosition(CSeparationBar * separtionBar) = 0;
	virtual bool OnLButtonDown(CSeparationBar * separtionBar) = 0;
	virtual void OnLButtonUp(CSeparationBar * separtionBar) = 0;
};

