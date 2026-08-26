#pragma once
#include "IMoveWindow.h"
#include "WindowMain.h"
#include "SplitterImpl.h"
#include <memory>
namespace Regards::Window
{
	// Forward declaration
	class CSeparationBar;
	

	/**
	 * @class CSplitter
	 * @brief A splitter window that manages two child windows with a movable separator
	 * 
	 * Uses the PIMPL pattern to hide implementation details and reduce compilation dependencies.
	 */
	class CSplitter : public CWindowMain, public IMoveWindow
	{
	public:
		CSplitter(wxWindow* parent, wxWindowID id, const CThemeSplitter& theme, const bool& horizontal = false);
		~CSplitter() = default;

		// Window management
		void SetWindow(wxWindow* window1, wxWindow* window2);
		void SetWindow1FixPosition(const bool& fixPosition, const int& windowSize);
		void SetWindow2FixPosition(const bool& fixPosition, const int& windowSize);

		// Query methods
		bool IsAllClose() const;
		int GetPosition() const;
		bool GetSeparationVisibility() const;

		// Configuration
		void SetFixPosition(const bool& value);
		virtual void SetHorizontal(const bool& horizontal);
		void SetPosition(const int& pos);
		void SetSeparationBarVisible(const bool& visible);

		// Rendering
		void GenerateHorizontalRenderBitmap();
		void GenerateVerticalRenderBitmap();

		// Event handlers (IMoveWindow interface)
		bool OnLButtonDown() override;
		void OnLButtonUp() override;
		void SetNewPosition(CSeparationBar* separationBar) override;
		void Resize() override;
		void Resize(wxWindow* window);

		// Layout
		void UpdateScreenRatio() override = 0;

	protected:
		virtual void ResizeEvent();

		// Access to implementation (for derived classes)
		CSeparationBar* GetSeparationBar() { return m_impl->separationBar.get(); }
		const CSeparationBar* GetSeparationBar() const { return m_impl->separationBar.get(); }

		bool GetHorizontal() const { return m_impl->horizontal; }
		void SetHorizontalImpl(bool value) { m_impl->horizontal = value; }

		int& GetPosBar() { return m_impl->posBar; }
		const int& GetPosBar() const { return m_impl->posBar; }

		const std::unique_ptr<CSplitterImpl>& GetImpl() const { return m_impl; }
		std::unique_ptr<CSplitterImpl>& GetImpl() { return m_impl; }

	private:
		static constexpr int DEFAULT_MINIMAL_WINDOW_SIZE = 100;

		void ResizeVertical();
		void ResizeHorizontal();
		void DrawSeparationBar(const int& x, const int& y, const int& width, const int& height);

		// Implementation (PIMPL)
		std::unique_ptr<CSplitterImpl> m_impl;
	};
} // namespace Regards::Window

