#include "wx_pch.h"
#include "CopyFileDlg.h"
#include <FileUtility.h>
#ifndef WX_PRECOMP
	//(*InternalHeadersPCH(CopyFileDlg)
	//*)
#endif
//(*InternalHeaders(CopyFileDlg)
#include <wx/xrc/xmlres.h>
#include <wx/dir.h>
#include <libResource.h>
#include <ConvertUtility.h>
#include <SqlFindCriteria.h>
#include <SqlCriteria.h>
#include <SqlGps.h>
#include <SqlPhotoCriteria.h>
#include <SqlPhotos.h>
using namespace Regards::Sqlite;
//*)

//(*IdInit(CopyFileDlg)
//*)

BEGIN_EVENT_TABLE(CopyFileDlg, wxDialog)
//(*EventTable(CopyFileDlg)
	EVT_IDLE(CopyFileDlg::OnIdle)
	EVT_SIZE(CopyFileDlg::OnSize)
	//*)
END_EVENT_TABLE()

CopyFileDlg::CopyFileDlg(wxWindow* parent)
{
	//(*Initialize(CopyFileDlg)
	wxXmlResource::Get()->LoadObject(this,parent,_T("CopyFileDlg"),_T("wxDialog"));
	Gauge1 = (wxGauge*)FindWindow(XRCID("ID_GAUGE1"));
	StaticText1 = (wxStaticText*)FindWindow(XRCID("ID_STATICTEXT1"));
	StaticText2 = (wxStaticText*)FindWindow(XRCID("ID_STATICTEXT2"));
	btOK = (wxButton*)FindWindow(XRCID("ID_BUTTON1"));
	progress = 0;
	Gauge1->SetRange(100);
	Gauge1->SetValue(0);
	Connect(XRCID("ID_BUTTON1"),wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CopyFileDlg::OnButton1Click);
	Connect(wxID_ANY,wxEVT_INIT_DIALOG,(wxObjectEventFunction)&CopyFileDlg::OnInit);
	//*)

	this->SetBackgroundColour(wxColour(64, 64, 64));
}

CopyFileDlg::~CopyFileDlg()
{
	//(*Destroy(CopyFileDlg)
	//*)
}

void CopyFileDlg::SetMode(const int &mode)
{
	this->mode = mode;
}

void CopyFileDlg::Start()
{
	start = true;
}

void CopyFileDlg::SetNewGeoInfos(const float &latitude, const float &longitude,const wxString &lat, const wxString &lng, const wxString &geoInfos)
{
    this->latitude = latitude;
    this->longitude = longitude;
    this->lat = lat;
    this->lng = lng;
    this->geoInfos = geoInfos;
}

void CopyFileDlg::SetNewDate(const wxDateTime &newDate, const wxString &selectDate)
{
    this->newDate = newDate;
    this->selectDate = selectDate;
}

void CopyFileDlg::SetSelectItem(vector<CThumbnailData *> * listItem)
{
	this->listItem = listItem;
	if (listItem != nullptr)
		Gauge1->SetRange((int)listItem->size());
}

void CopyFileDlg::SetLibelle(const wxString &caption, const wxString &text, const wxString &message
	, const wxString &finalMessage, const wxString &informaitons)
{
	this->caption = caption;
	this->text = text;
	this->message = message;
	this->finalMessage = finalMessage;
	this->informations = informaitons;

	StaticText1->SetLabelText("File :");
	this->SetTitle("File Copy");
}

void CopyFileDlg::SetDestinationFolder(const wxString &folder)
{
	destinationFolder = folder;
}

void CopyFileDlg::SetInfosFile(const InfoExportFile & infoFile)
{
	this->infoFile = infoFile;
	if (infoFile.outputFormat != 0)
	{
		libPicture.SavePictureOption(infoFile.outputFormat, optionPicture, qualityPicture);
	}

}

void CopyFileDlg::OnSize(wxSizeEvent& evt)
{

}


void CopyFileDlg::CreateFolder(const wxString &newFolder)
{
	if (!wxDir::Exists(newFolder))
		wxDir::Make(newFolder);
}

wxString CopyFileDlg::GenerateFileName(const InfoExportFile & infoFile, const wxString &dateFile, const wxString &gpsFile)
{
	wxString libelle = CLibResource::LoadStringFromResource("LBLNOTGEO", 1);
	wxString filename ;
	if (infoFile.priority == 0)
	{
		if (infoFile.dateInfoSelection != 0)
		{
			vector<wxString> intValue = CConvertUtility::split(dateFile, '.');
			if (intValue.size() == 3)
			{
				if (infoFile.dateInfoSelection == 1 || infoFile.dateInfoSelection == 2 || infoFile.dateInfoSelection == 3)
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
				if (infoFile.dateInfoSelection == 1 || infoFile.dateInfoSelection == 2 || infoFile.dateInfoSelection == 3)
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


wxString CopyFileDlg::CreateExportFolder(const InfoExportFile & infoFile, const wxString &folderDestination, const wxString &dateFile, const wxString &gpsFile)
{
    
    wxString separatorFolder = "\\";
	wxString libelle = CLibResource::LoadStringFromResource("LBLNOTGEO", 1);
	wxString folderComplete = folderDestination;
    
#if __APPLE__
    separatorFolder = "/";
#else
    separatorFolder = "\\";
#endif
    
	if (infoFile.priority == 0)
	{
		if (infoFile.dateInfoSelection != 0)
		{
			vector<wxString> intValue = CConvertUtility::split(dateFile, '.');
			if (intValue.size() == 3)
			{
				if (infoFile.dateInfoSelection == 1 || infoFile.dateInfoSelection == 2 || infoFile.dateInfoSelection == 3)
				{
					folderComplete.append(separatorFolder + intValue[0]);
					CreateFolder(folderComplete);
				}
				if (infoFile.dateInfoSelection == 2 || infoFile.dateInfoSelection == 3)
				{
					folderComplete.append(separatorFolder + intValue[1]);
					CreateFolder(folderComplete);
				}
				if (infoFile.dateInfoSelection == 3)
				{
					folderComplete.append(separatorFolder + intValue[2]);
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
					folderComplete.append(separatorFolder + intValue[1]);
					CreateFolder(folderComplete);
				}
				if (infoFile.geoInfoSelection == 2 || infoFile.geoInfoSelection == 3)
				{
					folderComplete.append(separatorFolder + intValue[2]);
					CreateFolder(folderComplete);
				}
				if (infoFile.geoInfoSelection == 3)
				{
					folderComplete.append(separatorFolder + intValue[3]);
					CreateFolder(folderComplete);
				}
			}
			else if (gpsFile == libelle)
			{
				folderComplete.append(separatorFolder + libelle);
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
					folderComplete.append(separatorFolder + intValue[1]);
					CreateFolder(folderComplete);
				}
				if (infoFile.geoInfoSelection == 2 || infoFile.geoInfoSelection == 3)
				{
					folderComplete.append(separatorFolder + intValue[2]);
					CreateFolder(folderComplete);
				}
				if (infoFile.geoInfoSelection == 3)
				{
					folderComplete.append(separatorFolder + intValue[3]);
					CreateFolder(folderComplete);
				}
			}
			else if (gpsFile == libelle)
			{
				folderComplete.append(separatorFolder + libelle);
				CreateFolder(folderComplete);
			}
		}

		if (infoFile.dateInfoSelection != 0)
		{
			vector<wxString> intValue = CConvertUtility::split(dateFile, '.');
			if (intValue.size() == 3)
			{
				if (infoFile.dateInfoSelection == 1 || infoFile.dateInfoSelection == 2 || infoFile.dateInfoSelection == 3)
				{
					folderComplete.append(separatorFolder + intValue[0]);
					CreateFolder(folderComplete);
				}
				if (infoFile.dateInfoSelection == 2 || infoFile.dateInfoSelection == 3)
				{
					folderComplete.append(separatorFolder + intValue[1]);
					CreateFolder(folderComplete);
				}
				if (infoFile.dateInfoSelection == 3)
				{
					folderComplete.append(separatorFolder + intValue[2]);
					CreateFolder(folderComplete);
				}
			}

		}


	}

	return folderComplete;
}

void CopyFileDlg::CopyFile(const wxString & filename, CThumbnailData * data)
{
	wxString file = CFileUtility::GetFileName(filename);
	wxString newFile = destinationFolder;

#if __APPLE__
	newFile += "/" + file;
#else
	newFile += "\\" + file;
#endif
	wxCopyFile(filename, newFile, true);
}

void CopyFileDlg::DeleteFile(const wxString & filename, CThumbnailData * data)
{
	wxRemoveFile(filename);
}

void CopyFileDlg::GeolocalizeFile(const wxString & filename)
{
#ifdef __APPLE__
    appleReadExif.WriteGps(filename, latitude, longitude);
#endif
    CSqlPhotos sqlPhotos;
    int numPhotoId = sqlPhotos.GetPhotoId(filename);
    
    bool isNew = false;
    CSqlCriteria sqlCriteria;
    CSqlGps sqlGps;
    CSqlPhotoCriteria sqlPhotoCriteria;
    
    int oldCriteriaId = sqlCriteria.GetCriteriaIdByCategorie(numPhotoId, 1);
    
    int numCriteriaId = sqlCriteria.GetOrInsertCriteriaId(1, 1, geoInfos, isNew);
    sqlPhotoCriteria.InsertPhotoCriteria(numPhotoId, numCriteriaId);
    if(oldCriteriaId != -1 && numCriteriaId != oldCriteriaId)
        sqlPhotoCriteria.DeletePhotoCriteria(numPhotoId, oldCriteriaId);
    
    sqlGps.DeleteGps(filename);
    sqlGps.InsertGps(filename, lat, lng);
}

void CopyFileDlg::ChangeDateFile(const wxString & filename)
{
#ifdef __APPLE__
    appleReadExif.WriteDateTime(filename, newDate);
#endif
    bool isNew = false;
    CSqlCriteria sqlCriteria;
    CSqlPhotoCriteria sqlPhotoCriteria;
    CSqlPhotos sqlPhotos;
    
    int numPhotoId = sqlPhotos.GetPhotoId(filename);
    int oldCriteriaId = sqlCriteria.GetCriteriaIdByCategorie(numPhotoId, 3);
    
    int numCriteriaId = sqlCriteria.GetOrInsertCriteriaId(1, 3, selectDate, isNew);
    sqlPhotoCriteria.InsertPhotoCriteria(numPhotoId, numCriteriaId);
    if(oldCriteriaId != -1 && numCriteriaId != oldCriteriaId)
        sqlPhotoCriteria.DeletePhotoCriteria(numPhotoId, oldCriteriaId);
}


void CopyFileDlg::ExportFile(const wxString & filename, CThumbnailData * data)
{
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

#if __APPLE__
		newFile += "/" + file;
#else
		newFile += "\\" + file;
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
		if (!libPicture.TestIsVideo(filename) && !libPicture.TestIsAnimation(filename))
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
#if __APPLE__
				file = outputFolder + "/" + file;
#else
				file = outputFolder + "\\" + file;
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
			}

			//Sauvegarde de l'image
			CRegardsBitmap * bitmap = libPicture.LoadPicture(filename);
			libPicture.SavePicture(file, bitmap, optionPicture, qualityPicture);
			delete bitmap;
		}
		else
		{
			file.append("." + CFileUtility::GetFileExtension(filename));
			wxCopyFile(filename, file, true);
		}
	}
}


void CopyFileDlg::OnIdle(wxIdleEvent& evt)
{
	if (listItem != nullptr)
	{
		if (start && progress < listItem->size())
		{

			Gauge1->SetValue(progress);

			wxString statusMessage;
			CThumbnailData * data = listItem->at(progress);
			wxString filename = data->GetFilename();
			switch (mode)
			{
				case 0:
					DeleteFile(filename, data);
					break;

				case 1:
					CopyFile(filename, data);
					break;

				case 2:
					ExportFile(filename, data);
					break;
                    
                case 3:
                    GeolocalizeFile(filename);
                    break;
                    
                case 4:
                    ChangeDateFile(filename);
                    break;
			}

			StaticText2->SetLabel(filename);

			progress++;
		}
		else if (start)
		{
			start = false;
			wxMessageBox(finalMessage, caption, wxICON_INFORMATION);
			this->Close();
		}
	}
}

void CopyFileDlg::OnInit(wxInitDialogEvent& event)
{
}

void CopyFileDlg::OnButton1Click(wxCommandEvent& event)
{
	this->Close();
}
