#include "ListPicture.h"
#include <FileUtility.h>
#include <SQLRemoveData.h>
#include <SqlFindCriteria.h>
#include <ThumbnailData.h>
#include "ExplorerTheme.h"
#include "ExplorerThemeInit.h"
#include <libResource.h>
#include "PreviewThumbnail.h"
#if defined(__WXMSW__)
#include "../include/window_id.h"
#else
#include <window_id.h>
#endif
#include <CopyFileDlg.h>
#include <ExportFile.h>
#include <ConvertUtility.h>
#include <wx/dir.h>
#include <libPicture.h>
#include <MapSelect.h>
#include <CalendarSelect.h>
wxDEFINE_EVENT(EVENT_REFRESHVIEWER, wxCommandEvent);

using namespace Regards::Sqlite;
using namespace Regards::Window;
using namespace Regards::Explorer;

CListPicture::CListPicture(wxWindow* parent, wxWindowID id, IStatusBarInterface * statusbar)
	: CWindowMain(parent, id)
{
	
	CExplorerTheme * explorerTheme = CExplorerThemeInit::getInstance();
    Connect(EVENT_REFRESHVIEWER, wxCommandEventHandler(CListPicture::RefreshViewer));

	if (explorerTheme != nullptr)
	{
		CThemeThumbnail themeThumbnail;
		CThemeScrollBar theme;
		explorerTheme->GetThumbnailScrollTheme(theme);
		thumbscrollbar = new CScrollbarWnd(this, wxID_ANY);

		explorerTheme->GetThumbnailTheme(themeThumbnail);
		thumbnailWindow = new CThumbnailCriteria(thumbscrollbar, wxID_ANY, statusbar, themeThumbnail);
		thumbnailWindow->Init(typeAffichage);
		thumbscrollbar->SetCentralWindow(thumbnailWindow, theme);
	}

	if (explorerTheme != nullptr)
	{
		vector<int> value = { 50, 100, 150, 200, 250, 300, 350, 400, 450, 500, 550, 600, 650, 700 };
		CThemeToolbar theme;
		explorerTheme->GetThumbnailToolbarTheme(theme);
		thumbToolbar = new CThumbnailToolBar(this, wxID_ANY, theme);
		thumbToolbar->SetTabValue(value);
		thumbToolbar->SetTrackBarPosition(4);
	}

	if (explorerTheme != nullptr)
	{
		CThemeToolBarZoom theme;
		explorerTheme->GetThumbnailToolbarZoomTheme(theme);
		thumbToolbarZoom = new CThumbnailToolBarZoom(this, wxID_ANY, theme);
	}

    if(thumbscrollbar != nullptr)
        thumbscrollbar->Show(true);
    if(thumbToolbar != nullptr)
        thumbToolbar->Show(true);
    if(thumbToolbarZoom != nullptr)
        thumbToolbarZoom->Show(true);
}


CListPicture::~CListPicture()
{
	delete(thumbnailWindow);
	delete(thumbToolbar);
	delete(thumbscrollbar);
	delete(thumbToolbarZoom);
}


void CListPicture::SetTypeOrderPhoto(const int &typeOrder)
{
	typeAffichage = typeOrder;
	thumbnailWindow->Init(typeOrder);
}

void CListPicture::StopLoadingPicture()
{
    if (thumbnailWindow != nullptr)
        thumbnailWindow->StopLoadingPicture();
}

void CListPicture::SetNumElement(const int &numElement)
{
	wxString filepath = L"";
	if (thumbnailWindow != nullptr)
    {
        thumbnailWindow->StopLoadingPicture();
        thumbnailWindow->StartLoadingPicture(numElement);
		filepath = thumbnailWindow->SetActifItem(numElement);
    }
	CPreviewThumbnail * previewThumbnail = (CPreviewThumbnail *)this->FindWindowById(PREVIEWTHUMBNAILWINDOWID);
	if (previewThumbnail != nullptr)
		previewThumbnail->PhotoSelect(filepath);
}


int CListPicture::GetNbElement()
{
	if (thumbnailWindow != nullptr)
		return thumbnailWindow->GetNbElement();

	return -1;
}

void CListPicture::ThumbnailZoomOn()
{
	thumbnailWindow->ZoomOn();
}

void CListPicture::ThumbnailZoomOff()
{
	thumbnailWindow->ZoomOff();
}

void CListPicture::ThumbnailZoomPosition(const int &position)
{
	thumbnailWindow->ZoomPosition(position);
}

void CListPicture::GeolocalizeFile()
{
    vector<CThumbnailData *> listItem;
    thumbnailWindow->GetSelectItem(listItem);
    if (listItem.size() > 0)
    {
        CMapSelect mapSelect;
        wxString url = "http://www.openstreetmap.org";
        wxString infoGpsLocalisation = mapSelect.SelectNewMapLocalisation(this, url);
        if(infoGpsLocalisation != "")
        {
            wxString caption = CLibResource::LoadStringFromResource(L"LBLGeoModifCaption", 1);
            wxString text = CLibResource::LoadStringFromResource(L"LBLGeoModifText", 1);
            wxString deleteMessage = CLibResource::LoadStringFromResource(L"LBLGeoModifMessage", 1);
            wxString deleteFinalMessage = CLibResource::LoadStringFromResource(L"LBLGeoModifFinalMessage", 1);
            wxString informations = CLibResource::LoadStringFromResource(L"LBLINFORMATIONS", 1);
            CopyFileDlg copyFile(this);
            
            copyFile.SetSelectItem(&listItem);
            copyFile.SetMode(3);
            copyFile.SetNewGeoInfos(mapSelect.GetLatitudeNumber(), mapSelect.GetLongitudeNumber(), mapSelect.GetLatitude(), mapSelect.GetLongitude(), infoGpsLocalisation);
            copyFile.SetLibelle(caption, text, deleteMessage, deleteFinalMessage, informations);
            copyFile.Start();
            copyFile.ShowModal();
        }
    }
}

void CListPicture::ChangeDateFile()
{
    vector<CThumbnailData *> listItem;
    thumbnailWindow->GetSelectItem(listItem);
    if (listItem.size() > 0)
    {
        wxDateTime dt = wxDateTime::Today();
        CCalendarSelect calendarSelect;
        if(calendarSelect.SelectNewDate(this, dt))
        {
            wxString caption = CLibResource::LoadStringFromResource(L"LBLDateModifCaption", 1);
            wxString text = CLibResource::LoadStringFromResource(L"LBLDateModifText", 1);
            wxString deleteMessage = CLibResource::LoadStringFromResource(L"LBLDateModifMessage", 1);
            wxString deleteFinalMessage = CLibResource::LoadStringFromResource(L"LBLDateModifFinalMessage", 1);
            wxString informations = CLibResource::LoadStringFromResource(L"LBLINFORMATIONS", 1);
            CopyFileDlg copyFile(this);
            
            copyFile.SetSelectItem(&listItem);
            copyFile.SetMode(4);
            copyFile.SetNewDate(calendarSelect.GetSelectDate(), calendarSelect.GetSelectStringDate());
            copyFile.SetLibelle(caption, text, deleteMessage, deleteFinalMessage, informations);
            copyFile.Start();
            copyFile.ShowModal();
        }
    }
}

void CListPicture::ExportFile()
{
	vector<CThumbnailData *> listItem;
	thumbnailWindow->GetSelectItem(listItem);
	if (listItem.size() > 0)
	{
		CExportFile exportFile(this);
		exportFile.ShowModal();
		if (exportFile.GetIsOK())
		{
			wxDirDialog dlg(nullptr, "Choose destination directory", "",
				wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);

			if (dlg.ShowModal() == wxID_OK)
			{
				wxString folderPath = dlg.GetPath();
				CSqlFindCriteria sqlFindCriteria;
				InfoExportFile infoFile = exportFile.GetInfoExportFile();

				if (infoFile.outputFormat != 0)
				{
					switch (infoFile.outputFormat)
					{
					case 1: //TIFF
						infoFile.outputFormat = TIFF;
						break;
					case 2://PNG
						infoFile.outputFormat = PNG;
						break;
					case 3://GIF
						infoFile.outputFormat = GIF;
						break;
					case 4://JPEG
						infoFile.outputFormat = JPEG;
						break;
					case 5:
						//BMP
						infoFile.outputFormat = BMP;
						break;
					case 6:
						infoFile.outputFormat = TGA;
						//TGA
						break;
					case 7:
						//PCX
						infoFile.outputFormat = PCX;
						break;
					case 8:
						//MNG
						infoFile.outputFormat = MNG;
						break;
					case 9:
						//PNM
						infoFile.outputFormat = PNM;
						break;
					case 10:
						//JPC
						infoFile.outputFormat = JPC;
						break;
					case 11:
						//JPEG2000
						infoFile.outputFormat = JPEG2000;
						break;
					case 12:
						//PPM
						infoFile.outputFormat = PPM;
						break;
					}
				}

				wxString caption = CLibResource::LoadStringFromResource(L"LBLExportCaption", 1);
				wxString text = CLibResource::LoadStringFromResource(L"LBLExportText", 1);
				wxString deleteMessage = CLibResource::LoadStringFromResource(L"LBLExportMessage", 1);
				wxString deleteFinalMessage = CLibResource::LoadStringFromResource(L"LBLExportFinalMessage", 1);
				wxString informations = CLibResource::LoadStringFromResource(L"LBLINFORMATIONS", 1);
				CopyFileDlg copyFile(this);

				copyFile.SetSelectItem(&listItem);
				copyFile.SetMode(2);
				copyFile.SetDestinationFolder(folderPath);
				copyFile.SetInfosFile(infoFile);
				copyFile.SetLibelle(caption, text, deleteMessage, deleteFinalMessage, informations);
				copyFile.Start();
				copyFile.ShowModal();
			}
		}
	}
	else
	{
		wxMessageBox(wxT("No item selected"), wxT("Copy File Informations"), wxICON_INFORMATION);
	}

}

void CListPicture::DeleteFile()
{
	vector<CThumbnailData *> listItem;
	thumbnailWindow->GetSelectItem(listItem);
	if (listItem.size() > 0)
	{
		wxString caption = CLibResource::LoadStringFromResource(L"LBLDeleteCaption", 1);
		wxString text = CLibResource::LoadStringFromResource(L"LBLDeleteText", 1);
		wxString deleteMessage = CLibResource::LoadStringFromResource(L"LBLDeleteMessage", 1);
		wxString deleteFinalMessage = CLibResource::LoadStringFromResource(L"LBLDeleteFinalMessage", 1);
		wxString informations = CLibResource::LoadStringFromResource(L"LBLINFORMATIONS", 1);
		CopyFileDlg copyFile(this);

		copyFile.SetSelectItem(&listItem);
		copyFile.SetMode(0);
		copyFile.SetLibelle(caption, text, deleteMessage, deleteFinalMessage, informations);
        copyFile.Start();
		copyFile.ShowModal();

		//Mise à jour du répertoire des fichiers
		CMainWindow * mainWnd = (CMainWindow *)this->FindWindowById(MAINEXPLORERWINDOWID);
		if (mainWnd != nullptr)
			mainWnd->RefreshFolder();
	}
	else
	{
		wxMessageBox(wxT("No item selected"), wxT("Copy File Informations"), wxICON_INFORMATION);
	}
}

void CListPicture::RefreshThumbnail()
{
    thumbnailWindow->EraseThumbnail();
}

void CListPicture::UpdateScreenRatio()
{
    thumbscrollbar->UpdateScreenRatio();
    thumbToolbar->UpdateScreenRatio();
    thumbToolbarZoom->UpdateScreenRatio();
    thumbnailWindow->UpdateScreenRatio();
    this->Resize();
}

void CListPicture::CopyFile()
{
	vector<CThumbnailData *> listItem;
	thumbnailWindow->GetSelectItem(listItem);
	if (listItem.size() > 0)
	{
		wxString caption = CLibResource::LoadStringFromResource(L"LBLDeleteCaption", 1);
		wxString text = CLibResource::LoadStringFromResource(L"LBLCopyText", 1);
		wxString copyMessage = CLibResource::LoadStringFromResource(L"LBLCopyMessage", 1);
		wxString copyFinalMessage = CLibResource::LoadStringFromResource(L"LBLCopyFinalMessage", 1);
		wxString informations = CLibResource::LoadStringFromResource(L"LBLINFORMATIONS", 1);

		wxDirDialog dlg(nullptr, "Choose destination directory", "",
			wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);

		if (dlg.ShowModal() == wxID_OK)
		{
			wxString folderPath = dlg.GetPath();
			CopyFileDlg copyFile(this);
			copyFile.SetSelectItem(&listItem);
			copyFile.SetMode(1);
			copyFile.SetLibelle(caption, text, copyMessage, copyFinalMessage, informations);
			copyFile.SetDestinationFolder(folderPath);
			copyFile.Start();
			copyFile.ShowModal();
		}
	}
	else
	{
		wxMessageBox(wxT("No item selected"), wxT("Copy File Informations"), wxICON_INFORMATION);
	}
}

void CListPicture::Resize()
{
	int pictureWidth = width;
	int pictureHeight = height - (thumbToolbar->GetHeight() + thumbToolbarZoom->GetHeight());

	thumbToolbarZoom->SetSize(0, 0, width, thumbToolbarZoom->GetHeight());
	thumbscrollbar->SetSize(0, thumbToolbarZoom->GetHeight(), pictureWidth, pictureHeight);
	thumbToolbar->SetSize(0, pictureHeight + thumbToolbarZoom->GetHeight(), width, thumbToolbar->GetHeight());

}

void CListPicture::PictureClick(const wxString &filepath, const int &numElement)
{
	if (thumbnailWindow != nullptr)
	{
		thumbnailWindow->StopLoadingPicture();
		thumbnailWindow->StartLoadingPicture(numElement);
	}

	CPreviewThumbnail * previewThumbnail = (CPreviewThumbnail *)this->FindWindowById(PREVIEWTHUMBNAILWINDOWID);
	if (previewThumbnail != nullptr)
	{
		previewThumbnail->PhotoSelect(filepath);
	}
}

void CListPicture::RefreshViewer(wxCommandEvent& event)
{
    CMainWindow * mainWindow = (CMainWindow *)this->FindWindowById(MAINEXPLORERWINDOWID);
    if (mainWindow != nullptr)
        mainWindow->RefreshViewer();

	if (thumbnailWindow != nullptr)
		thumbnailWindow->Init(typeAffichage);
}

void CListPicture::RefreshList()
{
	wxCommandEvent * event = new wxCommandEvent(EVENT_REFRESHVIEWER);
	wxQueueEvent(this, event);
}

void CListPicture::UpdateList()
{
	wxCommandEvent * event = new wxCommandEvent(EVENT_REFRESHVIEWER);
	wxQueueEvent(this, event);

}

