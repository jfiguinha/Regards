#include <header.h>
#ifdef WIN32

#include "Association.h"
#include <registryaccess.h>
#include <strsafe.h>
#include <wbemidl.h>
#include <shlobj.h>
#ifndef WX_PRECOMP
	//(*InternalHeadersPCH(Association)
	//*)
#endif
//(*InternalHeaders(Association)
#include <wx/xrc/xmlres.h>
//*)

//(*IdInit(Association)
//*)

BEGIN_EVENT_TABLE(Association,wxDialog)
	//(*EventTable(Association)
	//*)
END_EVENT_TABLE()

Association::Association(wxWindow* parent)
{
    isOk = false;
	//(*Initialize(Association)
	wxXmlResource::Get()->LoadObject(this,parent,_T("Association"),_T("wxDialog"));
	checkListAssociation = (wxCheckListBox*)FindWindow(XRCID("ID_CHECKLIST"));
	btnOK = (wxButton*)FindWindow(XRCID("ID_OK"));
	btnCancel = (wxButton*)FindWindow(XRCID("ID_CANCEL"));
	checkSelect = (wxCheckBox*)FindWindow(XRCID("ID_CHECKSELECT"));
	Connect(XRCID("ID_OK"),wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&Association::OnbtnOKClick);
	Connect(XRCID("ID_CANCEL"),wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&Association::OnbtnCancelClick);
	Connect(XRCID("ID_CHECKSELECT"), wxEVT_CHECKBOX, (wxObjectEventFunction)&Association::OnbtnCheckClick);
}

Association::~Association()
{
	//(*Destroy(Association)
	//*)
}


bool Association::AssociateExtension()
{
	if (isOk)
	{
		if (!IsAppRunningAsAdminMode())
		{
			wxMessageBox("Please restart in administrator mode", "Informations", wxICON_ERROR);
		}
		else
		{

			int nbCount = checkListAssociation->GetCount();

			AssociateRegards();


			for (int i = 0; i < nbCount; i++)
			{
				if (checkListAssociation->IsChecked(i))
				{
					wxString extension = "." + checkListAssociation->GetString(i);
					AssociateExtension(extension);
				}
			}

			SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_DWORD | SHCNF_FLUSH, nullptr, nullptr);
			Sleep(1000);
		}

	}
	return S_OK;
}

bool Association::IsOk()
{
    return isOk;
}

void Association::OnbtnCheckClick(wxCommandEvent& event)
{
	int nbCount = checkListAssociation->GetCount();
	if (checkSelect->IsChecked())
	{
		for (int i = 0; i < nbCount; i++)
		{
			checkListAssociation->Check(i, true);
		}
	}
	else
	{
		for (int i = 0; i < nbCount; i++)
		{
			checkListAssociation->Check(i, false);
		}
	}
}

void Association::OnbtnOKClick(wxCommandEvent& event)
{
    isOk = true;
    //compressOption = rbCompression->GetSelection();
    this->Close();
}

void Association::OnbtnCancelClick(wxCommandEvent& event)
{
    isOk = false;
    this->Close();
}


int Association::AssociateExtension(LPCWSTR szFileExtension)
{
	CRegistryAccess * m_cRegistryAccess = new CRegistryAccess();
	TCHAR Temp[255];
	StringCchPrintf(Temp, 255, L"%s", szFileExtension);
	m_cRegistryAccess->CreateNewKey(Temp, HKEY_CLASSES_ROOT);
	m_cRegistryAccess->SetKeyValue(HKEY_CLASSES_ROOT, Temp, L"", L"RegardsViewer.Files");
	delete m_cRegistryAccess;

	return 0;
} // AssignRMAsDefaultDotICS

BOOL Association::IsAppRunningAsAdminMode()
{
	BOOL fIsRunAsAdmin = FALSE;
	DWORD dwError = ERROR_SUCCESS;
	PSID pAdministratorsGroup = NULL;

	// Allocate and initialize a SID of the administrators group.
	SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
	if (!AllocateAndInitializeSid(
		&NtAuthority,
		2,
		SECURITY_BUILTIN_DOMAIN_RID,
		DOMAIN_ALIAS_RID_ADMINS,
		0, 0, 0, 0, 0, 0,
		&pAdministratorsGroup))
	{
		dwError = GetLastError();
		goto Cleanup;
	}

	// Determine whether the SID of administrators group is enabled in 
	// the primary access token of the process.
	if (!CheckTokenMembership(NULL, pAdministratorsGroup, &fIsRunAsAdmin))
	{
		dwError = GetLastError();
		goto Cleanup;
	}

Cleanup:
	// Centralized cleanup for all allocated resources.
	if (pAdministratorsGroup)
	{
		FreeSid(pAdministratorsGroup);
		pAdministratorsGroup = NULL;
	}

	// Throw the error if something failed in the function.
	if (ERROR_SUCCESS != dwError)
	{
		throw dwError;
	}

	return fIsRunAsAdmin;
}

int Association::AssociateRegards()
{
	CRegistryAccess * m_cRegistryAccess = new CRegistryAccess();
	//Dim sPath As String
	TCHAR sPath[MAX_PATH];
	TCHAR cDirLong[MAX_PATH];
	//TCHAR cDirShort[MAX_PATH];
	TCHAR Temp[255];

	GetCurrentDirectory(MAX_PATH, cDirLong);
	//GetShortPathName(cDirLong, cDirShort, MAX_PATH);
	//Convertir LongPath en ShortPath
	StringCchPrintfW(cDirLong, MAX_PATH, L"%s\\x64\\Release", cDirLong);
	StringCchPrintfW(sPath, MAX_PATH, L"\"%s\\RegardsViewer.exe\" \"%%1\"", cDirLong);
	
	m_cRegistryAccess->CreateNewKey(L"RegardsViewer.Files", HKEY_CLASSES_ROOT);
	m_cRegistryAccess->CreateNewKey(L"RegardsViewer.Files\\shell\\open\\command", HKEY_CLASSES_ROOT);
	m_cRegistryAccess->CreateNewKey(L"RegardsViewer.Files\\shell\\edit\\command", HKEY_CLASSES_ROOT);
	m_cRegistryAccess->CreateNewKey(L"RegardsViewer.Files\\DefaultIcon", HKEY_CLASSES_ROOT);

	m_cRegistryAccess->SetKeyValue(HKEY_CLASSES_ROOT, L"RegardsViewer.Files", L"", L"RegardsViewer.Files");
	m_cRegistryAccess->SetKeyValue(HKEY_CLASSES_ROOT, L"RegardsViewer.Files\\shell", L"", L"open");
	m_cRegistryAccess->SetKeyValue(HKEY_CLASSES_ROOT, L"RegardsViewer.Files\\shell\\open\\command", L"", sPath);
	m_cRegistryAccess->SetKeyValue(HKEY_CLASSES_ROOT, L"RegardsViewer.Files\\shell\\edit", L"", L"Edit RegardsViewer.Image");
	m_cRegistryAccess->SetKeyValue(HKEY_CLASSES_ROOT, L"RegardsViewer.Files\\shell\\edit\\command", L"", sPath);
	StringCchPrintfW(sPath, MAX_PATH, L"%s\\RegardsViewer.exe,1", cDirLong);
	m_cRegistryAccess->SetKeyValue(HKEY_CLASSES_ROOT, L"RegardsViewer.Files\\DefaultIcon", L"", sPath);
	
	delete m_cRegistryAccess;
	return 0;
}


#endif