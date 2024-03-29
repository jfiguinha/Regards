#pragma once
#include <Windows.h>
#include <Wia.h>
#include <Sti.h>
#include "comptr.h"


using PFNPROGRESSCALLBACK = HRESULT(CALLBACK *)(
	LONG lStatus,
	LONG lPercentComplete,
	PVOID pParam
);

//DataClass

class DataClasses : public IWiaEventCallback
{
public:
	DataClasses();

	LONG getNumDevices();


	STDMETHOD(QueryInterface)(REFIID iid, LPVOID* ppvObj) override;
	STDMETHOD_(ULONG, AddRef)() override;
	STDMETHOD_(ULONG, Release)() override;

	// IWiaEventCallback interface

	STDMETHOD(ImageEventCallback)(
		LPCGUID pEventGuid,
		BSTR bstrEventDescription,
		BSTR bstrDeviceID,
		BSTR bstrDeviceDescription,
		DWORD dwDeviceType,
		BSTR bstrFullItemName,
		ULONG* pulEventType,
		ULONG ulReserved
	) override;


	HRESULT Register();

	ULONG GetNumDevices() const;

private:
	LONG m_cRef;
	ULONG m_nNumDevices;
	ComPtr<IUnknown> m_pConnectEventObject;
	ComPtr<IUnknown> m_pDisconnectEventObject;
	HRESULT WiaGetNumDevices(
		IWiaDevMgr* pSuppliedWiaDevMgr,
		ULONG* pulNumDevices
	);
	ComPtr<IWiaDevMgr> pWiaDevMgr;
};


//CDataCallback class


class CDataCallback : public IWiaDataCallback
{
public:
	CDataCallback(
		PFNPROGRESSCALLBACK pfnProgressCallback,
		PVOID pProgressCallbackParam,
		LONG* plCount,
		IStream*** pppStream
	);

	// IUnknown interface

	STDMETHOD(QueryInterface)(REFIID iid, LPVOID* ppvObj) override;
	STDMETHOD_(ULONG, AddRef)() override;
	STDMETHOD_(ULONG, Release)() override;

	// IWiaDataCallback interface

	STDMETHOD(BandedDataCallback)(
		LONG lReason,
		LONG lStatus,
		LONG lPercentComplete,
		LONG lOffset,
		LONG lLength,
		LONG lReserved,
		LONG lResLength,
		PBYTE pbBuffer
	) override;

	// CDataCallback methods

private:
	HRESULT ReAllocBuffer(ULONG nBufferSize);
	HRESULT CopyToBuffer(ULONG nOffset, LPCVOID pBuffer, ULONG nSize);
	HRESULT StoreBuffer();

private:
	LONG m_cRef;

	BOOL m_bBMP;
	LONG m_nHeaderSize;
	LONG m_nDataSize;
	ComPtr<IStream> m_pStream;

	PFNPROGRESSCALLBACK m_pfnProgressCallback;
	PVOID m_pProgressCallbackParam;

	LONG* m_plCount;
	IStream*** m_pppStream;
};
