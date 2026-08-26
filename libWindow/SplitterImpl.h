#pragma once
#include <memory>
#include <wx/wx.h>
class CThemeSplitter;

// Forward declarations
namespace Regards::Window
{
	class CSeparationBar;


	/**
	 * @class CSplitterImpl
	 * @brief Implementation details of CSplitter (PIMPL pattern)
	 * 
	 * This class contains all internal state and implementation details
	 * separated from the public interface for better encapsulation and
	 * to reduce compilation dependencies.
	 */
	class CSplitterImpl
	{
	public:
		explicit CSplitterImpl(const CThemeSplitter& theme);
		~CSplitterImpl() = default;

		// Member variables (previously private/protected)
		int posBar;
		bool horizontal;
		int oldWidth;
		int oldHeight;
		wxWindow* window1;
		wxWindow* window2;
		std::unique_ptr<CSeparationBar> separationBar;

		// Implementation details
		bool fixPosition;
		wxBitmap renderBitmap;
		int taille;
		int posWindow;
		int diff;
		bool fixWindow1Size;
		int window1Size;
		bool fixWindow2Size;
		int window2Size;
		bool windowResize;
		bool fastRender;
		bool moving;
		int windowMinimalSize;
		bool showSeparationBar;
		const CThemeSplitter& themeSplitter;
	};
} // namespace Regards::Window
