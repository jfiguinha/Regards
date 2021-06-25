#pragma once

#include <Windows.h>
#include <Wia.h>
#include <sti.h>
#include <commctrl.h>
#include <shellapi.h>
#include <objbase.h>
#include "DataClasses.h"


class CProgressDlg : public IUnknown
{
public:
	CProgressDlg(HWND hWndParent);
	~CProgressDlg();

	// IUnknown interface

	STDMETHOD(QueryInterface)(REFIID iid, LPVOID *ppvObj);
	STDMETHOD_(ULONG, AddRef)();
	STDMETHOD_(ULONG, Release)();

	// CProgressDlg methods
	BOOL Cancelled() const;
	VOID SetTitle(PCTSTR pszTitle);
	VOID SetMessage(PCTSTR pszMessage);
	VOID SetPercent(UINT nPercent);
	HWND GetDlgWindow();

private:
	static DWORD WINAPI ThreadProc(PVOID pParameter);
	static INT_PTR CALLBACK DialogProc(HWND, UINT, WPARAM, LPARAM);

private:
	LONG    m_cRef;
	HWND    m_hDlg;
	HWND    m_hWndParent;
	LONG    m_bCancelled;
	HANDLE  m_hInitDlg;
	//HINSTANCE g_hInstance;

};

HRESULT
CALLBACK
DefaultProgressCallback(
	LONG   lStatus,
	LONG   lPercentComplete,
	PVOID  pParam
);
