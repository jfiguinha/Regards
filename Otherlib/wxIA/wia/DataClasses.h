#pragma once
#pragma once
#include <Windows.h>
#include <Wia.h>
#include <Sti.h>
#include <atlbase.h>


typedef HRESULT(CALLBACK *PFNPROGRESSCALLBACK)(
	LONG   lStatus,
	LONG   lPercentComplete,
	PVOID  pParam
	);

//DataClass

class DataClasses :public IWiaEventCallback
{
public:
	DataClasses();

	LONG getNumDevices();


	STDMETHOD(QueryInterface)(REFIID iid, LPVOID *ppvObj);
	STDMETHOD_(ULONG, AddRef)();
	STDMETHOD_(ULONG, Release)();

	// IWiaEventCallback interface

	STDMETHOD(ImageEventCallback)(
		LPCGUID pEventGuid,
		BSTR    bstrEventDescription,
		BSTR    bstrDeviceID,
		BSTR    bstrDeviceDescription,
		DWORD   dwDeviceType,
		BSTR    bstrFullItemName,
		ULONG  *pulEventType,
		ULONG   ulReserved
		);



	HRESULT Register();

	ULONG GetNumDevices() const;

private:
	LONG               m_cRef;
	ULONG              m_nNumDevices;
	CComPtr<IUnknown>  m_pConnectEventObject;
	CComPtr<IUnknown>  m_pDisconnectEventObject;
	HRESULT WiaGetNumDevices(
		IWiaDevMgr *pSuppliedWiaDevMgr,
		ULONG      *pulNumDevices
	);
	CComPtr<IWiaDevMgr> pWiaDevMgr;
};


//CDataCallback class


class CDataCallback : public IWiaDataCallback
{
public:
	CDataCallback(
		PFNPROGRESSCALLBACK  pfnProgressCallback,
		PVOID                pProgressCallbackParam,
		LONG                *plCount,
		IStream             ***pppStream
	);

	// IUnknown interface

	STDMETHOD(QueryInterface)(REFIID iid, LPVOID *ppvObj);
	STDMETHOD_(ULONG, AddRef)();
	STDMETHOD_(ULONG, Release)();

	// IWiaDataCallback interface

	STDMETHOD(BandedDataCallback) (
		LONG  lReason,
		LONG  lStatus,
		LONG  lPercentComplete,
		LONG  lOffset,
		LONG  lLength,
		LONG  lReserved,
		LONG  lResLength,
		PBYTE pbBuffer
		);

	// CDataCallback methods

private:
	HRESULT ReAllocBuffer(ULONG nBufferSize);
	HRESULT CopyToBuffer(ULONG nOffset, LPCVOID pBuffer, ULONG nSize);
	HRESULT StoreBuffer();




private:
	LONG m_cRef;

	BOOL              m_bBMP;
	LONG              m_nHeaderSize;
	LONG              m_nDataSize;
	CComPtr<IStream>  m_pStream;

	PFNPROGRESSCALLBACK  m_pfnProgressCallback;
	PVOID                m_pProgressCallbackParam;

	LONG    *m_plCount;
	IStream ***m_pppStream;
};