#include <header.h>
#include "ModificationManager.h"
#include <wx/dir.h>
#include <wx/filename.h>
#include <ImageLoadingFormat.h>
#include <wx/filename.h>
CModificationManager::CModificationManager(const wxString& folder)
{
	orientation = 0;
	nbModification = 0;
	numModification = 0;
	wxFileName file(folder, "history");
	this->folder = file.GetFullPath();
}

void CModificationManager::Init(CImageLoadingFormat* bitmap)
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
		wxFileName file(folder, filename);
        wxRemoveFile(file.GetFullPath());
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
	wxFileName filename(folder, wxString::Format("%d.png", numModification));
	return filename.GetFullPath();
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

	auto bitmap = new CImageLoadingFormat();
	bitmap->ReadFile(filename);
	if (numModification > 0)
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

	nbModification = numModification;

	numModification++;
	nbModification++;

	listLibelle.push_back(libelle);

	wxString filename = GetFilenameWithModification(numModification);

	if (wxFileName::FileExists(filename))
		wxRemoveFile(filename);

	bitmap->WriteFile(filename);
}
