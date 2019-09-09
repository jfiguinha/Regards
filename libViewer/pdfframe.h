#pragma once
// ------------------------------------------------------------------------
// wxWidgets GUI
// ------------------------------------------------------------------------
class BitmapViewer;
class Gutter;

const int ID_PREV_PAGE = wxNewId();
const int ID_NEXT_PAGE = wxNewId();
const int ID_ZOOM_IN = wxNewId();
const int ID_ZOOM_OUT = wxNewId();
const int ID_OFFSET_LEFT = wxNewId();
const int ID_OFFSET_RIGHT = wxNewId();
const int ID_OFFSET_UP = wxNewId();
const int ID_OFFSET_DOWN = wxNewId();
const int ID_GUTTER = wxNewId();

#define BMP_ARTPROV(id) wxArtProvider::GetBitmap(id, wxART_TOOLBAR)

#define BMP_PREV_PAGE      BMP_ARTPROV(wxART_GO_BACK)
#define BMP_NEXT_PAGE      BMP_ARTPROV(wxART_GO_FORWARD)

#define BMP_OFFSET_LEFT    BMP_ARTPROV(wxART_GO_BACK)
#define BMP_OFFSET_RIGHT   BMP_ARTPROV(wxART_GO_FORWARD)
#define BMP_OFFSET_UP      BMP_ARTPROV(wxART_GO_UP)
#define BMP_OFFSET_DOWN    BMP_ARTPROV(wxART_GO_DOWN)

#ifdef __WXGTK__
#define BMP_ZOOM_IN    BMP_ARTPROV(wxT("gtk-zoom-in"))
#define BMP_ZOOM_OUT   BMP_ARTPROV(wxT("gtk-zoom-out"))
#else
#include "gtk-zoom-in.xpm"
#include "gtk-zoom-out.xpm"
#define BMP_ZOOM_IN    wxBitmap(gtk_zoom_in_xpm)
#define BMP_ZOOM_OUT   wxBitmap(gtk_zoom_out_xpm)
#endif

static const float ZOOM_FACTOR_STEP = 1.2f;

class DiffFrame : public wxFrame
{
public:

	DiffFrame(const wxString& title);
	void SetDocs(const wxString &filename);
	void GoToPage(int n);

private:

	void DoUpdatePage();

	void UpdateStatus();

	void OnSetPage(wxCommandEvent& event);
	
	void OnPrevPage(wxCommandEvent&);

	void OnNextPage(wxCommandEvent&);

	void OnUpdatePrevPage(wxUpdateUIEvent& event);

	void OnUpdateNextPage(wxUpdateUIEvent& event);

	void OnZoomIn(wxCommandEvent&);

	void OnZoomOut(wxCommandEvent&);

	void DoOffset(int x, int y);

	void OnOffsetLeft(wxCommandEvent&);
	void OnOffsetRight(wxCommandEvent&);
	void OnOffsetUp(wxCommandEvent&);
	void OnOffsetDown(wxCommandEvent&);

	DECLARE_EVENT_TABLE()

private:

	//PopplerDocument *m_doc1;
	BitmapViewer *m_viewer;
	Gutter *m_gutter;
	//PopplerDocument *m_doc1, *m_doc2;
	std::vector<bool> m_pages;
	int m_diff_count;
	int m_cur_page;
	wxPoint m_offset;
};

BEGIN_EVENT_TABLE(DiffFrame, wxFrame)
EVT_LISTBOX(ID_GUTTER, DiffFrame::OnSetPage)
EVT_TOOL(ID_PREV_PAGE, DiffFrame::OnPrevPage)
EVT_TOOL(ID_NEXT_PAGE, DiffFrame::OnNextPage)
EVT_UPDATE_UI(ID_PREV_PAGE, DiffFrame::OnUpdatePrevPage)
EVT_UPDATE_UI(ID_NEXT_PAGE, DiffFrame::OnUpdateNextPage)
EVT_TOOL(ID_ZOOM_IN, DiffFrame::OnZoomIn)
EVT_TOOL(ID_ZOOM_OUT, DiffFrame::OnZoomOut)
EVT_TOOL(ID_OFFSET_LEFT, DiffFrame::OnOffsetLeft)
EVT_TOOL(ID_OFFSET_RIGHT, DiffFrame::OnOffsetRight)
EVT_TOOL(ID_OFFSET_UP, DiffFrame::OnOffsetUp)
EVT_TOOL(ID_OFFSET_DOWN, DiffFrame::OnOffsetDown)
END_EVENT_TABLE()