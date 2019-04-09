#include <header.h>
#include "ListPicture.h"
#include <FileUtility.h>
#include <SQLRemoveData.h>
#include <SqlFindCriteria.h>
#include <ThumbnailData.h>
#include "ViewerTheme.h"
#include "ViewerThemeInit.h"
#include <LibResource.h>
#include "MainWindow.h"
#include "ImageList.h"
#include <window_id.h>
#include <picture_id.h>
#include <CopyFileDlg.h>
#include <ExportFile.h>
#include <ConvertUtility.h>
#include <wx/dir.h>
#include <libPicture.h>
#include <MapSelect.h>
#include <CalendarSelect.h>
#include "SqlFindPhotos.h"
#include "ViewerParam.h"
#include "ViewerParamInit.h"
#include <IndexGenerator.h>
#include <ThumbnailDataSQL.h>
#include <SavePicture.h>
wxDEFINE_EVENT(EVENT_REFRESHVIEWER, wxCommandEvent);

using namespace Regards::Sqlite;
using namespace Regards::Window;
using namespace Regards::Viewer;

CListPicture::CListPicture(wxWindow* parent, wxWindowID id, IStatusBarInterface * statusbar, CImageList * imageList)
	: CWindowMain("CListPicture",parent, id)
{
	thumbscrollbar = nullptr;
	thumbToolbar = nullptr;
	thumbToolbarZoom = nullptr;
	thumbnailFolder = nullptr;
	typeAffichage = SHOW_ALL;
	update = true;
	showToolbar = true;
	isBottom = false;
	bool checkValidity = false;
	CViewerParam * config = CViewerParamInit::getInstance();
	if (config != nullptr)
		checkValidity = config->GetCheckThumbnailValidity();

	if (imageList != nullptr)
	{
		photoVector = imageList->GetCopy();
	}
	
	CViewerTheme * viewerTheme = CViewerThemeInit::getInstance();

	if (viewerTheme != nullptr)
	{
		CThemeThumbnail themeThumbnail;
		CThemeScrollBar theme;
		viewerTheme->GetThumbnailScrollTheme(theme);
		thumbscrollbar = new CScrollbarWnd(this, wxID_ANY);

		viewerTheme->GetThumbnailTheme(themeThumbnail);
		thumbnailFolder = new CThumbnailFolder(thumbscrollbar, THUMBNAILFOLDER, statusbar, themeThumbnail, checkValidity);
		//thumbnailWindow->Init(typeAffichage);
		thumbscrollbar->SetCentralWindow(thumbnailFolder, theme);
	}

	if (viewerTheme != nullptr)
	{
		std::vector<int> value = { 50, 100, 150, 200, 250, 300, 350, 400, 450, 500, 550, 600, 650, 700 };
		CThemeToolbar theme;
		viewerTheme->GetThumbnailToolbarTheme(theme);
		thumbToolbar = new CThumbnailToolBar(this, wxID_ANY, theme);
		thumbToolbar->SetTabValue(value);
		thumbToolbar->SetTrackBarPosition(4);
	}

	if (viewerTheme != nullptr)
	{
		CThemeToolBarZoom theme;
		viewerTheme->GetThumbnailToolbarZoomTheme(theme);
		thumbToolbarZoom = new CThumbnailToolBarZoom(this, wxID_ANY, theme);
	}

    if(thumbscrollbar != nullptr)
        thumbscrollbar->Show(true);
    if(thumbToolbar != nullptr)
        thumbToolbar->Show(true);
    if(thumbToolbarZoom != nullptr)
        thumbToolbarZoom->Show(true);

	Connect(wxEVENT_THUMBNAILZOOMON, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CListPicture::ThumbnailZoomOn));
	Connect(wxEVENT_THUMBNAILZOOMOFF, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CListPicture::ThumbnailZoomOff));
	Connect(wxEVENT_THUMBNAILZOOMPOSITION, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CListPicture::ThumbnailZoomPosition));
	Connect(wxEVENT_EXPORTFILE, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CListPicture::ExportFile));
	Connect(wxEVENT_DELETEFILE, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CListPicture::DeleteFile));
	Connect(wxEVENT_COPYFILE, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CListPicture::CopyFile));
	Connect(wxEVENT_GEOLOCALIZEFILE, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CListPicture::GeolocalizeFile));
	Connect(wxEVENT_CHANGEDATEFILE, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CListPicture::ChangeDateFile));
	Connect(wxEVENT_SETTHUMBNAILBOTTOM, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CListPicture::SetBottom));
	Connect(wxEVENT_SETTHUMBNAILRIGHT, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CListPicture::SetRight));
	Connect(wxEVENT_CHANGETYPEAFFICHAGE, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CListPicture::ChangeTypeAffichage));
	Connect(wxEVENT_SETLISTPICTURE, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CListPicture::SetListeFile));
	Connect(wxEVENT_GENERATEINDEXFILE, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CListPicture::GenerateIndexFile));
	Connect(wxEVT_IDLE, wxIdleEventHandler(CListPicture::OnIdle));

	update = false;
	processIdle = true;
}

CListPicture::~CListPicture()
{
	delete(thumbnailFolder);
	delete(thumbToolbar);
	delete(thumbscrollbar);
	delete(thumbToolbarZoom);
}

int CListPicture::GetThumbnailHeight()
{
	return thumbnailFolder->GetIconeHeight() + thumbscrollbar->GetBarHeight();
}

void CListPicture::OnIdle(wxIdleEvent& evt)
{
    //TRACE();
    //if (processIdle && !endProgram)
    //    ProcessIdle();
	//StartThread();

	if (processIdle && !endProgram)
	{
		printf("CListPicture::ProcessIdle() \n");
		if (update)
		{
			thumbnailFolder->Init(&photoVector, typeAffichage);
			update = false;
		}
		processIdle = false;
	}
}

void CListPicture::SetBottom(wxCommandEvent& event)
{
	
	isBottom = true;
	showToolbar = false;
	thumbToolbar->Show(false);
	thumbToolbarZoom->Show(false);
	thumbscrollbar->HideVerticalScroll();
	thumbnailFolder->SetNoVScroll(true);
	thumbnailFolder->SetCheck(false);
	thumbnailFolder->Init(&photoVector, typeAffichage);
	this->Resize();
}

void CListPicture::ChangeTypeAffichage(wxCommandEvent& event)
{
	typeAffichage = event.GetExtraLong();
	thumbnailFolder->Init(&photoVector, typeAffichage);
}

void CListPicture::SetRight(wxCommandEvent& event)
{
	isBottom = false;
	showToolbar = true;
	thumbToolbar->Show(true);
	thumbToolbarZoom->Show(true);
	thumbscrollbar->ShowVerticalScroll();
	thumbnailFolder->SetNoVScroll(false);
	thumbnailFolder->SetCheck(true);
	thumbnailFolder->Init(&photoVector, typeAffichage);
	this->Resize();
}

void CListPicture::SetActifItem(const int &numItem, const bool &move)
{
	if (thumbnailFolder != nullptr)
		thumbnailFolder->SetActifItem(numItem, move);
}

void CListPicture::SetListeFile(wxCommandEvent& event)
{
	CImageList * listPicture = (CImageList *)event.GetClientData();
	if (listPicture != nullptr)
	{
		this->photoVector.clear();
		photoVector = listPicture->GetCopy();
	}

	if (thumbnailFolder != nullptr)
		thumbnailFolder->Init(&this->photoVector, typeAffichage);
	if (!isBottom)
		thumbnailFolder->SetCheck(true);
}




void CListPicture::ThumbnailZoomOn(wxCommandEvent& event)
{
	thumbnailFolder->ZoomOn();
}

void CListPicture::ThumbnailZoomOff(wxCommandEvent& event)
{
	thumbnailFolder->ZoomOff();
}

void CListPicture::ThumbnailZoomPosition(wxCommandEvent& event)
{
	thumbnailFolder->ZoomPosition(event.GetExtraLong());
}

void CListPicture::GenerateIndexFile(wxCommandEvent& event)
{
	vector<CThumbnailData *> listItem;
	thumbnailFolder->GetSelectItem(listItem);
	if (listItem.size() > 0)
	{
		IndexGenerator indexGenerator(this);
		indexGenerator.SetThumbnailSize(thumbnailFolder->GetIconeWidth(), thumbnailFolder->GetIconeHeight());
		indexGenerator.ShowModal();
		if (indexGenerator.IsOk())
		{
			int width, height, nbPictureLine;
			wxColour color = indexGenerator.GetColor();
			wxColour fontColor = wxColour(255 - color.Red(), 255 - color.Green(), 255 - color.Blue());
			indexGenerator.GetThumbnailSize(width, height);
			nbPictureLine = indexGenerator.GetNbPictureByLine();

			int getNbLine = listItem.size() / nbPictureLine;
			if (getNbLine * nbPictureLine < listItem.size())
				getNbLine++;

			int heightLibelle = 50;
			wxString libelle = indexGenerator.GetIndexTitle();
			int bitmapWidth = nbPictureLine * width;
			int bitmapHeight = getNbLine * height;
			wxBitmap bitmap = wxBitmap(bitmapWidth, bitmapHeight+ heightLibelle);
			wxMemoryDC memdc;
			memdc.SelectObject(bitmap);
			memdc.SetBackground(color);
			memdc.Clear();

			CViewerTheme * viewerTheme = CViewerThemeInit::getInstance();
			CThemeThumbnail themeThumbnail;
			viewerTheme->GetThumbnailTheme(themeThumbnail);

			CThemeIcone themeIcone = themeThumbnail.themeIcone;
			themeIcone.font.SetColorFont(fontColor);
			wxSize size = CWindowMain::GetSizeTexte(&memdc, libelle, themeIcone.font);
			CWindowMain::DrawTexte(&memdc, libelle, (bitmapWidth - size.x) / 2, (heightLibelle - size.y) / 2, themeIcone.font);
			

			for (int i = 0;i < listItem.size(); i++)
			{
				CThumbnailData * data = listItem[i];
				int x = (i % nbPictureLine) * width;
				int y = (i / nbPictureLine) * height + heightLibelle;
				CIcone * pBitmapIcone = new CIcone(nullptr);
				pBitmapIcone->SetNumElement(data->GetNumElement());
				pBitmapIcone->SetData(data);
				pBitmapIcone->SetBackgroundColor(color);
				pBitmapIcone->SetTheme(themeIcone);
				pBitmapIcone->SetSizeIcone(width, height);
				pBitmapIcone->SetWindowPos(x, y);
				pBitmapIcone->RenderIcone(&memdc, 0, 0);
				delete pBitmapIcone;
			}
			
			memdc.SelectObject(wxNullBitmap);
			

			wxImage picture = bitmap.ConvertToImage();

			CImageLoadingFormat * imageLoad = new CImageLoadingFormat();
			imageLoad->SetPicture(&picture);
			CRegardsBitmap * regardsBmp = imageLoad->GetRegardsBitmap();
			CSavePicture::SavePicture(this, regardsBmp, "photoindex.png");
			delete regardsBmp;
		}
	}
	else
	{
        wxString noItemSelected = CLibResource::LoadStringFromResource(L"NoItemSelected",1);
        wxString generateIndexInformations = CLibResource::LoadStringFromResource(L"GenerateIndexInformations",1);
		wxMessageBox(noItemSelected, generateIndexInformations, wxICON_INFORMATION);
	}
}

void CListPicture::GeolocalizeFile(wxCommandEvent& event)
{
    vector<CThumbnailData *> listItem;
	thumbnailFolder->GetSelectItem(listItem);
    if (listItem.size() > 0)
    {
        CMapSelect mapSelect;
        wxString url = CLibResource::LoadStringFromResource(L"openstreetmapurl", 1);//"http://www.openstreetmap.org";
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
	else
	{
        wxString noItemSelected = CLibResource::LoadStringFromResource(L"NoItemSelected",1);
        wxString geolocalizeFile = CLibResource::LoadStringFromResource(L"GeolocalizeFile",1);
		wxMessageBox(noItemSelected, geolocalizeFile, wxICON_INFORMATION);
	}
}

void CListPicture::ChangeDateFile(wxCommandEvent& event)
{
    vector<CThumbnailData *> listItem;
	thumbnailFolder->GetSelectItem(listItem);
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
	else
	{
        wxString noItemSelected = CLibResource::LoadStringFromResource(L"NoItemSelected",1);
        wxString changeDateInfos = CLibResource::LoadStringFromResource(L"ChangeDateInfos",1);
		wxMessageBox(noItemSelected, changeDateInfos, wxICON_INFORMATION);
	}
}

void CListPicture::ExportFile(wxCommandEvent& event)
{
	vector<CThumbnailData *> listItem;
	thumbnailFolder->GetSelectItem(listItem);
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
        wxString noItemSelected = CLibResource::LoadStringFromResource(L"NoItemSelected",1);
        wxString exportFileInfos = CLibResource::LoadStringFromResource(L"ExportFileInfos",1);
		wxMessageBox(noItemSelected, exportFileInfos, wxICON_INFORMATION);
	}

}

void CListPicture::DeleteFile(wxCommandEvent& event)
{
	vector<CThumbnailData *> listItem;
	thumbnailFolder->GetSelectItem(listItem);
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
		CMainWindow * mainWindow = (CMainWindow *)this->FindWindowById(MAINVIEWERWINDOWID);
		if (mainWindow != nullptr)
		{
			wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_REFRESHFOLDER);
			mainWindow->GetEventHandler()->AddPendingEvent(evt);
		}
	}
	else
	{
        wxString noItemSelected = CLibResource::LoadStringFromResource(L"NoItemSelected",1);
        wxString deleteFileInfos = CLibResource::LoadStringFromResource(L"DeleteFileInfos",1);
		wxMessageBox(noItemSelected, deleteFileInfos, wxICON_INFORMATION);
	}
}

void CListPicture::UpdateScreenRatio()
{
    thumbscrollbar->UpdateScreenRatio();
    thumbToolbar->UpdateScreenRatio();
    thumbToolbarZoom->UpdateScreenRatio();
	thumbnailFolder->UpdateScreenRatio();
    this->Resize();
}

void CListPicture::CopyFile(wxCommandEvent& event)
{
	vector<CThumbnailData *> listItem;
	thumbnailFolder->GetSelectItem(listItem);
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
        wxString noItemSelected = CLibResource::LoadStringFromResource(L"NoItemSelected",1);
        wxString copyFileInfo = CLibResource::LoadStringFromResource(L"CopyFileInfo",1);
		wxMessageBox(noItemSelected, copyFileInfo, wxICON_INFORMATION);
	}
}

void CListPicture::Resize()
{
	if (showToolbar)
	{
		int pictureWidth = GetWindowWidth();
		int pictureHeight = GetWindowHeight() - (thumbToolbar->GetHeight() + thumbToolbarZoom->GetHeight());

		thumbToolbarZoom->SetSize(0, 0, GetWindowWidth(), thumbToolbarZoom->GetHeight());
		thumbscrollbar->SetSize(0, thumbToolbarZoom->GetHeight(), pictureWidth, pictureHeight);
		thumbToolbar->SetSize(0, pictureHeight + thumbToolbarZoom->GetHeight(), GetWindowWidth(), thumbToolbar->GetHeight());
	}
	else
	{
		
		thumbscrollbar->SetSize(0, 0, GetWindowWidth(), GetWindowHeight());
	}

}

void CListPicture::PictureClick(const wxString &filepath, const int &numElement)
{
	if (thumbnailFolder != nullptr)
	{
		//thumbnailFolder->StopLoadingPicture();
		//thumbnailFolder->StartLoadingPicture(numElement);
		wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_ONSTOPLOADINGPICTURE);
		thumbnailFolder->GetEventHandler()->AddPendingEvent(evt);

		wxCommandEvent evt2(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_ONSTARTLOADINGPICTURE);
		evt2.SetExtraLong(numElement);
		thumbnailFolder->GetEventHandler()->AddPendingEvent(evt2);
	}
}
