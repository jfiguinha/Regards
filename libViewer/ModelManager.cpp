#include <header.h>
#include "ModelManager.h"
#include <LibResource.h>
#include <FileUtility.h>
#include <wx/wfstream.h>
#include <wx/txtstrm.h>
#include <wx/progdlg.h>
#include <wx/filename.h>
#include <httprequest.h>
#include <wx/zipstrm.h>
using namespace Regards::Viewer;
using namespace Regards::Internet;

CModelManager::CModelManager(wxWindow* parent)
    : parent_(parent)
{
}

bool CModelManager::VerifyAndUpdate()
{
	wxString documentPath = CFileUtility::GetDocumentFolderPathWithFilename("model");
	wxFileName file(documentPath, "hash.txt");

	wxString fileHash = file.GetFullPath();//documentPath + wxFILE_SEP_PATH + "model" + wxFILE_SEP_PATH + "hash.txt";

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


bool CModelManager::ExtractZipFiles(
    const wxString& zipFile,
    const wxString& targetDir,
    wxWindow* parent)
{
    wxFileInputStream input(zipFile);

    if (!input.IsOk())
    {
        wxLogError("Cannot open '%s'", zipFile);
        return false;
    }

    wxZipInputStream zip(input);

    const int totalEntries =
        std::max(
            1,
            static_cast<int>(
                zip.GetTotalEntries()));

    wxProgressDialog progress(
        "Extraction",
        "Please wait...",
        totalEntries,
        parent,
        wxPD_APP_MODAL |
        wxPD_AUTO_HIDE |
        wxPD_SMOOTH);

    int current = 0;

    std::unique_ptr<wxZipEntry> entry;

    while ((entry.reset(zip.GetNextEntry())),
        entry)
    {
        wxString output =
            targetDir +
            wxFILE_SEP_PATH +
            entry->GetInternalName();

        wxFileName fn(output);

        if (entry->IsDir())
        {
            wxFileName::Mkdir(
                output,
                wxS_DIR_DEFAULT,
                wxPATH_MKDIR_FULL);
        }
        else
        {
            wxFileName::Mkdir(
                fn.GetPath(),
                wxS_DIR_DEFAULT,
                wxPATH_MKDIR_FULL);

            wxFileOutputStream file(output);

            if (!file.IsOk())
            {
                wxLogError(
                    "Cannot create '%s'",
                    output);

                return false;
            }

            zip.Read(file);
        }

        ++current;

        if (!progress.Update(
            std::min(current, totalEntries),
            entry->GetName()))
        {
            return false;
        }
    }

    return true;
}


void CModelManager::NewModelsAvailable()
{
	bool isOk = false;
	bool fileExist = false;
	wxString localVersion = CLibResource::LoadStringFromResource("LBLMODELHASH", 1);
	wxString line = "";
	wxString documentPath = CFileUtility::GetDocumentFolderPath();
	wxString tempModel = CFileUtility::GetTempFile("model.zip", true);
	wxString resourcePath = CFileUtility::GetDocumentFolderPathWithFilename("model");
	wxFileName fileHash(resourcePath,"hash.txt");
    const wxString serverURL = CLibResource::LoadStringFromResource("LBLWEBSITEMODELDOWNLOAD", 1);

    
	if (wxFileExists(fileHash.GetFullPath()))
	{
		wxFileInputStream input(fileHash.GetFullPath());
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

		isOk = Regards::Internet::CHttpRequest::DownloadFile(serverURL, tempModel, &dialog);
        dialog.Close();
	}


	if (wxFileExists(tempModel))
	{
		ExtractZipFiles(tempModel, resourcePath, parent_);
	}

}
