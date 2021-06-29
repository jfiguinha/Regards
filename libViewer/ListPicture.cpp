#include <header.h>
#include "ListPicture.h"
#include <FileUtility.h>
#include <SqlFindCriteria.h>
#include <ThumbnailData.h>
#include "MainTheme.h"
#include "MainThemeInit.h"
#include <LibResource.h>
#include "MainWindow.h"
#include <picture_id.h>
#include <ExportFile.h>
#include <ConvertUtility.h>
#include <wx/dir.h>
#include <libPicture.h>
#include <MapSelect.h>
#include <CalendarSelect.h>
#include "ViewerParam.h"
#include "ViewerParamInit.h"
#include <IndexGenerator.h>
#include <SavePicture.h>
#include <wx/progdlg.h>
#include <SqlCriteria.h>
#include <SqlGps.h>
#include <SqlPhotoCriteria.h>
#include <SqlPhotos.h>
#include <ScrollbarWnd.h>
#include "ThumbnailFolder.h"
#include "ThumbnailToolBar.h"
#include "ThumbnailToolBarZoom.h"
#include <GpsEngine.h>
#include <RegardsConfigParam.h>
#include <ImageLoadingFormat.h>
#include <WindowMain.h>
#ifdef EXIV2
#include <MetadataExiv2.h>
#elif defined(WIN32)
#include <SetMetadataGps.h>
#include <SetMetadataDate.h>
#endif
using namespace Regards::exiv2;
using namespace Regards::Picture;
using namespace Regards::Sqlite;
using namespace Regards::Window;
using namespace Regards::Viewer;

CListPicture::CListPicture(wxWindow* parent, wxWindowID id)
	: CWindowMain("CListPicture", parent, id)
{
	thumbscrollbar = nullptr;
	thumbToolbar = nullptr;
	thumbToolbarZoom = nullptr;
	thumbnailFolder = nullptr;
	typeAffichage = SHOW_ALL;
	std::vector<int> value = {100, 150, 200, 250, 300, 350, 400, 450, 500, 550, 600, 650, 700};
	wxRect rect;
	int positionTab = 3;
	bool checkValidity = false;
	CMainParam* config = CMainParamInit::getInstance();
	if (config != nullptr)
		checkValidity = config->GetCheckThumbnailValidity();

	if (config != nullptr)
		config->GetSlideFolderPos(positionTab);
	CMainTheme* viewerTheme = CMainThemeInit::getInstance();

	if (viewerTheme != nullptr)
	{
		CThemeSplitter theme;
		viewerTheme->GetSplitterTheme(&theme);
		windowManager = new CWindowManager(this, wxID_ANY, theme);
	}

	if (viewerTheme != nullptr)
	{
		CThemeThumbnail themeThumbnail;
		CThemeScrollBar theme;
		viewerTheme->GetScrollTheme(&theme);

		viewerTheme->GetThumbnailTheme(&themeThumbnail);
		thumbnailFolder = new CThumbnailFolder(windowManager, THUMBNAILFOLDER, themeThumbnail, checkValidity);
		thumbscrollbar = new CScrollbarWnd(windowManager, thumbnailFolder, wxID_ANY);
		thumbscrollbar->ShowVerticalScroll();
		thumbnailFolder->SetNoVScroll(false);
		thumbnailFolder->SetCheck(true);
		thumbnailFolder->ChangeTabValue(value, positionTab);
		windowManager->AddWindow(thumbscrollbar, Pos::wxCENTRAL, false, 0, rect, wxID_ANY, false);
	}

	if (viewerTheme != nullptr)
	{
		CThemeToolbar theme;
		//viewerTheme->GetThumbnailToolbarTheme(theme);
		viewerTheme->GetBitmapToolbarTheme(&theme);
		thumbToolbar = new CThumbnailToolBar(windowManager, wxID_ANY, theme, false);
		thumbToolbar->SetTabValue(value);
		thumbToolbar->SetTrackBarPosition(positionTab - 1);

		windowManager->AddWindow(thumbToolbar, Pos::wxBOTTOM, true, thumbToolbar->GetHeight(), rect, wxID_ANY, false);
	}

	if (viewerTheme != nullptr)
	{
		CThemeToolBarZoom theme;
		viewerTheme->GetThumbnailToolbarZoomTheme(theme);
		//viewerTheme->GetBitmapToolbarTheme(&theme);
		thumbToolbarZoom = new CThumbnailToolBarZoom(windowManager, wxID_ANY, theme);
		windowManager->AddWindow(thumbToolbarZoom, Pos::wxTOP, true, thumbToolbarZoom->GetHeight(), rect, wxID_ANY,
		                         false);
	}

	Connect(wxEVENT_THUMBNAILZOOMON, wxCommandEventHandler(CListPicture::ThumbnailZoomOn));
	Connect(wxEVENT_THUMBNAILZOOMOFF, wxCommandEventHandler(CListPicture::ThumbnailZoomOff));
	Connect(wxEVENT_THUMBNAILZOOMPOSITION, wxCommandEventHandler(CListPicture::ThumbnailZoomPosition));
	Connect(wxEVENT_EXPORTFILE, wxCommandEventHandler(CListPicture::ExportFileCmd));
	Connect(wxEVENT_DELETEFILE, wxCommandEventHandler(CListPicture::DeleteFile));
	Connect(wxEVENT_COPYFILE, wxCommandEventHandler(CListPicture::CopyFile));
	Connect(wxEVENT_GEOLOCALIZEFILE, wxCommandEventHandler(CListPicture::GeolocalizeFileCmd));
	Connect(wxEVENT_CHANGEDATEFILE, wxCommandEventHandler(CListPicture::ChangeDateFileCmd));
	Connect(wxEVENT_GENERATEINDEXFILE, wxCommandEventHandler(CListPicture::GenerateIndexFile));
}

void CListPicture::ForceRefresh()
{
	thumbToolbar->ForceRefresh();
	thumbToolbarZoom->ForceRefresh();
	thumbnailFolder->ForceRefresh();
}

CListPicture::~CListPicture()
{
	int positionTab = thumbnailFolder->GetTabValue();
	CMainParam* config = CMainParamInit::getInstance();
	if (config != nullptr)
		config->SetSlideFolderPos(positionTab);

	if (windowManager != nullptr)
		delete(windowManager);
}

int CListPicture::GetThumbnailHeight()
{
	return thumbnailFolder->GetIconeHeight() + thumbscrollbar->GetBarHeight();
}

void CListPicture::ChangeTypeAffichage(const long& typeAffichage)
{
	this->typeAffichage = typeAffichage;
	thumbnailFolder->Init(typeAffichage);
}

void CListPicture::SetActifItem(const int& numItem, const bool& move)
{
	if (thumbnailFolder != nullptr)
		thumbnailFolder->SetActifItem(numItem, move);
}

int CListPicture::GetNumItem()
{
	if (thumbnailFolder != nullptr)
		return thumbnailFolder->GetNumItem();

	return 0;
}

void CListPicture::SetListeFile()
{
	if (thumbnailFolder != nullptr)
	{
		thumbnailFolder->Init(typeAffichage);
		thumbnailFolder->SetCheck(true);
	}
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
	vector<CThumbnailData*> listItem;
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
			auto fontColor = wxColour(255 - color.Red(), 255 - color.Green(), 255 - color.Blue());
			indexGenerator.GetThumbnailSize(width, height);
			nbPictureLine = indexGenerator.GetNbPictureByLine();

			int getNbLine = listItem.size() / nbPictureLine;
			if (getNbLine * nbPictureLine < listItem.size())
				getNbLine++;

			int heightLibelle = 50;
			wxString libelle = indexGenerator.GetIndexTitle();
			int bitmapWidth = nbPictureLine * width;
			int bitmapHeight = getNbLine * height;
			auto bitmap = wxBitmap(bitmapWidth, bitmapHeight + heightLibelle);
			wxMemoryDC memdc;
			memdc.SelectObject(bitmap);
			memdc.SetBackground(color);
			memdc.Clear();

			CMainTheme* viewerTheme = CMainThemeInit::getInstance();
			CThemeThumbnail themeThumbnail;
			viewerTheme->GetThumbnailTheme(&themeThumbnail);

			CThemeIcone themeIcone = themeThumbnail.themeIcone;
			themeIcone.font.SetColorFont(fontColor);
			wxSize size = GetSizeTexte(&memdc, libelle, themeIcone.font);
			DrawTexte(&memdc, libelle, (bitmapWidth - size.x) / 2, (heightLibelle - size.y) / 2, themeIcone.font);


			for (int i = 0; i < listItem.size(); i++)
			{
				CThumbnailData* data = listItem[i];
				int x = (i % nbPictureLine) * width;
				int y = (i / nbPictureLine) * height + heightLibelle;
				auto pBitmapIcone = new CIcone();
				pBitmapIcone->SetNumElement(data->GetNumElement());
				pBitmapIcone->SetData(data);
				pBitmapIcone->SetBackgroundColor(color);
				pBitmapIcone->SetTheme(themeIcone);
				pBitmapIcone->SetSizeIcone(width, height);
				pBitmapIcone->SetWindowPos(x, y);
				pBitmapIcone->RenderIcone(&memdc, 0, 0, false, false);
				delete pBitmapIcone;
			}

			memdc.SelectObject(wxNullBitmap);


			wxImage picture = bitmap.ConvertToImage();

			auto imageLoad = new CImageLoadingFormat();
			imageLoad->SetPicture(&picture);
			CSavePicture::SavePicture(nullptr, imageLoad, "photoindex.png");
			delete imageLoad;
		}
	}
	else
	{
		wxString noItemSelected = CLibResource::LoadStringFromResource(L"NoItemSelected", 1);
		wxString generateIndexInformations = CLibResource::LoadStringFromResource(L"GenerateIndexInformations", 1);
		wxMessageBox(noItemSelected, generateIndexInformations, wxICON_INFORMATION);
	}
}


void CListPicture::GeolocalizeFile(const wxString& filename, const float& latitude, const float& longitude,
                                   const wxString& lat, const wxString& lng, const wxString& geoInfos)
{
	CLibPicture libPicture;
	if (libPicture.TestIsExifCompatible(filename))
	{
#if defined(EXIV2)
		wxString wlatitudeRef = "";
		wxString wlongitudeRef = "";
		wxString wlongitude = to_string(abs(longitude));
		wxString wlatitude = to_string(abs(latitude));

		if (latitude < 0)
			wlatitudeRef = "S";
		else
			wlatitudeRef = "N";

		if (longitude < 0)
			wlongitudeRef = "W";
		else
			wlongitudeRef = "E";

		CMetadataExiv2 metadataExiv2(filename);
		metadataExiv2.SetGpsInfos(wlatitudeRef, wlongitudeRef, wlatitude, wlongitude);

#elif defined(__APPLE__)
		CAppleReadExif appleReadExif;
		appleReadExif.WriteGps(filename, latitude, longitude);
#elif defined(WIN32)

		wxString wlatitudeRef = "";
		wxString wlongitudeRef = "";
		wxString wlongitude = to_string(abs(longitude));
		wxString wlatitude = to_string(abs(latitude));

		if (latitude < 0)
			wlatitudeRef = "S";
		else
			wlatitudeRef = "N";

		if (longitude < 0)
			wlongitudeRef = "W";
		else
			wlongitudeRef = "E";

		CSetMetadataGps metadata(wlatitudeRef, wlongitudeRef, wlatitude, wlongitude);
		metadata.SetMetadata(filename);

#endif
	}

	CSqlPhotos sqlPhotos;
	int numPhotoId = sqlPhotos.GetPhotoId(filename);

	bool isNew = false;
	CSqlCriteria sqlCriteria;
	CSqlGps sqlGps;
	CSqlPhotoCriteria sqlPhotoCriteria;

	int oldCriteriaId = sqlCriteria.GetCriteriaIdByCategorie(numPhotoId, 1);

	int numCriteriaId = sqlCriteria.GetOrInsertCriteriaId(1, 1, geoInfos, isNew);
	sqlPhotoCriteria.InsertPhotoCriteria(numPhotoId, numCriteriaId);
	if (oldCriteriaId != -1 && numCriteriaId != oldCriteriaId)
		sqlPhotoCriteria.DeletePhotoCriteria(numPhotoId, oldCriteriaId);

	sqlGps.DeleteGps(filename);
	sqlGps.InsertGps(filename, lat, lng);

	CListOfWindow* fileGeolocalisation = CGpsEngine::getInstance();
	fileGeolocalisation->SendMessageToWindow(filename, 1);
}


void CListPicture::GeolocalizeFileCmd(wxCommandEvent& event)
{
	vector<CThumbnailData*> listItem;
	thumbnailFolder->GetSelectItem(listItem);
	if (listItem.size() > 0)
	{
		CMapSelect mapSelect;
		wxString url = CLibResource::LoadStringFromResource(L"openstreetmapurl", 1); //"http://www.openstreetmap.org";
		wxString infoGpsLocalisation = mapSelect.SelectNewMapLocalisation(this, url);
		if (infoGpsLocalisation != "")
		{
			wxString caption = CLibResource::LoadStringFromResource(L"LBLGeoModifCaption", 1);
			wxString text = CLibResource::LoadStringFromResource(L"LBLGeoModifText", 1);
			wxString deleteMessage = CLibResource::LoadStringFromResource(L"LBLGeoModifMessage", 1);
			wxString deleteFinalMessage = CLibResource::LoadStringFromResource(L"LBLGeoModifFinalMessage", 1);
			wxString informations = CLibResource::LoadStringFromResource(L"LBLINFORMATIONS", 1);

			wxProgressDialog dialog(caption, text, listItem.size(), this,
			                        wxPD_APP_MODAL | wxPD_CAN_ABORT | wxPD_AUTO_HIDE);
			for (int i = 0; i < listItem.size(); i++)
			{
				int j = i + 1;
				CThumbnailData* data = listItem.at(i);
				wxString filename = data->GetFilename();
				wxString message = text + to_string(j) + "/" + to_string(listItem.size());
				GeolocalizeFile(filename, mapSelect.GetLatitudeNumber(), mapSelect.GetLongitudeNumber(),
				                mapSelect.GetLatitude(), mapSelect.GetLongitude(), infoGpsLocalisation);
				if (false == dialog.Update(i, message))
					break;
			}

			/*
            CopyFileDlg copyFile(this);
			copyFile.SetSelectItem(&listItem);
            copyFile.SetMode(3);
            copyFile.SetNewGeoInfos(mapSelect.GetLatitudeNumber(), mapSelect.GetLongitudeNumber(), mapSelect.GetLatitude(), mapSelect.GetLongitude(), infoGpsLocalisation);
            copyFile.SetLibelle(caption, text, deleteMessage, deleteFinalMessage, informations);
            copyFile.Start();
            copyFile.ShowModal();
			*/
		}
	}
	else
	{
		wxString noItemSelected = CLibResource::LoadStringFromResource(L"NoItemSelected", 1);
		wxString geolocalizeFile = CLibResource::LoadStringFromResource(L"GeolocalizeFile", 1);
		wxMessageBox(noItemSelected, geolocalizeFile, wxICON_INFORMATION);
	}
}


void CListPicture::ChangeDateFile(const wxString& filename, const wxDateTime& newDate, const wxString& selectDate)
{
	CLibPicture libPicture;
	if (libPicture.TestIsExifCompatible(filename))
	{
#if defined(EXIV2)
		CMetadataExiv2 metadataExiv2(filename);
		metadataExiv2.SetDateTime(selectDate);
#elif defined(__APPLE__)
		appleReadExif.WriteDateTime(filename, newDate);
#elif defined(WIN32)
		CSetMetadataDate metadata(newDate.FormatDate());
		metadata.SetMetadata(filename);
#endif
	}
	bool isNew = false;
	CSqlCriteria sqlCriteria;
	CSqlPhotoCriteria sqlPhotoCriteria;
	CSqlPhotos sqlPhotos;

	int numPhotoId = sqlPhotos.GetPhotoId(filename);
	int oldCriteriaId = sqlCriteria.GetCriteriaIdByCategorie(numPhotoId, 3);

	int numCriteriaId = sqlCriteria.GetOrInsertCriteriaId(1, 3, selectDate, isNew);
	sqlPhotoCriteria.InsertPhotoCriteria(numPhotoId, numCriteriaId);
	if (oldCriteriaId != -1 && numCriteriaId != oldCriteriaId)
		sqlPhotoCriteria.DeletePhotoCriteria(numPhotoId, oldCriteriaId);
}


void CListPicture::ChangeDateFileCmd(wxCommandEvent& event)
{
	vector<CThumbnailData*> listItem;
	thumbnailFolder->GetSelectItem(listItem);
	if (listItem.size() > 0)
	{
		wxDateTime dt = wxDateTime::Today();
		CCalendarSelect calendarSelect;
		if (calendarSelect.SelectNewDate(this, dt))
		{
			wxString caption = CLibResource::LoadStringFromResource(L"LBLDateModifCaption", 1);
			wxString text = CLibResource::LoadStringFromResource(L"LBLDateModifText", 1);
			wxString deleteMessage = CLibResource::LoadStringFromResource(L"LBLDateModifMessage", 1);
			wxString deleteFinalMessage = CLibResource::LoadStringFromResource(L"LBLDateModifFinalMessage", 1);
			wxString informations = CLibResource::LoadStringFromResource(L"LBLINFORMATIONS", 1);

			wxProgressDialog dialog(caption, text, listItem.size(), this,
			                        wxPD_APP_MODAL | wxPD_CAN_ABORT | wxPD_AUTO_HIDE);
			for (int i = 0; i < listItem.size(); i++)
			{
				int j = i + 1;
				CThumbnailData* data = listItem.at(i);
				wxString filename = data->GetFilename();
				wxString message = text + to_string(j) + "/" + to_string(listItem.size());
				ChangeDateFile(filename, calendarSelect.GetSelectDate(), calendarSelect.GetSelectStringDate());
				if (false == dialog.Update(i, message))
					break;
			}

			/*
            CopyFileDlg copyFile(this);
            copyFile.SetSelectItem(&listItem);
            copyFile.SetMode(4);
            copyFile.SetNewDate(calendarSelect.GetSelectDate(), calendarSelect.GetSelectStringDate());
            copyFile.SetLibelle(caption, text, deleteMessage, deleteFinalMessage, informations);
            copyFile.Start();
            copyFile.ShowModal();
			*/
		}
	}
	else
	{
		wxString noItemSelected = CLibResource::LoadStringFromResource(L"NoItemSelected", 1);
		wxString changeDateInfos = CLibResource::LoadStringFromResource(L"ChangeDateInfos", 1);
		wxMessageBox(noItemSelected, changeDateInfos, wxICON_INFORMATION);
	}
}


wxString CListPicture::GenerateFileName(const InfoExportFile& infoFile, const wxString& dateFile,
                                        const wxString& gpsFile)
{
	wxString libelle = CLibResource::LoadStringFromResource("LBLNOTGEO", 1);
	wxString filename;
	if (infoFile.priority == 0)
	{
		if (infoFile.dateInfoSelection != 0)
		{
			vector<wxString> intValue = CConvertUtility::split(dateFile, '.');
			if (intValue.size() == 3)
			{
				if (infoFile.dateInfoSelection == 1 || infoFile.dateInfoSelection == 2 || infoFile.dateInfoSelection ==
					3)
				{
					filename.append(intValue[0]);
				}
				if (infoFile.dateInfoSelection == 2 || infoFile.dateInfoSelection == 3)
				{
					filename.append("_" + intValue[1]);
				}
				if (infoFile.dateInfoSelection == 3)
				{
					filename.append("_" + intValue[2]);
				}
			}
		}

		if (infoFile.geoInfoSelection != 0)
		{
			vector<wxString> intValue = CConvertUtility::split(gpsFile, '.');
			if (intValue.size() > 3)
			{
				if (infoFile.geoInfoSelection == 1 || infoFile.geoInfoSelection == 2 || infoFile.geoInfoSelection == 3)
				{
					filename.append("_" + intValue[1]);
				}
				if (infoFile.geoInfoSelection == 2 || infoFile.geoInfoSelection == 3)
				{
					filename.append("_" + intValue[2]);
				}
				if (infoFile.geoInfoSelection == 3)
				{
					filename.append("_" + intValue[3]);
				}
			}
			else if (gpsFile == libelle)
			{
				filename.append("_" + libelle);
			}
		}
	}
	else
	{
		if (infoFile.geoInfoSelection != 0)
		{
			vector<wxString> intValue = CConvertUtility::split(gpsFile, '.');
			if (intValue.size() > 3)
			{
				if (infoFile.geoInfoSelection == 1 || infoFile.geoInfoSelection == 2 || infoFile.geoInfoSelection == 3)
				{
					filename.append(intValue[1]);
				}
				if (infoFile.geoInfoSelection == 2 || infoFile.geoInfoSelection == 3)
				{
					filename.append("_" + intValue[2]);
				}
				if (infoFile.geoInfoSelection == 3)
				{
					filename.append("_" + intValue[3]);
				}
			}
			else if (gpsFile == libelle)
			{
				filename.append(libelle);
			}
		}

		if (infoFile.dateInfoSelection != 0)
		{
			vector<wxString> intValue = CConvertUtility::split(dateFile, '.');
			if (intValue.size() == 3)
			{
				if (infoFile.dateInfoSelection == 1 || infoFile.dateInfoSelection == 2 || infoFile.dateInfoSelection ==
					3)
				{
					filename.append("_" + intValue[0]);
				}
				if (infoFile.dateInfoSelection == 2 || infoFile.dateInfoSelection == 3)
				{
					filename.append("_" + intValue[1]);
				}
				if (infoFile.dateInfoSelection == 3)
				{
					filename.append("_" + intValue[2]);
				}
			}
		}
	}

	return filename;
}

void CListPicture::CreateFolder(const wxString& newFolder)
{
	if (!wxDir::Exists(newFolder))
		wxDir::Make(newFolder);
}

wxString CListPicture::CreateExportFolder(const InfoExportFile& infoFile, const wxString& folderDestination,
                                          const wxString& dateFile, const wxString& gpsFile)
{
	wxString separator_folder;
	wxString libelle = CLibResource::LoadStringFromResource("LBLNOTGEO", 1);
	wxString folderComplete = folderDestination;

#if __APPLE__
	separator_folder = "/";
#else
	separator_folder = "\\";
#endif

	if (infoFile.priority == 0)
	{
		if (infoFile.dateInfoSelection != 0)
		{
			vector<wxString> intValue = CConvertUtility::split(dateFile, '.');
			if (intValue.size() == 3)
			{
				if (infoFile.dateInfoSelection == 1 || infoFile.dateInfoSelection == 2 || infoFile.dateInfoSelection ==
					3)
				{
					folderComplete.append(separator_folder + intValue[0]);
					CreateFolder(folderComplete);
				}
				if (infoFile.dateInfoSelection == 2 || infoFile.dateInfoSelection == 3)
				{
					folderComplete.append(separator_folder + intValue[1]);
					CreateFolder(folderComplete);
				}
				if (infoFile.dateInfoSelection == 3)
				{
					folderComplete.append(separator_folder + intValue[2]);
					CreateFolder(folderComplete);
				}
			}
		}

		if (infoFile.geoInfoSelection != 0)
		{
			vector<wxString> intValue = CConvertUtility::split(gpsFile, '.');
			if (intValue.size() > 3)
			{
				if (infoFile.geoInfoSelection == 1 || infoFile.geoInfoSelection == 2 || infoFile.geoInfoSelection == 3)
				{
					folderComplete.append(separator_folder + intValue[1]);
					CreateFolder(folderComplete);
				}
				if (infoFile.geoInfoSelection == 2 || infoFile.geoInfoSelection == 3)
				{
					folderComplete.append(separator_folder + intValue[2]);
					CreateFolder(folderComplete);
				}
				if (infoFile.geoInfoSelection == 3)
				{
					folderComplete.append(separator_folder + intValue[3]);
					CreateFolder(folderComplete);
				}
			}
			else if (gpsFile == libelle)
			{
				folderComplete.append(separator_folder + libelle);
				CreateFolder(folderComplete);
			}
		}
	}
	else
	{
		if (infoFile.geoInfoSelection != 0)
		{
			vector<wxString> intValue = CConvertUtility::split(gpsFile, '.');
			if (intValue.size() > 3)
			{
				if (infoFile.geoInfoSelection == 1 || infoFile.geoInfoSelection == 2 || infoFile.geoInfoSelection == 3)
				{
					folderComplete.append(separator_folder + intValue[1]);
					CreateFolder(folderComplete);
				}
				if (infoFile.geoInfoSelection == 2 || infoFile.geoInfoSelection == 3)
				{
					folderComplete.append(separator_folder + intValue[2]);
					CreateFolder(folderComplete);
				}
				if (infoFile.geoInfoSelection == 3)
				{
					folderComplete.append(separator_folder + intValue[3]);
					CreateFolder(folderComplete);
				}
			}
			else if (gpsFile == libelle)
			{
				folderComplete.append(separator_folder + libelle);
				CreateFolder(folderComplete);
			}
		}

		if (infoFile.dateInfoSelection != 0)
		{
			vector<wxString> intValue = CConvertUtility::split(dateFile, '.');
			if (intValue.size() == 3)
			{
				if (infoFile.dateInfoSelection == 1 || infoFile.dateInfoSelection == 2 || infoFile.dateInfoSelection ==
					3)
				{
					folderComplete.append(separator_folder + intValue[0]);
					CreateFolder(folderComplete);
				}
				if (infoFile.dateInfoSelection == 2 || infoFile.dateInfoSelection == 3)
				{
					folderComplete.append(separator_folder + intValue[1]);
					CreateFolder(folderComplete);
				}
				if (infoFile.dateInfoSelection == 3)
				{
					folderComplete.append(separator_folder + intValue[2]);
					CreateFolder(folderComplete);
				}
			}
		}
	}
	return folderComplete;
}

void CListPicture::ExportFile(const wxString& filename, CThumbnailData* data, InfoExportFile infoFile,
                              wxString destinationFolder, int optionPicture, int qualityPicture)
{
	CLibPicture libPicture;
	wxString file;
	CSqlFindCriteria sqlFindCriteria;
	wxString criteriaDate = "";
	wxString criteriaGps = "";
	CriteriaVector m_criteriaVector;
	sqlFindCriteria.SearchCriteria(&m_criteriaVector, data->GetNumPhotoId());

	for (CCriteria criteria : m_criteriaVector)
	{
		if (criteria.GetCategorieId() == 1)
			criteriaGps = criteria.GetLibelle();
		else if (criteria.GetCategorieId() == 3)
			criteriaDate = criteria.GetLibelle();
	}

	if (infoFile.changeFilename)
		file = GenerateFileName(infoFile, criteriaDate, criteriaGps);
	else
		file = CFileUtility::GetFileName(filename);

	wxString outputFolder = CreateExportFolder(infoFile, destinationFolder, criteriaDate, criteriaGps);


	if (infoFile.outputFormat == 0)
	{
		wxString newFile = outputFolder;

#if defined(WIN32)
		newFile += "\\" + file;

#else
		newFile += "/" + file;
#endif

		if (infoFile.changeFilename)
		{
			wxArrayString array;
			wxDir::GetAllFiles(outputFolder, &array);
			newFile.append("_" + to_string(array.GetCount()) + "." + CFileUtility::GetFileExtension(filename));
		}
		wxCopyFile(filename, newFile, true);
	}
	else
	{
		if (!libPicture.TestIsVideo(filename) && !libPicture.TestIsAnimation(filename) && !libPicture.
			TestIsPDF(filename))
		{
			if (!infoFile.changeFilename)
			{
				wxString extension = CFileUtility::GetFileExtension(filename);
				size_t index = file.find(extension) - 1;
				file = file.SubString(0, index);
			}
			else
			{
				wxArrayString array;
				wxDir::GetAllFiles(outputFolder, &array);
#if defined(WIN32)
				file = outputFolder + "\\" + file;
#else
				file = outputFolder + "/" + file;
#endif
				file.append("_" + to_string(array.GetCount()));
			}


			switch (infoFile.outputFormat)
			{
			case TIFF:
				file.append(".tif");
				break;
			case PNG:
				file.append(".png");
				break;
			case GIF:
				file.append(".gif");
				break;
			case JPEG:
				file.append(".jpg");
				break;
			case BMP:
				//BMP
				file.append(".bmp");
				break;
			case TGA:
				file.append(".tga");
				//TGA
				break;
			case PCX:
				//PCX
				file.append(".pcx");
				break;
			case MNG:
				//MNG
				file.append(".mng");
				break;
			case PNM:
				//PNM
				file.append(".pnm");
				break;
			case JPC:
				file.append(".jpc");
				break;
			case JPEG2000:
				file.append(".jp2");
				break;
			case PPM:
				file.append(".ppm");
				break;
			case PDF:
				file.append(".pdf");
				break;
			default: ;
			}

			//Sauvegarde de l'image
			CImageLoadingFormat* bitmap = libPicture.LoadPicture(filename);
			if (bitmap != nullptr && bitmap->IsOk())
			{
				if (bitmap->GetWidth() == 0 || bitmap->GetHeight() == 0)
				{
					libPicture.SavePicture(file, bitmap, optionPicture, qualityPicture);
				}
				delete bitmap;
			}
		}
		else
		{
			file.append("." + CFileUtility::GetFileExtension(filename));
			wxCopyFile(filename, file, true);
		}
	}
}

void CListPicture::ExportFileCmd(wxCommandEvent& event)
{
	vector<CThumbnailData*> listItem;
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
				int optionPicture;
				int qualityPicture;
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
					case 2: //PNG
						infoFile.outputFormat = PNG;
						break;
					case 3: //GIF
						infoFile.outputFormat = GIF;
						break;
					case 4: //JPEG
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
					default: ;
					}
				}

				wxString caption = CLibResource::LoadStringFromResource(L"LBLExportCaption", 1);
				wxString text = CLibResource::LoadStringFromResource(L"LBLExportText", 1);
				wxString deleteMessage = CLibResource::LoadStringFromResource(L"LBLExportMessage", 1);
				wxString deleteFinalMessage = CLibResource::LoadStringFromResource(L"LBLExportFinalMessage", 1);
				wxString informations = CLibResource::LoadStringFromResource(L"LBLINFORMATIONS", 1);


				if (infoFile.outputFormat != 0)
				{
					CLibPicture libPicture;
					libPicture.SavePictureOption(infoFile.outputFormat, optionPicture, qualityPicture);
				}

				wxProgressDialog dialog(caption, text, listItem.size(), this,
				                        wxPD_APP_MODAL | wxPD_CAN_ABORT | wxPD_AUTO_HIDE);
				for (int i = 0; i < listItem.size(); i++)
				{
					int j = i + 1;
					CThumbnailData* data = listItem.at(i);
					wxString filename = data->GetFilename();
					wxString message = text + to_string(j) + "/" + to_string(listItem.size());
					ExportFile(filename, data, infoFile, folderPath, optionPicture, qualityPicture);
					if (false == dialog.Update(i, message))
						break;
				}
			}
		}
	}
	else
	{
		wxString noItemSelected = CLibResource::LoadStringFromResource(L"NoItemSelected", 1);
		wxString exportFileInfos = CLibResource::LoadStringFromResource(L"ExportFileInfos", 1);
		wxMessageBox(noItemSelected, exportFileInfos, wxICON_INFORMATION);
	}
}

void CListPicture::DeleteFile(wxCommandEvent& event)
{
	vector<CThumbnailData*> listItem;
	thumbnailFolder->GetSelectItem(listItem);
	if (listItem.size() > 0)
	{
		wxString caption = CLibResource::LoadStringFromResource(L"LBLDeleteCaption", 1);
		wxString text = CLibResource::LoadStringFromResource(L"LBLDeleteText", 1);
		wxString deleteMessage = CLibResource::LoadStringFromResource(L"LBLDeleteMessage", 1);
		wxString deleteFinalMessage = CLibResource::LoadStringFromResource(L"LBLDeleteFinalMessage", 1);
		wxString informations = CLibResource::LoadStringFromResource(L"LBLINFORMATIONS", 1);

		wxProgressDialog dialog(caption, text, listItem.size(), this, wxPD_APP_MODAL | wxPD_CAN_ABORT | wxPD_AUTO_HIDE);
		for (int i = 0; i < listItem.size(); i++)
		{
			int j = i + 1;
			CThumbnailData* data = listItem.at(i);
			wxString filename = data->GetFilename();
			wxString message = text + to_string(j) + "/" + to_string(listItem.size());
			wxRemoveFile(filename);
			if (false == dialog.Update(i, message))
				break;
		}

		/*
		CopyFileDlg copyFile(this);
		copyFile.SetSelectItem(&listItem);
		copyFile.SetMode(0);
		copyFile.SetLibelle(caption, text, deleteMessage, deleteFinalMessage, informations);
        copyFile.Start();
		copyFile.ShowModal();
		*/
		//Mise à jour du répertoire des fichiers
		auto mainWindow = static_cast<CMainWindow*>(this->FindWindowById(MAINVIEWERWINDOWID));
		if (mainWindow != nullptr)
		{
			wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_REFRESHFOLDER);
			mainWindow->GetEventHandler()->AddPendingEvent(evt);
		}
	}
	else
	{
		wxString noItemSelected = CLibResource::LoadStringFromResource(L"NoItemSelected", 1);
		wxString deleteFileInfos = CLibResource::LoadStringFromResource(L"DeleteFileInfos", 1);
		wxMessageBox(noItemSelected, deleteFileInfos, wxICON_INFORMATION);
	}
}

int CListPicture::ImageSuivante()
{
	if (thumbnailFolder != nullptr)
		return thumbnailFolder->ImageSuivante();
	return 0;
}

int CListPicture::ImagePrecedente()
{
	if (thumbnailFolder != nullptr)
		return thumbnailFolder->ImagePrecedente();
	return 0;
}

int CListPicture::ImageFin()
{
	if (thumbnailFolder != nullptr)
		return thumbnailFolder->ImageFin();
	return 0;
}

int CListPicture::ImageDebut()
{
	if (thumbnailFolder != nullptr)
		return thumbnailFolder->ImageDebut();
	return 0;
}

vector<wxString> CListPicture::GetFileList()
{
	vector<wxString> list;
	if (thumbnailFolder != nullptr)
		return thumbnailFolder->GetFileList();
	return list;
}

wxString CListPicture::GetFilename(const int& numItem)
{
	if (thumbnailFolder != nullptr)
		return thumbnailFolder->GetFilename(numItem);
	return "";
}

void CListPicture::UpdateScreenRatio()
{
	if (windowManager != nullptr)
		windowManager->UpdateScreenRatio();
}

void CListPicture::CopyFile(wxCommandEvent& event)
{
	vector<CThumbnailData*> listItem;
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
			wxProgressDialog dialog(caption, text, listItem.size(), this,
			                        wxPD_APP_MODAL | wxPD_CAN_ABORT | wxPD_AUTO_HIDE);
			for (int i = 0; i < listItem.size(); i++)
			{
				int j = i + 1;
				CThumbnailData* data = listItem.at(i);
				wxString filename = data->GetFilename();
				wxString message = text + to_string(j) + "/" + to_string(listItem.size());

				wxString file = CFileUtility::GetFileName(filename);
				wxString newFile = folderPath;

#ifdef WIN32
				newFile += "\\" + file;
#else
				newFile += "/" + file;
#endif
				wxCopyFile(filename, newFile, true);

				//wxRemoveFile(filename);
				if (false == dialog.Update(i, message))
					break;
			}
		}
	}
	else
	{
		wxString noItemSelected = CLibResource::LoadStringFromResource(L"NoItemSelected", 1);
		wxString copyFileInfo = CLibResource::LoadStringFromResource(L"CopyFileInfo", 1);
		wxMessageBox(noItemSelected, copyFileInfo, wxICON_INFORMATION);
	}
}

void CListPicture::Resize()
{
	if (windowManager != nullptr)
	{
		windowManager->SetSize(GetWindowWidth(), GetWindowHeight());
		//windowManager->Resize();
		this->ForceRefresh();
	}
}
