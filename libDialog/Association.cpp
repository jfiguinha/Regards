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
	m_cRegistryAccess->SetKeyValue(HKEY_CLASSES_ROOT, Temp, L"", L"RegardsViewer.1");
	/*
	m_cRegistryAccess->SetKeyValue(HKEY_LOCAL_MACHINE, L"SOFTWARE\\RegardsViewer\\Capabilities\\FileAssociations", szFileExtension, L"RegardsViewer.1");
	TCHAR Temp[255];

	StringCchPrintf(Temp, 255, L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\%s", szFileExtension);
	m_cRegistryAccess->CreateNewKey(Temp, HKEY_CURRENT_USER);

	StringCchPrintf(Temp, 255, L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\%s\\UserChoice", szFileExtension);
	m_cRegistryAccess->CreateNewKey(Temp, HKEY_CURRENT_USER);
	m_cRegistryAccess->SetKeyValue(HKEY_CURRENT_USER, Temp, L"Progid", L"RegardsViewer.1");

	StringCchPrintf(Temp, 255, L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\%s\\OpenWithProgids", szFileExtension);
	m_cRegistryAccess->CreateNewKey(Temp, HKEY_CURRENT_USER);
	m_cRegistryAccess->WriteBinValue(L"RegardsViewer.1");


	StringCchPrintf(Temp, 255, L"%s", szFileExtension);
	m_cRegistryAccess->CreateNewKey(Temp, HKEY_CLASSES_ROOT);
	StringCchPrintf(Temp, 255, L"%s\\OpenWithProgids", szFileExtension);
	m_cRegistryAccess->SetKeyValue(HKEY_CLASSES_ROOT, Temp, L"RegardsViewer.1", L"");


	m_cRegistryAccess->SetKeyValue(HKEY_CLASSES_ROOT, L"RegardsViewer.exe\\SupportedTypes", szFileExtension, L"");

	TCHAR sPath[MAX_PATH];
	TCHAR cDirLong[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, cDirLong);
	StringCchPrintfW(sPath, MAX_PATH, L"%s\\RegardsViewer.exe %%1", cDirLong);
	StringCchPrintf(Temp, 255, L"SystemFileAssociations\\%s\\shell\\edit\\command", szFileExtension);
	m_cRegistryAccess->SetKeyValue(HKEY_CLASSES_ROOT, Temp, L"", sPath);
	StringCchPrintf(Temp, 255, L"SystemFileAssociations\\%s\\shell\\open\\command", szFileExtension);
	m_cRegistryAccess->SetKeyValue(HKEY_CLASSES_ROOT, Temp, L"", sPath);

	TCHAR sPath[MAX_PATH];
	TCHAR cDirLong[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, cDirLong);
	StringCchPrintfW(sPath, MAX_PATH, L"%s\\RegardsViewer.exe %%1", cDirLong);
	StringCchPrintf(Temp, 255, L"%s\\shell\\edit\\command", szFileExtension);
	m_cRegistryAccess->CreateNewKey(Temp, HKEY_CLASSES_ROOT);
	m_cRegistryAccess->SetKeyValue(HKEY_CLASSES_ROOT, Temp, L"", sPath);
	StringCchPrintf(Temp, 255, L"%s\\shell\\open\\command", szFileExtension);
	m_cRegistryAccess->CreateNewKey(Temp, HKEY_CLASSES_ROOT);
	m_cRegistryAccess->SetKeyValue(HKEY_CLASSES_ROOT, Temp, L"", sPath);
	

	//Ordinateur\HKEY_CURRENT_USER\SOFTWARE\Microsoft\Windows\CurrentVersion\ApplicationAssociationToasts Applications\RegardsViewer.exe_.JPG DWORD
	//Ordinateur\HKEY_CURRENT_USER\SOFTWARE\Microsoft\Windows\CurrentVersion\Explorer\FileExts\.jpg\OpenWithList b
	//Ordinateur\HKEY_CURRENT_USER\SOFTWARE\Microsoft\Windows\CurrentVersion\Explorer\FileExts\.jpg\OpenWithProgids //Regards.Document
	//Ordinateur\HKEY_CURRENT_USER\SOFTWARE\Microsoft\Windows\CurrentVersion\Explorer\FileExts\.jpg\UserChoice ProgId

	/*
	TCHAR Temp[255];
	StringCchPrintf(Temp, 255, L"Applications\\RegardsViewer.exe_%s", szFileExtension);
	m_cRegistryAccess->SetKeyValue(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\ApplicationAssociationToasts", Temp, L"0", REG_DWORD);
	StringCchPrintf(Temp, 255, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\%s\\OpenWithList", szFileExtension);
	m_cRegistryAccess->CreateNewKey(Temp, HKEY_CURRENT_USER);
	m_cRegistryAccess->SetKeyValue(HKEY_CURRENT_USER, Temp, L"b", L"RegardsViewer.exe");
	StringCchPrintf(Temp, 255, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\%s\\OpenWithProgids", szFileExtension);
	m_cRegistryAccess->CreateNewKey(Temp, HKEY_CURRENT_USER);
	m_cRegistryAccess->WriteBinValue(L"Regards.Document");
	StringCchPrintf(Temp, 255, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\%s\\UserChoice", szFileExtension);
	m_cRegistryAccess->SetKeyValue(HKEY_CURRENT_USER, Temp, L"ProgId", L"RegardsViewer.exe");
	*/
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

/*
void Association::AssociateApplication(HKEY classesDefined)
{
	TCHAR sPath[MAX_PATH];
	//TCHAR cDirShort[MAX_PATH];
	TCHAR cDirLong[MAX_PATH];

	GetCurrentDirectory(MAX_PATH, cDirLong);
	//GetShortPathName(cDirLong, cDirShort, MAX_PATH);
	StringCchPrintfW(sPath, MAX_PATH, L"%s\\RegardsViewer.exe %%1", cDirLong);

	CRegistryAccess * m_cRegistryAccess = new CRegistryAccess();

	m_cRegistryAccess->CreateNewKey(L"Applications\\RegardsViewer.exe", classesDefined);
	m_cRegistryAccess->CreateNewKey(L"Applications\\RegardsViewer.exe\\shell\\open\\command", classesDefined);
	m_cRegistryAccess->CreateNewKey(L"Applications\\RegardsViewer.exe\\DefaultIcon", classesDefined);

	m_cRegistryAccess->SetKeyValue(classesDefined, L"Applications\\RegardsViewer.exe", L"", L"RegardsViewer Image");

	StringCchPrintf(sPath, MAX_PATH, L"%s\\RegardsViewer.exe %%1", cDirLong);
	m_cRegistryAccess->SetKeyValue(classesDefined, L"Applications\\RegardsViewer.exe\\shell\\open\\command", L"", sPath);

	//StringCchPrintf(sPath,"%s\\regards.exe,1",cDir);

	StringCchPrintf(sPath, MAX_PATH, L"%s\\RegardsViewer.exe,1", cDirLong);
	m_cRegistryAccess->SetKeyValue(classesDefined, L"Applications\\RegardsViewer.exe\\DefaultIcon", L"", sPath);

	delete m_cRegistryAccess;
}

void RegSet(HKEY hkeyHive, const TCHAR * pszVar, const TCHAR * pszValue) {

	HKEY hkey;

	char szValueCurrent[1000];
	DWORD dwType;
	DWORD dwSize = sizeof(szValueCurrent);

	int iRC = RegGetValue(hkeyHive, pszVar, NULL, RRF_RT_ANY, &dwType, szValueCurrent, &dwSize);

	bool bDidntExist = iRC == ERROR_FILE_NOT_FOUND;

	if (iRC != ERROR_SUCCESS && !bDidntExist)
		AKS(AKSFatal, "RegGetValue( %s ): %s", pszVar, strerror(iRC));

	if (!bDidntExist) {
		if (dwType != REG_SZ)
			AKS(AKSFatal, "RegGetValue( %s ) found type unhandled %d", pszVar, dwType);

		if (strcmp(szValueCurrent, pszValue) == 0) {
			AKS(AKSTrace, "RegSet( \"%s\" \"%s\" ): already correct", pszVar, pszValue);
			return;
		}
	}

	DWORD dwDisposition;
	iRC = RegCreateKeyEx(hkeyHive, pszVar, 0, 0, 0, KEY_ALL_ACCESS, NULL, &hkey, &dwDisposition);
	if (iRC != ERROR_SUCCESS)
		AKS(AKSFatal, "RegCreateKeyEx( %s ): %s", pszVar, strerror(iRC));

	iRC = RegSetValueEx(hkey, "", 0, REG_SZ, (BYTE*)pszValue, strlen(pszValue) + 1);
	if (iRC != ERROR_SUCCESS)
		AKS(AKSFatal, "RegSetValueEx( %s ): %s", pszVar, strerror(iRC));

	if (bDidntExist)
		AKS(AKSTrace, "RegSet( %s ): set to \"%s\"", pszVar, pszValue);
	else
		AKS(AKSTrace, "RegSet( %s ): changed \"%s\" to \"%s\"", pszVar, szValueCurrent, pszValue);

	RegCloseKey(hkey);
}



int SetUpRegistry() {

	//app doesn't have permission for this when run as normal user, but may for Admin?  Anyway, not needed.
	//RegSet( HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\MoselleIDE.exe", "C:\\Moselle\\bin\\MoselleIDE.exe" );

	RegSet(HKEY_CURRENT_USER, "Software\\Classes\\.moselle", "Moselle.MoselleIDE.1");

	// Not needed.
	RegSet(HKEY_CURRENT_USER, "Software\\Classes\\.moselle\\Content Type", "text/plain");
	RegSet(HKEY_CURRENT_USER, "Software\\Classes\\.moselle\\PerceivedType", "text");

	//Not needed, but may be be a way to have wordpad show up on the default list.
	//RegSet( HKEY_CURRENT_USER, "Software\\Classes\\.moselle\\OpenWithProgIds\\Moselle.MoselleIDE.1", "" );

	RegSet(HKEY_CURRENT_USER, "Software\\Classes\\Moselle.MoselleIDE.1", "Moselle IDE");

	RegSet(HKEY_CURRENT_USER, "Software\\Classes\\Moselle.MoselleIDE.1\\Shell\\Open\\Command", "C:\\Moselle\\bin\\MoselleIDE.exe %1");

	SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, NULL, NULL);
	return 0;
}
*/

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
	StringCchPrintfW(sPath, MAX_PATH, L"%s\\RegardsViewer.exe %%1", cDirLong);
	

	//Create the 'MyApp.Document' item under
	//HKEY_CLASSES_ROOT. This is where you'll put
	//the command line to execute or other shell
	//statements necessary.
	
	//Ordinateur\HKEY_CURRENT_USER\SOFTWARE\Classes\Applications\RegardsViewer.exe\shell\open\command
	   /*
	m_cRegistryAccess->SetKeyValue(HKEY_CURRENT_USER, L"SOFTWARE\\Classes\\Applications\\RegardsViewer.exe", L"", L"RegardsViewer Image");
	m_cRegistryAccess->CreateNewKey(L"SOFTWARE\\Classes\\Applications\\RegardsViewer.exe\\shell\\open\\command", HKEY_CURRENT_USER);
	m_cRegistryAccess->SetKeyValue(HKEY_CURRENT_USER, L"SOFTWARE\\Classes\\Applications\\RegardsViewer.exe\\shell\\open\\command", L"", sPath);
	m_cRegistryAccess->CreateNewKey(L"SOFTWARE\\Classes\\Applications\\RegardsViewer.exe\\DefaultIcon", HKEY_CURRENT_USER);
	StringCchPrintfW(sPath, MAX_PATH, L"%s\\x64\\Release\\RegardsViewer.exe,1", cDirLong);
	m_cRegistryAccess->SetKeyValue(HKEY_CURRENT_USER, L"SOFTWARE\\Classes\\Applications\\RegardsViewer.exe\\DefaultIcon", L"", sPath);
	*/
	/*
	HKEY_LOCAL_MACHINE
	   SOFTWARE
		  Contoso
			 WebBrowser
				Capabilities
				   ApplicationDescription = This award-winning Contoso browser is better than ever. Search the Internet and find exactly what you want in just seconds. Use integrated tabs and new phishing detectors to enhance your Internet experience.
				   FileAssociations
					  .htm = ContosoHTML
					  .html = ContosoHTML
					  .shtml = ContosoHTML
					  .xht = ContosoHTML
					  .xhtml = ContosoHTML
				   Startmenu
					  StartmenuInternet = Contoso.exe
				   UrlAssociations
					  http = Contoso.Url.Http
					  https = Contoso.Url.Https
					  ftp = Contoso.Url.ftp
	   SOFTWARE
		  RegisteredApplications
			 Contoso.WebBrowser.1.06 = SOFTWARE\Contoso\WebBrowser\Capabilities



	
	m_cRegistryAccess->CreateNewKey(L"SOFTWARE\\RegardsViewer", HKEY_LOCAL_MACHINE);
	m_cRegistryAccess->CreateNewKey(L"SOFTWARE\\RegardsViewer\\Capabilities", HKEY_LOCAL_MACHINE);
	m_cRegistryAccess->SetKeyValue(HKEY_LOCAL_MACHINE, L"SOFTWARE\\RegardsViewer\\Capabilities", L"ApplicationDescription", L"Regards is a modern photo viewer");
	m_cRegistryAccess->SetKeyValue(HKEY_LOCAL_MACHINE, L"SOFTWARE\\RegardsViewer\\Capabilities", L"ApplicationName", L"RegardsViewer 2.0");
	m_cRegistryAccess->CreateNewKey(L"SOFTWARE\\RegardsViewer\\Capabilities\\FileAssociations", HKEY_LOCAL_MACHINE);
	m_cRegistryAccess->CreateNewKey(L"SOFTWARE\\RegisteredApplications", HKEY_LOCAL_MACHINE);
	m_cRegistryAccess->SetKeyValue(HKEY_LOCAL_MACHINE, L"SOFTWARE\\RegisteredApplications", L"RegardsViewer.2", L"Software\\RegardsViewer\\Capabilities");


	m_cRegistryAccess->CreateNewKey(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths", HKEY_LOCAL_MACHINE);
	m_cRegistryAccess->CreateNewKey(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\RegardsViewer.exe", HKEY_LOCAL_MACHINE);
	m_cRegistryAccess->SetKeyValue(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\RegardsViewer.exe", L"", sPath);
	m_cRegistryAccess->SetKeyValue(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\RegardsViewer.exe", L"Path", cDirLong);

	//RegSet(HKEY_CURRENT_USER, "Software\\Classes\\.moselle", "Moselle.MoselleIDE.1");

	m_cRegistryAccess->CreateNewKey(L"Software\\Classes\\RegardsViewer.1", HKEY_CURRENT_USER);
	m_cRegistryAccess->SetKeyValue(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\RegardsViewer.exe", L"RegardsViewer", L"");


	//---------------------------------------------------------------------------------------
	//Registration on the CLASSES ROOT
	//---------------------------------------------------------------------------------------

	m_cRegistryAccess->CreateNewKey(L"RegardsViewer.1", HKEY_CLASSES_ROOT);
	m_cRegistryAccess->CreateNewKey(L"RegardsViewer.1\\shell\\open\\command", HKEY_CLASSES_ROOT);
	m_cRegistryAccess->CreateNewKey(L"RegardsViewer.1\\DefaultIcon", HKEY_CLASSES_ROOT);

	m_cRegistryAccess->SetKeyValue(HKEY_CLASSES_ROOT, L"RegardsViewer.1", L"", L"RegardsViewer Image");
	m_cRegistryAccess->SetKeyValue(HKEY_CLASSES_ROOT, L"RegardsViewer.1\\shell\\open\\command", L"", sPath);
	m_cRegistryAccess->SetKeyValue(HKEY_CLASSES_ROOT, L"RegardsViewer.1\\shell\\edit\\command", L"", sPath);
	m_cRegistryAccess->SetKeyValue(HKEY_CLASSES_ROOT, L"RegardsViewer.1\\DefaultIcon", L"", sPath);
	
	m_cRegistryAccess->CreateNewKey(L"Applications\\RegardsViewer.exe", HKEY_CLASSES_ROOT);
	m_cRegistryAccess->CreateNewKey(L"Applications\\RegardsViewer.exe\\shell\\open\\command", HKEY_CLASSES_ROOT);
	m_cRegistryAccess->CreateNewKey(L"Applications\\RegardsViewer.exe\\DefaultIcon", HKEY_CLASSES_ROOT);

	m_cRegistryAccess->SetKeyValue(HKEY_CLASSES_ROOT, L"Applications\\RegardsViewer.exe", L"", L"RegardsViewer Image");
	m_cRegistryAccess->SetKeyValue(HKEY_CLASSES_ROOT, L"Applications\\RegardsViewer.exe\\shell\\open\\command", L"", sPath);
	m_cRegistryAccess->SetKeyValue(HKEY_CLASSES_ROOT, L"Applications\\RegardsViewer.exe\\shell\\edit\\command", L"", sPath);
	m_cRegistryAccess->SetKeyValue(HKEY_CLASSES_ROOT, L"Applications\\RegardsViewer.exe\\DefaultIcon", L"", sPath);

	//---------------------------------------------------------------------------------------
	//Registration on the CURRENT USER
	//---------------------------------------------------------------------------------------

	m_cRegistryAccess->CreateNewKey(L"Applications\\RegardsViewer.exe", HKEY_CURRENT_USER);
	m_cRegistryAccess->CreateNewKey(L"Applications\\RegardsViewer.exe\\shell\\open\\command", HKEY_CURRENT_USER);
	m_cRegistryAccess->CreateNewKey(L"Applications\\RegardsViewer.exe\\DefaultIcon", HKEY_CURRENT_USER);

	m_cRegistryAccess->SetKeyValue(HKEY_CURRENT_USER, L"Applications\\RegardsViewer.exe", L"", L"RegardsViewer Image");
	m_cRegistryAccess->SetKeyValue(HKEY_CURRENT_USER, L"Applications\\RegardsViewer.exe\\shell\\open\\command", L"", sPath);
	m_cRegistryAccess->SetKeyValue(HKEY_CURRENT_USER, L"Applications\\RegardsViewer.exe\\DefaultIcon", L"", sPath);

	//---------------------------------------------------------------------------------------
	//Registration on the HKEY_LOCAL_MACHINE
	//---------------------------------------------------------------------------------------

	m_cRegistryAccess->CreateNewKey(L"SOFTWARE\\Classes\\RegardsViewer.1", HKEY_LOCAL_MACHINE);
	m_cRegistryAccess->CreateNewKey(L"SOFTWARE\\Classes\\RegardsViewer.1\\shell\\open\\command", HKEY_LOCAL_MACHINE);
	m_cRegistryAccess->CreateNewKey(L"SOFTWARE\\Classes\\RegardsViewer.1\\DefaultIcon", HKEY_LOCAL_MACHINE);

	m_cRegistryAccess->SetKeyValue(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Classes\\RegardsViewer.1", L"", L"RegardsViewer Image");
	m_cRegistryAccess->SetKeyValue(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Classes\\RegardsViewer.1\\shell\\open\\command", L"", sPath);
	m_cRegistryAccess->SetKeyValue(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Classes\\RegardsViewer.1\\DefaultIcon", L"", sPath);
	
	m_cRegistryAccess->CreateNewKey(L"SOFTWARE\\Classes\\Applications\\RegardsViewer.exe", HKEY_LOCAL_MACHINE);
	m_cRegistryAccess->CreateNewKey(L"SOFTWARE\\Classes\\Applications\\RegardsViewer.exe\\shell\\open\\command", HKEY_LOCAL_MACHINE);
	m_cRegistryAccess->CreateNewKey(L"SOFTWARE\\Classes\\Applications\\RegardsViewer.exe\\DefaultIcon", HKEY_LOCAL_MACHINE);

	m_cRegistryAccess->SetKeyValue(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Classes\\Applications\\RegardsViewer.exe", L"", L"RegardsViewer Image");
	m_cRegistryAccess->SetKeyValue(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Classes\\Applications\\RegardsViewer.exe\\shell\\open\\command", L"", sPath);
	m_cRegistryAccess->SetKeyValue(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Classes\\Applications\\RegardsViewer.exe\\DefaultIcon", L"", sPath);
		*/

	m_cRegistryAccess->CreateNewKey(L"RegardsViewer.1", HKEY_CLASSES_ROOT);
	m_cRegistryAccess->CreateNewKey(L"RegardsViewer.1\\shell\\open\\command", HKEY_CLASSES_ROOT);
	m_cRegistryAccess->CreateNewKey(L"RegardsViewer.1\\shell\\edit\\command", HKEY_CLASSES_ROOT);
	m_cRegistryAccess->CreateNewKey(L"RegardsViewer.1\\DefaultIcon", HKEY_CLASSES_ROOT);

	m_cRegistryAccess->SetKeyValue(HKEY_CLASSES_ROOT, L"RegardsViewer.1", L"", L"RegardsViewer Image");
	m_cRegistryAccess->SetKeyValue(HKEY_CLASSES_ROOT, L"RegardsViewer.1\\shell\\open\\command", L"", sPath);
	m_cRegistryAccess->SetKeyValue(HKEY_CLASSES_ROOT, L"RegardsViewer.1\\shell\\edit\\command", L"", sPath);
	StringCchPrintfW(sPath, MAX_PATH, L"%s\\RegardsViewer.exe,1", cDirLong);
	m_cRegistryAccess->SetKeyValue(HKEY_CLASSES_ROOT, L"RegardsViewer.1\\DefaultIcon", L"", sPath);


#ifdef OLD_STYLE

	m_cRegistryAccess->CreateNewKey(L"Regards.Document", HKEY_CLASSES_ROOT);
	m_cRegistryAccess->CreateNewKey(L"Regards.Document\\shell\\open\\command", HKEY_CLASSES_ROOT);
	m_cRegistryAccess->CreateNewKey(L"Regards.Document\\DefaultIcon", HKEY_CLASSES_ROOT);


	m_cRegistryAccess->SetKeyValue(HKEY_CLASSES_ROOT, L"Regards.Document", L"", L"Regards Document");

	GetCurrentDirectory(MAX_PATH, cDirLong);

	GetShortPathName(cDirLong, cDirShort, MAX_PATH);

	//Convertir LongPath en ShortPath

	StringCchPrintfW(sPath, MAX_PATH, L"%s\\RegardsViewer.exe %%1", cDirShort);
	m_cRegistryAccess->SetKeyValue(HKEY_CLASSES_ROOT, L"Regards.Document\\shell\\open\\command", L"", sPath);

	StringCchPrintfW(sPath, MAX_PATH, L"%s\\RegardsViewer.exe,1", cDirShort);

	m_cRegistryAccess->SetKeyValue(HKEY_CLASSES_ROOT, L"Regards.Document\\DefaultIcon", L"", sPath);



	m_cRegistryAccess->CreateNewKey(L"Applications\\RegardsViewer.exe", HKEY_CLASSES_ROOT);
	m_cRegistryAccess->CreateNewKey(L"Applications\\RegardsViewer.exe\\shell\\open\\command", HKEY_CLASSES_ROOT);
	m_cRegistryAccess->CreateNewKey(L"Applications\\RegardsViewer.exe\\DefaultIcon", HKEY_CLASSES_ROOT);

	m_cRegistryAccess->SetKeyValue(HKEY_CLASSES_ROOT, L"Applications\\RegardsViewer.exe", L"", L"Regards Document");

	StringCchPrintfW(sPath, MAX_PATH, L"%s\\RegardsViewer.exe %%1", cDirShort);
	m_cRegistryAccess->SetKeyValue(HKEY_CLASSES_ROOT, L"Applications\\RegardsViewer.exe\\shell\\open\\command", L"", sPath);

	//StringCchPrintfW(sPath,"%s\\RegardsViewer.exe,1",cDir);

	StringCchPrintfW(sPath, MAX_PATH, L"%s\\RegardsViewer.exe,1", cDirShort);
	m_cRegistryAccess->SetKeyValue(HKEY_CLASSES_ROOT, L"Applications\\RegardsViewer.exe\\DefaultIcon", L"", sPath);

#endif

	delete m_cRegistryAccess;
	return 0;
}


#endif