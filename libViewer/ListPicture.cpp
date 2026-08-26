#include <header.h>
#include "ListPicture.h"
#include <FileUtility.h>
#include <SqlFindCriteria.h>
#include <ThumbnailData.h>
#include "MainTheme.h"
#include "MainThemeInit.h"
#include <LibResource.h>
#include "MainWindow.h"
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
#include <wx/filename.h>
#include <ImageLoadingFormat.h>
#include <WindowMain.h>
#include <config_id.h>
#include <memory>
#ifdef EXIV2
#include <MetadataExiv2.h>
#elif defined(WIN32)
#include <SetMetadataGps.h>
#include <SetMetadataDate.h>
#endif

#include <PhotoCriteriaUtility.h>
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
	std::vector<int> value = { 100, 150, 200, 250, 300, 350, 400, 450, 500, 550, 600, 650, 700 };
	wxRect rect;
	int positionTab = 3;
	bool checkValidity = false;
	CMainParam* config = CMainParamInit::getInstance();
	if (config != nullptr)
	{
		checkValidity = config->GetCheckThumbnailValidity();
		typeAffichage = config->GetTypeAffichage();
	}



	if (config != nullptr)
		config->GetSlideFolderPos(positionTab);
	CMainTheme* viewerTheme = CMainThemeInit::getInstance();

	if (viewerTheme != nullptr)
	{
		{
			CThemeSplitter theme;
			viewerTheme->GetSplitterTheme(&theme);
			windowManager = new CWindowManager(this, wxID_ANY, theme);
		}
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

		{
			CThemeToolbar theme;
			//viewerTheme->GetThumbnailToolbarTheme(theme);
			viewerTheme->GetBitmapToolbarTheme(&theme);
			thumbToolbar = new CThumbnailToolBar(windowManager, wxID_ANY, theme, false);
			thumbToolbar->SetTabValue(value);
			thumbToolbar->SetTrackBarPosition(positionTab - 1);

			windowManager->AddWindow(thumbToolbar, Pos::wxBOTTOM, true, thumbToolbar->GetHeight(), rect, wxID_ANY, false);
		}
		{
			CThemeToolBarZoom theme;
			viewerTheme->GetThumbnailToolbarZoomTheme(theme);
			//viewerTheme->GetBitmapToolbarTheme(&theme);
			thumbToolbarZoom = new CThumbnailToolBarZoom(windowManager, wxID_ANY, theme);
			windowManager->AddWindow(thumbToolbarZoom, Pos::wxTOP, true, thumbToolbarZoom->GetHeight(), rect, wxID_ANY,
				false);
		}

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

CListPicture::~CListPicture()
{
	// Correctif : thumbnailFolder peut rester nullptr si viewerTheme etait nullptr
	// au moment de la construction (voir constructeur) -> crash sinon.
	if (thumbnailFolder != nullptr)
	{
		int positionTab = thumbnailFolder->GetTabValue();
		CMainParam* config = CMainParamInit::getInstance();
		if (config != nullptr)
			config->SetSlideFolderPos(positionTab);
	}
}

int CListPicture::GetThumbnailHeight()
{
	if (!thumbnailFolder || !thumbscrollbar)
		return 0;

	return thumbnailFolder->GetIconeHeight()
		+ thumbscrollbar->GetBarHeight();
}

void CListPicture::ChangeTypeAffichage(const long& typeAffichage)
{
	this->typeAffichage = typeAffichage;
	// Correctif : garde nullptr manquante (incoherente avec le reste de la classe).
	if (thumbnailFolder != nullptr)
		thumbnailFolder->ChangeTypeAffichage(typeAffichage);
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

void CListPicture::SetListeFile(const int& typeAffichage, const bool& isDeleteFolder, const bool& isSqlUpdate)
{
	if (thumbnailFolder != nullptr)
	{
		thumbnailFolder->Init(typeAffichage, isDeleteFolder, isSqlUpdate);
		thumbnailFolder->SetCheck(true);
	}
}

void CListPicture::ThumbnailZoomOn(wxCommandEvent& event)
{
	if (thumbnailFolder)
		thumbnailFolder->ZoomOn();
}

void CListPicture::ThumbnailZoomOff(wxCommandEvent& event)
{
	if (thumbnailFolder)
		thumbnailFolder->ZoomOff();
}

void CListPicture::ThumbnailZoomPosition(wxCommandEvent& event)
{
	if (thumbnailFolder)
		thumbnailFolder->ZoomPosition(event.GetExtraLong());
}

// -----------------------------------------------------------------------
// Helpers de factorisation - commandes sur la selection courante
// -----------------------------------------------------------------------

bool CListPicture::GetSelectedItems(std::vector<wxString>& listItem, const wxString& noItemInfoKey)
{
	if (thumbnailFolder == nullptr)
		return false;

	thumbnailFolder->GetSelectItemFilename(listItem);
	if (listItem.empty())
	{
		wxString noItemSelected = CLibResource::LoadStringFromResource(L"NoItemSelected", 1);
		wxString info = CLibResource::LoadStringFromResource(noItemInfoKey, 1);
		wxMessageBox(noItemSelected, info, wxICON_INFORMATION);
		return false;
	}
	return true;
}

void CListPicture::RunWithProgressDialog(const wxString& caption, const wxString& text,
	const std::vector<wxString>& listItem,
	const std::function<void(const wxString&, int)>& action)
{
	wxProgressDialog dialog(caption, text, static_cast<int>(listItem.size()), this,
		wxPD_APP_MODAL | wxPD_CAN_ABORT | wxPD_AUTO_HIDE);
	for (size_t i = 0; i < listItem.size(); i++)
	{
		int j = static_cast<int>(i) + 1;
		wxString message = text + to_string(j) + "/" + to_string(listItem.size());
		action(listItem[i], static_cast<int>(i));
		if (false == dialog.Update(static_cast<int>(i), message))
			break;
	}
}

void CListPicture::GenerateIndexFile(wxCommandEvent& event)
{
	vector<wxString> listItem;
	if (!GetSelectedItems(listItem, L"GenerateIndexInformations"))
		return;

	IndexGenerator indexGenerator(this);
	indexGenerator.SetThumbnailSize(thumbnailFolder->GetIconeWidth(), thumbnailFolder->GetIconeHeight());
	indexGenerator.ShowModal();
	if (!indexGenerator.IsOk())
		return;

	int width, height, nbPictureLine;
	wxColour color = indexGenerator.GetColor();
	auto fontColor = wxColour(255 - color.Red(), 255 - color.Green(), 255 - color.Blue());
	indexGenerator.GetThumbnailSize(width, height);
	nbPictureLine = indexGenerator.GetNbPictureByLine();

	// Correctif : garde contre une division par zero si la valeur saisie est invalide.
	if (nbPictureLine <= 0)
		return;

	int getNbLine = static_cast<int>(listItem.size()) / nbPictureLine;
	if (getNbLine * nbPictureLine < static_cast<int>(listItem.size()))
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


	for (size_t i = 0; i < listItem.size(); i++)
	{
		wxString& filename = listItem[i];

		int x = (static_cast<int>(i) % nbPictureLine) * width;
		int y = (static_cast<int>(i) / nbPictureLine) * height + heightLibelle;

		CIcone* icone = thumbnailFolder->GetIconeByPath(filename);
		if (icone != nullptr && icone->GetPtData() != nullptr)
		{
			// Correctif fuite memoire : anciennement "new CIcone(...)" jamais delete.
			// unique_ptr choisi pour rester coherent avec le reste de la migration en cours.
			auto pBitmapIcone = std::make_unique<CIcone>(icone->GetPtData());
			pBitmapIcone->SetNumElement(static_cast<int>(i));
			pBitmapIcone->SetFilename(filename);
			pBitmapIcone->SetBackgroundColor(color);
			pBitmapIcone->SetTheme(themeIcone);
			pBitmapIcone->SetSizeIcone(width, height);
			pBitmapIcone->SetWindowPos(x, y);
			pBitmapIcone->RenderIcone(&memdc, 0, 0, false, false);
		}
	}

	memdc.SelectObject(wxNullBitmap);


	wxImage picture = bitmap.ConvertToImage();

	// Correctif fuite memoire : anciennement "new CImageLoadingFormat()" jamais delete.
	auto imageLoad = std::make_unique<CImageLoadingFormat>();
	imageLoad->SetPicture(picture);
	CSavePicture::SavePicture(nullptr, imageLoad.get(), "photoindex.png");
}

void CListPicture::GeolocalizeFileCmd(wxCommandEvent& event)
{
	vector<wxString> listItem;
	if (!GetSelectedItems(listItem, L"GeolocalizeFile"))
		return;

	CMapSelect mapSelect;
	wxString url = CLibResource::LoadStringFromResource(L"openstreetmapurl", 1); //"http://www.openstreetmap.org";
	wxString infoGpsLocalisation = mapSelect.SelectNewMapLocalisation(this, url);
	if (infoGpsLocalisation == "")
		return;

	wxString caption = CLibResource::LoadStringFromResource(L"LBLGeoModifCaption", 1);
	wxString text = CLibResource::LoadStringFromResource(L"LBLGeoModifText", 1);

	RunWithProgressDialog(caption, text, listItem, [&](const wxString& filename, int)
		{
			if (CPhotoCriteriaUtility::GeolocalizeFile(filename, mapSelect.GetLatitude(), mapSelect.GetLongitude(), infoGpsLocalisation))
			{
				CListOfWindow* fileGeolocalisation = CGpsEngine::getInstance();
				fileGeolocalisation->SendMessageToWindow(filename, 1);
			}
		});
}

void CListPicture::ChangeDateFileCmd(wxCommandEvent& event)
{
	vector<wxString> listItem;
	if (!GetSelectedItems(listItem, L"ChangeDateInfos"))
		return;

	wxDateTime dt = wxDateTime::Today();
	CCalendarSelect calendarSelect;
	if (!calendarSelect.SelectNewDate(this, dt))
		return;

	wxString caption = CLibResource::LoadStringFromResource(L"LBLDateModifCaption", 1);
	wxString text = CLibResource::LoadStringFromResource(L"LBLDateModifText", 1);

	RunWithProgressDialog(caption, text, listItem, [&](const wxString& filename, int)
		{
			CPhotoCriteriaUtility::ChangeDateFile(filename, calendarSelect.GetSelectDate(), calendarSelect.GetSelectStringDate());
		});
}


// -----------------------------------------------------------------------
// Helpers de factorisation - construction des noms/dossiers d'export
//
// Regle reconstituee et conservee a l'identique par rapport au code d'origine :
//   - Le "premier groupe" traite (date si priority==0, sinon geo) a son tout premier
//     jeton ajoute SANS separateur en tete, les suivants avec separateur.
//   - Le "second groupe" a TOUS ses jetons (y compris le premier) ajoutes AVEC
//     separateur en tete.
//   - Pour l'arborescence de dossiers (createFolder == true), tous les jetons sont
//     systematiquement prefixes du separateur (le chemin de base existe deja) et
//     CreateFolder() est appele progressivement, comme dans le code d'origine.
// Cette regle reproduit fidelement (y compris son cas limite : underscore de tete
// si le premier groupe est vide et le second non) le comportement observe dans
// GenerateFileName/CreateExportFolder avant refactorisation.
// -----------------------------------------------------------------------

std::vector<wxString> CListPicture::BuildDateTokens(const int& dateInfoSelection, const wxString& dateFile)
{
	std::vector<wxString> tokens;
	if (dateInfoSelection != 0)
	{
		vector<wxString> intValue = CConvertUtility::split(dateFile, '.');
		if (intValue.size() == 3)
		{
			if (dateInfoSelection == 1 || dateInfoSelection == 2 || dateInfoSelection == 3)
				tokens.push_back(intValue[0]);
			if (dateInfoSelection == 2 || dateInfoSelection == 3)
				tokens.push_back(intValue[1]);
			if (dateInfoSelection == 3)
				tokens.push_back(intValue[2]);
		}
	}
	return tokens;
}

std::vector<wxString> CListPicture::BuildGeoTokens(const int& geoInfoSelection, const wxString& gpsFile)
{
	std::vector<wxString> tokens;
	wxString libelle = CLibResource::LoadStringFromResource("LBLNOTGEO", 1);
	if (geoInfoSelection != 0)
	{
		vector<wxString> intValue = CConvertUtility::split(gpsFile, '.');
		if (intValue.size() > 3)
		{
			if (geoInfoSelection == 1 || geoInfoSelection == 2 || geoInfoSelection == 3)
				tokens.push_back(intValue[1]);
			if (geoInfoSelection == 2 || geoInfoSelection == 3)
				tokens.push_back(intValue[2]);
			if (geoInfoSelection == 3)
				tokens.push_back(intValue[3]);
		}
		else if (gpsFile == libelle)
		{
			tokens.push_back(libelle);
		}
	}
	return tokens;
}

void CListPicture::AppendTokens(wxString& result, const std::vector<wxString>& tokens, const wxString& separator,
	bool isFirstGroup, bool createFolder)
{
	for (size_t i = 0; i < tokens.size(); i++)
	{
		if (!createFolder && isFirstGroup && i == 0)
			result.append(tokens[i]);
		else
			result.append(separator + tokens[i]);

		if (createFolder)
			CreateFolder(result);
	}
}

wxString CListPicture::GenerateFileName(const InfoExportFile& infoFile, const wxString& dateFile,
	const wxString& gpsFile)
{
	std::vector<wxString> dateTokens = BuildDateTokens(infoFile.dateInfoSelection, dateFile);
	std::vector<wxString> geoTokens = BuildGeoTokens(infoFile.geoInfoSelection, gpsFile);

	wxString filename;
	if (infoFile.priority == 0)
	{
		AppendTokens(filename, dateTokens, "_", true, false);
		AppendTokens(filename, geoTokens, "_", false, false);
	}
	else
	{
		AppendTokens(filename, geoTokens, "_", true, false);
		AppendTokens(filename, dateTokens, "_", false, false);
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
	// Correctif : l'ancien code utilisait "\\" pour toute plateforme non-Apple, ce qui
	// est incorrect sous Linux. wxFileName::GetPathSeparator() donne le bon separateur
	// partout (et reproduit "\\" sous Windows, "/" sous macOS, comme avant).
	wxString separator_folder = wxFileName::GetPathSeparator();
	wxString folderComplete = folderDestination;

	std::vector<wxString> dateTokens = BuildDateTokens(infoFile.dateInfoSelection, dateFile);
	std::vector<wxString> geoTokens = BuildGeoTokens(infoFile.geoInfoSelection, gpsFile);

	if (infoFile.priority == 0)
	{
		AppendTokens(folderComplete, dateTokens, separator_folder, true, true);
		AppendTokens(folderComplete, geoTokens, separator_folder, true, true);
	}
	else
	{
		AppendTokens(folderComplete, geoTokens, separator_folder, true, true);
		AppendTokens(folderComplete, dateTokens, separator_folder, true, true);
	}

	return folderComplete;
}

void CListPicture::ExportFile(const wxString& filename, const int& numPhotoId, const InfoExportFile& infoFile,
	const wxString& destinationFolder, int optionPicture, int qualityPicture)
{
	CLibPicture libPicture;
	wxString file;
	CSqlFindCriteria sqlFindCriteria;
	wxString criteriaDate = "";
	wxString criteriaGps = "";
	CriteriaVector m_criteriaVector;
	sqlFindCriteria.SearchCriteria(&m_criteriaVector, numPhotoId);

	for (CCriteria& criteria : m_criteriaVector)
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
		wxString newFile = CConvertUtility::GeneratePath(outputFolder, file);

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
				auto name = wxFileName(filename);
				file = CConvertUtility::GeneratePath(outputFolder, name.GetName());
			}
			else
			{
				wxArrayString array;
				wxDir::GetAllFiles(outputFolder, &array);
				file = CConvertUtility::GeneratePath(outputFolder, file);
				file.append("_" + to_string(array.GetCount()));
			}


			if (infoFile.outputFormat != 0)
			{
				vector<wxString> listExtension = CLibResource::GetSavePictureExtension();
				file.append(listExtension.at(infoFile.outputFormat - 1).Lower());
			}

			//Sauvegarde de l'image
			CImageLoadingFormat* bitmap = libPicture.LoadPicture(filename);
			if (bitmap != nullptr && bitmap->IsOk())
			{
				libPicture.SavePicture(file, bitmap, optionPicture, qualityPicture);
			}
			if (bitmap != nullptr)
				delete bitmap;
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
	vector<wxString> listItem;
	if (!GetSelectedItems(listItem, L"ExportFileInfos"))
		return;

	CExportFile exportFile(this);
	exportFile.ShowModal();
	if (!exportFile.GetIsOK())
		return;

	wxDirDialog dlg(nullptr, "Choose destination directory", "",
		wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);

	if (dlg.ShowModal() != wxID_OK)
		return;

	// Correctif UB : anciennement non initialisees, elles n'etaient renseignees
	// que si infoFile.outputFormat != 0, mais toujours utilisees plus bas.
	int optionPicture = 0;
	int qualityPicture = 0;
	wxString folderPath = dlg.GetPath();
	InfoExportFile infoFile = exportFile.GetInfoExportFile();

	wxString caption = CLibResource::LoadStringFromResource(L"LBLExportCaption", 1);
	wxString text = CLibResource::LoadStringFromResource(L"LBLExportText", 1);

	if (infoFile.outputFormat != 0)
	{
		vector<wxString> listExtension = CLibResource::GetSavePictureExtension();
		wxString ext = listExtension.at(infoFile.outputFormat - 1).Lower();
		int format = CLibPicture::TestExtension(ext.substr(1, ext.size() - 1));

		CLibPicture libPicture;
		libPicture.SavePictureOption(format, optionPicture, qualityPicture);
	}

	RunWithProgressDialog(caption, text, listItem, [&](const wxString& filename, int)
		{
			CIcone* icone = thumbnailFolder->GetIconeByPath(filename);
			if (icone != nullptr)
				ExportFile(filename, icone->GetPtData()->GetNumPhotoId(), infoFile, folderPath, optionPicture, qualityPicture);
		});
}

void CListPicture::DeleteFile(wxCommandEvent& event)
{
	vector<wxString> listItem;
	if (!GetSelectedItems(listItem, L"DeleteFileInfos"))
		return;

	wxString caption = CLibResource::LoadStringFromResource(L"LBLDeleteCaption", 1);
	wxString text = CLibResource::LoadStringFromResource(L"LBLDeleteText", 1);

	RunWithProgressDialog(caption, text, listItem, [&](const wxString& filename, int)
		{
			wxRemoveFile(filename);
		});


	//Mise à jour du répertoire des fichiers
	auto mainWindow = static_cast<CMainWindow*>(this->FindWindowById(MAINVIEWERWINDOWID));
	if (mainWindow != nullptr)
	{
		wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_REFRESHFOLDER);
		mainWindow->GetEventHandler()->AddPendingEvent(evt);
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
	vector<wxString> listItem;
	if (!GetSelectedItems(listItem, L"CopyFileInfo"))
		return;

	// TODO : "LBLDeleteCaption" est reutilise ici depuis le code d'origine, tres
	// probablement un copier-coller depuis DeleteFile. A remplacer par une cle
	// dediee (ex. "LBLCopyCaption") des qu'elle existera dans les ressources.
	wxString caption = CLibResource::LoadStringFromResource(L"LBLDeleteCaption", 1);
	wxString text = CLibResource::LoadStringFromResource(L"LBLCopyText", 1);

	wxDirDialog dlg(nullptr, "Choose destination directory", "",
		wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);

	if (dlg.ShowModal() != wxID_OK)
		return;

	wxString folderPath = dlg.GetPath();
	RunWithProgressDialog(caption, text, listItem, [&](const wxString& filename, int)
		{
			wxString file = CFileUtility::GetFileName(filename);
			wxString newFile = CConvertUtility::GeneratePath(folderPath, file);
			wxCopyFile(filename, newFile, true);
		});
}

void CListPicture::Resize()
{
	if (windowManager != nullptr)
	{
		windowManager->SetSize(GetWindowWidth(), GetWindowHeight());
		needToRefresh = true;
	}
}