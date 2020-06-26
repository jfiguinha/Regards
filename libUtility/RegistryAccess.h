#pragma once
#ifdef WIN32

class CRegistryAccess
{
public:
	CRegistryAccess(void);
	~CRegistryAccess(void);

	int GetKeyValue(HKEY KeyD, LPCWSTR Key, LPCWSTR SubKey, wchar_t * Value);
	int SetKeyValue(HKEY KeyD, LPCWSTR Key, LPCWSTR SubKey, LPCWSTR NewValue, DWORD dwType = 1);
	int CreateNewKey(LPTSTR sNewKeyName, HKEY lPredefinedKey);

	//Write
	long WriteOrCreateKeyValue(LPCWSTR cKeyValue);
	long WriteStringValue(LPCWSTR cValue, LPCWSTR KeyName);
	long WriteIntValue(int iValue, LPCWSTR KeyName);
	long WriteDwordValue(DWORD iValue, LPCWSTR KeyName);

	//Read
	long OpenKeyValue(LPCWSTR cKeyValue);
	int ReadStringValue(wchar_t * cValue,int iSize, LPCWSTR KeyName);
	int ReadIntValue(LPCWSTR KeyName);

	long WriteBinValue(LPCWSTR KeyName);

private:

	HKEY  hKey; 
	DWORD  dwDisposition; 

};

#endif