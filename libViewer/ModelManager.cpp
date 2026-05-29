#include <header.h>
#include "ModelManager.h"
#include <LibResource.h>
#include <FileUtility.h>
#include "DownloadFile.h"
#include <wx/filename.h>
#include <wx/wfstream.h>
#include <wx/txtstrm.h>

using namespace Regards::Viewer;
using namespace Regards::Internet;

CModelManager::CModelManager(wxWindow* parent)
    : parent_(parent)
{
}

bool CModelManager::VerifyAndUpdate()
{
	wxString documentPath = CFileUtility::GetDocumentFolderPath();
	wxString fileHash = documentPath + wxFILE_SEP_PATH + "model" + wxFILE_SEP_PATH + "hash.txt";

    bool fileExist = false;
    //Vérification de la version du hash
    if (wxFileExists(fileHash))
	{
        wxString md5 = "";
		wxFileInputStream input(fileHash);
		wxTextInputStream text(input, wxT("\x09"), wxConvUTF8);
		while (input.IsOk() && !input.Eof())
		{
			md5 = text.ReadLine();
			break;
		}

		fileExist = true;
        wxString model_hash = CLibResource::LoadStringFromResource("REGARDSMODELHASH", 1);
        if(model_hash != md5)
        {
            fileExist = false;
        }
	}

	if (!fileExist)
	{
        NewModelsAvailable();
        if (!wxFileExists(fileHash))
        {
            return false;
        }
	}
    return true;
}


void CModelManager::NewModelsAvailable()
{
	bool fileExist = false;
	wxString localVersion = CLibResource::LoadStringFromResource("LBLMODELHASH", 1);
	wxString line = "";
	wxString documentPath = CFileUtility::GetDocumentFolderPath();
	wxString tempModel = CFileUtility::GetTempFile("model.zip", true);
	wxString resourcePath = documentPath + wxFILE_SEP_PATH + "model";
	wxString fileHash = resourcePath + wxFILE_SEP_PATH + "hash.txt";
    const wxString serverURL = CLibResource::LoadStringFromResource("LBLWEBSITEMODELDOWNLOAD", 1);
    CDownloadFile downloader(serverURL);
    
	if (wxFileExists(fileHash))
	{
		wxFileInputStream input(fileHash);
		wxTextInputStream text(input, wxT("\x09"), wxConvUTF8);
		while (input.IsOk() && !input.Eof())
		{
			line = text.ReadLine();
			break;
		}

		fileExist = true;
	}

	if (!fileExist || localVersion != line)
	{
        wxProgressDialog dialog(
            "Downloading models ...", "Please wait...", 100, parent_,
            wxPD_APP_MODAL | wxPD_AUTO_HIDE | wxPD_CAN_ABORT |
            wxPD_ELAPSED_TIME | wxPD_ESTIMATED_TIME | wxPD_REMAINING_TIME | wxPD_SMOOTH);


        downloader.DownloadFile(&dialog, tempModel,
                                CFileUtility::GetResourcesFolderPathWithExt("ca-bundle.crt"));
        dialog.Close();
	}


	if (wxFileExists(tempModel))
	{
		downloader.ExtractZipFiles(tempModel, resourcePath, parent_);
	}

}
