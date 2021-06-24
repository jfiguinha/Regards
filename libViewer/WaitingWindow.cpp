#include <header.h>
#include "WaitingWindow.h"
#include <FileUtility.h>
using namespace Regards::Viewer;

CWaitingWindow::CWaitingWindow(wxWindow* parent, wxWindowID id) : CWindowMain("CWaitingWindow", parent, id)
{
	textToShow = "Please wait ...";
	const wxString resourcePath = CFileUtility::GetResourcesFolderPath();
	//m_animation = new wxAnimation(resourcePath + "/loading.gif");
	m_animationCtrl = new wxAnimationCtrl(this, wxID_ANY);
	m_animationCtrl->Show(true);
	Connect(wxEVT_PAINT, wxPaintEventHandler(CWaitingWindow::OnPaint));

#ifdef WIN32
	m_animationCtrl->LoadFile(resourcePath + "\\loading.gif");
#else
	m_animationCtrl->LoadFile(resourcePath + "//loading.gif");
#endif

	m_animationCtrl->SetBackgroundColour(wxColour("white"));
#ifdef __WXGTK__
	m_animationCtrl->Play();
#else
	m_animationCtrl->Play(true);
#endif
};

CWaitingWindow::~CWaitingWindow()
{
	if (m_animationCtrl != nullptr)
		delete m_animationCtrl;
};

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
	rcAffichageBitmap.y = 0;
	rcAffichageBitmap.width = GetWindowWidth() / scale_factor;
	rcAffichageBitmap.height = GetWindowHeight() / scale_factor;

	if (m_animationCtrl->IsShown())
	{
		const wxAnimation animation = m_animationCtrl->GetAnimation();
		const wxSize animationSize = animation.GetSize();

		const int xPos = rcAffichageBitmap.x + (GetWindowWidth() / scale_factor - animationSize.GetWidth()) / 2;
		const int yPos = (GetWindowHeight() / scale_factor - animationSize.GetHeight()) / 2;

		m_animationCtrl->SetSize(xPos, yPos, animationSize.GetWidth(), animationSize.GetHeight());
	}
};

void CWaitingWindow::SetTexte(const wxString& libelle)
{
	textToShow = libelle;
	Refresh();
}

void CWaitingWindow::OnPaint(wxPaintEvent& event)
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
	Regards::Viewer::CWaitingWindow::FillRect(&dc, rc, wxColour("white"));
	font.SetColorFont(wxColour("black"));

	const wxSize size = GetSizeTexte(&dc, textToShow, font);
	const int xPos = (GetWindowWidth() / scale_factor - size.x) / 2;

	const wxAnimation animation = m_animationCtrl->GetAnimation();
	const wxSize animationSize = animation.GetSize();
	int yPos = (GetWindowHeight() / scale_factor - animationSize.GetHeight()) / 2;
	yPos -= size.y * 2;
	DrawTexte(&dc, textToShow, xPos, yPos, font);
}
