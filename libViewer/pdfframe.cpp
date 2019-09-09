/*
 * This file is part of diff-pdf.
 *
 * Copyright (C) 2009 TT-Solutions.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <header.h>
#include "pdfframe.h"
#include "bmpviewer.h"
#include "gutter.h"

#include <stdio.h>
#include <assert.h>

#include <vector>

#include <cairo/cairo.h>
#include <cairo/cairo-pdf.h>

#include <wx/app.h>
#include <wx/evtloop.h>
#include <wx/cmdline.h>
#include <wx/filename.h>
#include <wx/log.h>
#include <wx/frame.h>
#include <wx/sizer.h>
#include <wx/toolbar.h>
#include <wx/artprov.h>
#include <wx/progdlg.h>
#include <wx/filesys.h>

 // ------------------------------------------------------------------------
 // PDF rendering functions
 // ------------------------------------------------------------------------

bool g_verbose = false;

// Resolution to use for rasterization, in DPI
#define RESOLUTION  300

DiffFrame::DiffFrame(const wxString& title) : wxFrame(NULL, wxID_ANY, title)
{
	m_cur_page = -1;

	CreateStatusBar(2);
	SetStatusBarPane(0);
	const int stat_widths[] = { -1, 150 };
	SetStatusWidths(2, stat_widths);

	wxToolBar *toolbar =
		new wxToolBar
		(
			this, wxID_ANY,
			wxDefaultPosition, wxDefaultSize,
			wxTB_HORIZONTAL | wxTB_FLAT | wxTB_HORZ_TEXT
		);

	toolbar->AddTool(ID_PREV_PAGE, wxT("Previous"), BMP_PREV_PAGE,
		wxT("Go to previous page (PgUp)"));
	toolbar->AddTool(ID_NEXT_PAGE, wxT("Next"), BMP_NEXT_PAGE,
		wxT("Go to next page (PgDown)"));
	toolbar->AddTool(ID_ZOOM_IN, wxT("Zoom in"), BMP_ZOOM_IN,
		wxT("Make the page larger (Ctrl +)"));
	toolbar->AddTool(ID_ZOOM_OUT, wxT("Zoom out"), BMP_ZOOM_OUT,
		wxT("Make the page smaller (Ctrl -)"));
	toolbar->AddTool(ID_OFFSET_LEFT, wxT(""), BMP_OFFSET_LEFT,
		wxT("Offset one of the pages to the left (Ctrl left)"));
	toolbar->AddTool(ID_OFFSET_RIGHT, wxT(""), BMP_OFFSET_RIGHT,
		wxT("Offset one of the pages to the right (Ctrl right)"));
	toolbar->AddTool(ID_OFFSET_UP, wxT(""), BMP_OFFSET_UP,
		wxT("Offset one of the pages up (Ctrl up)"));
	toolbar->AddTool(ID_OFFSET_DOWN, wxT(""), BMP_OFFSET_DOWN,
		wxT("Offset one of the pages down (Ctrl down)"));

	toolbar->Realize();
	SetToolBar(toolbar);

	wxAcceleratorEntry accels[8];
	accels[0].Set(wxACCEL_NORMAL, WXK_PAGEUP, ID_PREV_PAGE);
	accels[1].Set(wxACCEL_NORMAL, WXK_PAGEDOWN, ID_NEXT_PAGE);
	accels[2].Set(wxACCEL_CTRL, (int)'=', ID_ZOOM_IN);
	accels[3].Set(wxACCEL_CTRL, (int)'-', ID_ZOOM_OUT);
	accels[4].Set(wxACCEL_CTRL, WXK_LEFT, ID_OFFSET_LEFT);
	accels[5].Set(wxACCEL_CTRL, WXK_RIGHT, ID_OFFSET_RIGHT);
	accels[6].Set(wxACCEL_CTRL, WXK_UP, ID_OFFSET_UP);
	accels[7].Set(wxACCEL_CTRL, WXK_DOWN, ID_OFFSET_DOWN);

	wxAcceleratorTable accel_table(8, accels);
	SetAcceleratorTable(accel_table);

	m_gutter = new Gutter(this, ID_GUTTER);

	m_viewer = new BitmapViewer(this);
	m_viewer->AttachGutter(m_gutter);
	m_viewer->SetFocus();

	wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(m_gutter, wxSizerFlags(0).Expand().Border(wxALL, 2));
	sizer->Add(m_viewer, wxSizerFlags(1).Expand());
	SetSizer(sizer);
}

void DiffFrame::SetDocs(const wxString &filename)
{
	const wxString url1 = wxFileSystem::FileNameToURL(filename);
	/*
	PopplerDocument *doc1 = poppler_document_new_from_file(url1.utf8_str(), NULL, &err);
	m_doc1 = doc1;

	m_diff_count = 0;
	for (std::vector<bool>::const_iterator i = m_pages.begin();
		i != m_pages.end();
		++i)
	{
		if (*i)
			m_diff_count++;
	}

	GoToPage(0);
	*/
	m_viewer->SetBestFitZoom();
	UpdateStatus();

	//progress.Hide();
	
}

void DiffFrame::GoToPage(int n)
{
	m_cur_page = n;
	m_gutter->SetSelection(n);
	DoUpdatePage();
}

void DiffFrame::DoUpdatePage()
	{
		wxBusyCursor wait;
		wxImage thumbnail;
		/*
		const int pages1 = poppler_document_get_n_pages(m_doc1);
		PopplerPage *page1 = m_cur_page < pages1 
			? poppler_document_get_page(m_doc1, m_cur_page)
			: NULL;
		cairo_surface_t *img1 = page1 ? render_page(page1) : NULL;
		
		m_viewer->Set(img1);
		m_gutter->SetThumbnail(m_cur_page, thumbnail);

		const int pages1 = poppler_document_get_n_pages(m_doc1);
		const int pages2 = poppler_document_get_n_pages(m_doc2);

		PopplerPage *page1 = m_cur_page < pages1
			? poppler_document_get_page(m_doc1, m_cur_page)
			: NULL;
		PopplerPage *page2 = m_cur_page < pages2
			? poppler_document_get_page(m_doc2, m_cur_page)
			: NULL;

		cairo_surface_t *img1 = page1 ? render_page(page1) : NULL;
		cairo_surface_t *img2 = page2 ? render_page(page2) : NULL;

		wxImage thumbnail;
		cairo_surface_t *diff = diff_images
		(
			img1, img2,
			m_offset.x, m_offset.y,
			&thumbnail, Gutter::WIDTH
		);

		m_viewer->Set(diff ? diff : img1);

		// Always update the diff map. It will be all-white if there were
		// no differences.
		m_gutter->SetThumbnail(m_cur_page, thumbnail);
	
		if (img1)
			cairo_surface_destroy(img1);

				*/
		UpdateStatus();
	}

	void DiffFrame::UpdateStatus()
	{
		SetStatusText
		(
			wxString::Format
			(
				wxT("Page %d of %d; %d of them %s different, this page %s"),
				m_cur_page + 1 /* humans prefer 1-based counting*/,
				(int)m_pages.size(),
				m_diff_count,
				m_diff_count == 1 ? wxT("is") : wxT("are"),
				m_pages[m_cur_page] ? wxT("differs") : wxT("is unchanged")
			),
			0
		);

		SetStatusText
		(
			wxString::Format
			(
				wxT("%.1f%% [offset %d,%d]"),
				m_viewer->GetZoom() * 100.0,
				m_offset.x, m_offset.y
			),
			1
		);
	}

	void DiffFrame::OnSetPage(wxCommandEvent& event)
	{
		GoToPage(event.GetSelection());
	}

	void DiffFrame::OnPrevPage(wxCommandEvent&)
	{
		if (m_cur_page > 0)
			GoToPage(m_cur_page - 1);
	}

	void DiffFrame::OnNextPage(wxCommandEvent&)
	{
		if (m_cur_page < m_pages.size() - 1)
			GoToPage(m_cur_page + 1);
	}

	void DiffFrame::OnUpdatePrevPage(wxUpdateUIEvent& event)
	{
		event.Enable(m_cur_page > 0);
	}

	void DiffFrame::OnUpdateNextPage(wxUpdateUIEvent& event)
	{
		event.Enable(m_cur_page < m_pages.size() - 1);
	}

	void DiffFrame::OnZoomIn(wxCommandEvent&)
	{
		wxBusyCursor wait;
		m_viewer->SetZoom(m_viewer->GetZoom() * ZOOM_FACTOR_STEP);
		UpdateStatus();
	}

	void DiffFrame::OnZoomOut(wxCommandEvent&)
	{
		wxBusyCursor wait;
		m_viewer->SetZoom(m_viewer->GetZoom() / ZOOM_FACTOR_STEP);
		UpdateStatus();
	}

	void DiffFrame::DoOffset(int x, int y)
	{
		m_offset.x += x;
		m_offset.y += y;
		DoUpdatePage();
	}

	void DiffFrame::OnOffsetLeft(wxCommandEvent&) { DoOffset(-1, 0); }
	void DiffFrame::OnOffsetRight(wxCommandEvent&) { DoOffset(1, 0); }
	void DiffFrame::OnOffsetUp(wxCommandEvent&) { DoOffset(0, -1); }
	void DiffFrame::OnOffsetDown(wxCommandEvent&) { DoOffset(0, 1); }