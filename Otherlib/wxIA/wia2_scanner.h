#pragma once
#include <Windows.h>
#include <Wia.h>
#include <Sti.h>


class CWia2Scanner
{
public:
	CWia2Scanner() {};
	~CWia2Scanner() {};
	HRESULT EnumerateWiaDevices(IWiaDevMgr2 *pWiaDevMgr);
	HRESULT CreateWiaDeviceManager(IWiaDevMgr2 **ppWiaDevMgr);
	HRESULT ReadSomeWiaProperties(IWiaPropertyStorage *pWiaPropertyStorage);
	HRESULT CreateWiaDevice(IWiaDevMgr2 *pWiaDevMgr, BSTR bstrDeviceID, IWiaItem2 **ppWiaDevice);
	HRESULT EnumerateItems(IWiaItem2 *pWiaItem);
};