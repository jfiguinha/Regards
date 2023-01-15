#pragma once

namespace Regards::Window
{
	class CSeparationBar;

	class IMoveWindow
	{
	public:
		virtual ~IMoveWindow() = default;
		virtual void SetNewPosition(CSeparationBar* separationBar) = 0;
		virtual bool OnLButtonDown() = 0;
		virtual void OnLButtonUp() = 0;
	};

}


