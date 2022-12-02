#include <header.h>
#include "ModificationManager.h"
#include <wx/dir.h>
#include <wx/filename.h>
#include <ImageLoadingFormat.h>

CModificationManager::CModificationManager(const wxString& folder)
{
	orientation = 0;
	nbModification = 0;
	numModification = 0;
	this->folder = folder;
#ifdef WIN32
	this->folder.append("\\history");
#else
    this->folder.append("//history");
#endif
}

void CModificationManager::Init(CImageLoadingFormat * bitmap)
{
	if (!wxDir::Exists(folder))
	{
		wxFileName::Mkdir(folder);
	}

	EraseData();
	filenameBitmap = bitmap->GetFilename();
	orientation = bitmap->GetOrientation();
	listLibelle.push_back(bitmap->GetFilename());
	wxString filename = GetFilenameWithModification(numModification);
	bitmap->WriteFile(filename);
}

CModificationManager::~CModificationManager()
{
	EraseData();
}

void CModificationManager::EraseData()
{
	nbModification = 0;
	numModification = 0;
	listLibelle.clear();

	const wxDir dir(folder);
	if (!dir.IsOpened())
	{
		// deal with the error here - wxDir would already log an error message
		// explaining the exact reason of the failure
		return;
	}

	wxString filename;
	bool cont = dir.GetFirst(&filename);
	while (cont)
	{
#ifdef WIN32
		wxRemoveFile(folder + "\\" + filename);
#else
        ::wxRemoveFile(folder + "//" + filename);
#endif

		cont = dir.GetNext(&filename);
	}
}

unsigned int CModificationManager::GetNbModification()
{
	return nbModification;
}

unsigned int CModificationManager::GetNumModification()
{
	return numModification;
}

void CModificationManager::SetNumModification(const unsigned int& numModification)
{
	this->numModification = numModification;
}

wxString CModificationManager::GetFilenameWithModification(const unsigned int& numModification)
{
	wxString filename;
	filename.append(folder.begin(), folder.end());

#ifdef WIN32
	filename.append("\\" + to_string(numModification) + ".png");
#else
    filename.append("//" + to_string(numModification) + ".png");
#endif

	return filename;
}

wxString CModificationManager::GetModificationLibelle(const unsigned int& numModification)
{
	return listLibelle.at(numModification);
}

CImageLoadingFormat* CModificationManager::GetModification(const unsigned int& numModification)
{
	//CSqlHistory history;
	//return history.GetPhoto(numModification);
	wxString filename = GetFilenameWithModification(numModification);

	CImageLoadingFormat* bitmap = new CImageLoadingFormat();
	bitmap->ReadFile(filename);
	if(numModification > 0)
		bitmap->SetOrientation(0);
	else
		bitmap->SetOrientation(orientation);
	bitmap->SetFilename(filenameBitmap);
	this->numModification = numModification;

	return bitmap;
}

void CModificationManager::AddModification(CImageLoadingFormat* bitmap, const wxString& libelle)
{
	if (numModification < nbModification)
		listLibelle.erase(listLibelle.begin() + (numModification + 1), listLibelle.end());

	/*
	int startRemove = numModification;
	while(startRemove < nbModification)
	{
	    wxString filename = GetFilenameWithModification(startRemove);
	    
	    if(wxFileName::FileExists(filename))
	        ::wxRemoveFile(filename);
	    
	    startRemove++;
	}*/

	nbModification = numModification;

	numModification++;
	nbModification++;

	listLibelle.push_back(libelle);

	wxString filename = GetFilenameWithModification(numModification);

	if (wxFileName::FileExists(filename))
		wxRemoveFile(filename);

	bitmap->WriteFile(filename);
}
