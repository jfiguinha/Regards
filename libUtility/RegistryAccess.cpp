#include "header.h"
#ifdef WIN32
#include "registryaccess.h"
#include <strsafe.h>

CRegistryAccess::CRegistryAccess(void)
{
	dwDisposition = 0;
	hKey = nullptr;
}

CRegistryAccess::~CRegistryAccess(void)
{
	if (hKey != nullptr)
		RegCloseKey(hKey);

	hKey = nullptr;
}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

int CRegistryAccess::GetKeyValue(HKEY KeyD, LPCWSTR Key, LPCWSTR SubKey, wchar_t* Value)
{
	HKEY hKeyResult;
	long Result;

	Result = RegOpenKeyEx(KeyD, Key, 0, KEY_ALL_ACCESS, &hKeyResult);

	if (Result != ERROR_SUCCESS)
		return 0;

	Result = RegQueryValueEx(hKeyResult, SubKey, nullptr, (ULONG*)1, (LPBYTE)Value,
	                         (ULONG*)(wcslen(Value) * sizeof(TCHAR)));
	if (Result != ERROR_SUCCESS)
		return 0;

	return 1;
}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

int CRegistryAccess::CreateNewKey(LPTSTR sNewKeyName, HKEY lPredefinedKey)
{
	HKEY hKey;
	DWORD result;
	LSTATUS status;
	status = RegCreateKeyEx(lPredefinedKey, sNewKeyName, 0, nullptr, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, nullptr,
	                        &hKey, &result);
	if (status != ERROR_SUCCESS)
		return 0;

	RegCloseKey(hKey);

	hKey = nullptr;

	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

int CRegistryAccess::SetKeyValue(HKEY KeyD, LPCWSTR Key, LPCWSTR SubKey, LPCWSTR NewValue, DWORD dwType)
{
	HKEY hKey;
	long Result;
	Result = RegOpenKeyEx(KeyD, Key, 0, KEY_ALL_ACCESS, &hKey);

	if (Result != ERROR_SUCCESS)
		return 0;

	Result = RegSetValueEx(hKey, SubKey, 0, dwType, (LPBYTE)NewValue,
	                       static_cast<ULONG>(wcslen(NewValue)) * sizeof(TCHAR));

	if (Result != ERROR_SUCCESS)
		return 0;

	return 1;
}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

long CRegistryAccess::WriteOrCreateKeyValue(LPCWSTR cKeyValue)
{
	return RegCreateKeyEx(HKEY_CURRENT_USER, cKeyValue, 0, nullptr, REG_OPTION_NON_VOLATILE,
	                      KEY_WRITE, nullptr, &hKey, &dwDisposition);
}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

long CRegistryAccess::WriteIntValue(int iValue, LPCWSTR KeyName)
{
	long m_lReturnValue = -1;
	if (hKey != nullptr)
	{
		TCHAR dummy[5];
		StringCchPrintfW(dummy, 5, L"%d", iValue);
		m_lReturnValue = RegSetValueEx(hKey, KeyName, 0, REG_SZ, (LPBYTE)dummy, wcslen(dummy) * sizeof(TCHAR));
	}

	return m_lReturnValue;
}

long CRegistryAccess::WriteDwordValue(DWORD iValue, LPCWSTR KeyName)
{
	long m_lReturnValue = -1;
	if (hKey != nullptr)
	{
		TCHAR dummy[5];
		StringCchPrintfW(dummy, 5, L"%d", iValue);
		m_lReturnValue = RegSetValueEx(hKey, KeyName, 0, REG_DWORD, (LPBYTE)dummy, wcslen(dummy) * sizeof(TCHAR));
	}

	return m_lReturnValue;
}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

long CRegistryAccess::WriteBinValue(LPCWSTR KeyName)
{
	long m_lReturnValue = -1;
	if (hKey != nullptr)
	{
		wchar_t dummy[5];
		StringCchPrintfW(dummy, 5, L"%d", 0);
		m_lReturnValue = RegSetValueEx(hKey, KeyName, 0, REG_NONE, (LPBYTE)dummy, wcslen(dummy) * sizeof(TCHAR));
	}

	return m_lReturnValue;
}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

long CRegistryAccess::WriteStringValue(LPCWSTR cValue, LPCWSTR KeyName)
{
	long m_lReturnValue = -1;
	if (hKey != nullptr)
		m_lReturnValue = RegSetValueEx(hKey, KeyName, 0, REG_SZ, (LPBYTE)cValue, wcslen(cValue) * sizeof(TCHAR));

	return m_lReturnValue;
}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

long CRegistryAccess::OpenKeyValue(LPCWSTR cKeyValue)
{
	return RegOpenKeyEx(HKEY_CURRENT_USER, cKeyValue, 0, KEY_READ, &hKey);
}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

int CRegistryAccess::ReadStringValue(wchar_t* cValue, int iSize, LPCWSTR KeyName)
{
	DWORD groesse = 255 * sizeof(TCHAR);
	DWORD art = REG_SZ;
	TCHAR puffer[255];
	if (hKey != nullptr)
	{
		LRESULT lResult = RegQueryValueEx(hKey, KeyName, nullptr, &art, (LPBYTE)puffer, &groesse);
		if (lResult == ERROR_SUCCESS)
		{
			StringCchCopyW(cValue, iSize, puffer);
			return 0;
		}
	}
	return -1;
}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

int CRegistryAccess::ReadIntValue(LPCWSTR KeyName)
{
	DWORD groesse = 255 * sizeof(TCHAR);
	DWORD art = REG_SZ;
	TCHAR puffer[255];

	if (hKey != nullptr)
	{
		//::MessageBox(NULL,KeyName,"infos",0);
		LRESULT lResult = RegQueryValueEx(hKey, KeyName, nullptr, &art, (LPBYTE)puffer, &groesse);

		if (lResult == ERROR_SUCCESS)
			return _wtoi(puffer);
	}

	return -1;
}
#endif
