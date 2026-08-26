#include <header.h>
#include "WaitingWindow.h"
#include <FileUtility.h>
#include <wx/filename.h>
using namespace Regards::Viewer;

CWaitingWindow::CWaitingWindow(wxWindow* parent, wxWindowID id) : CWindowMain("CWaitingWindow", parent, id)
{
	textToShow = "Please wait ...";
	wxFileName resourcePath = wxFileName(CFileUtility::GetResourcesFolderPath(), "loading.gif");
	//m_animation = new wxAnimation(resourcePath + "/loading.gif");
	m_animationCtrl = std::make_unique<wxAnimationCtrl>(this, wxID_ANY);
	m_animationCtrl->SetSize(wxSize(640, 480));
	m_animationCtrl->Show(true);
	Connect(wxEVT_PAINT, wxPaintEventHandler(CWaitingWindow::on_paint));

	m_animationCtrl->SetBackgroundColour(wxColour("white"));
#ifdef __WXGTK__
	m_animationCtrl->Play();
#else
	m_animationCtrl->Play(true);
#endif
}

void CWaitingWindow::UpdateScreenRatio()
{
	Resize();
}

void CWaitingWindow::Resize()
{
#ifdef __WXGTK__
#if wxCHECK_VERSION(3, 1, 2)
	double scale_factor = 1.0f;
#else
	double scale_factor = GetContentScaleFactor();
#endif
#else
	const double scale_factor = 1.0f;
#endif
	wxRect rcAffichageBitmap;
	rcAffichageBitmap.x = 0;

	if (m_animationCtrl->IsShown())
	{
		const wxAnimation animation = m_animationCtrl->GetAnimation();
		if (animation.IsOk())
		{
			const wxSize animationSize = animation.GetSize();

			if (animationSize.GetWidth() <= 0 || animationSize.GetHeight() <= 0)
				return;

			const int xPos = rcAffichageBitmap.x + (GetWindowWidth() / scale_factor - animationSize.GetWidth()) / 2;
			const int yPos = (GetWindowHeight() / scale_factor - animationSize.GetHeight()) / 2;

			m_animationCtrl->SetSize(xPos, yPos, animationSize.GetWidth(), animationSize.GetHeight());
		}
		else
		{
			const wxSize animationSize = wxSize(640, 480);
			const int xPos = rcAffichageBitmap.x + (GetWindowWidth() / scale_factor - animationSize.GetWidth()) / 2;
			const int yPos = (GetWindowHeight() / scale_factor - animationSize.GetHeight()) / 2;
			m_animationCtrl->SetSize(xPos, yPos, animationSize.GetWidth(), animationSize.GetHeight());
		}
	}
};

void CWaitingWindow::SetTexte(const wxString& libelle)
{
	textToShow = libelle;
	needToRefresh = true;
}

void CWaitingWindow::on_paint(wxPaintEvent& event)
{
#ifdef __WXGTK__
#if wxCHECK_VERSION(3, 1, 2)
	double scale_factor = 1.0f;
#else
	double scale_factor = GetContentScaleFactor();
#endif
#else
	double scale_factor = 1.0f;
#endif
	CThemeFont font;
	wxPaintDC dc(this);
	//dc.SetBackgroundMode(wxTRANSPARENT);
	//dc.SetBackground(*wxTRANSPARENT_BRUSH);
	wxRect rc = GetRect();
	FillRect(&dc, rc, wxColour("white"));
	font.SetColorFont(wxColour("black"));

	const wxSize size = GetSizeTexte(&dc, textToShow, font);
	const int xPos = (GetWindowWidth() / scale_factor - size.x) / 2;

	const wxAnimation animation = m_animationCtrl->GetAnimation();
	if (animation.IsOk())
	{
		const wxSize animationSize = animation.GetSize();
		int yPos = (GetWindowHeight() / scale_factor - animationSize.GetHeight()) / 2;
		yPos -= size.y * 2;
		DrawTexte(&dc, textToShow, xPos, yPos, font);
	}

}
