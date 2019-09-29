#include <header.h>
#ifdef __WXMSW__
#include "ProgressDlg.h"
#include <resource.h>

//////////////////////////////////////////////////////////////////////////
//
// CProgressDlg::CProgressDlg
//



CProgressDlg::CProgressDlg(HWND hWndParent)
{
	m_cRef = 0;

	m_hDlg = NULL;
	m_hWndParent = hWndParent;
	m_bCancelled = FALSE;

	m_hInitDlg = CreateEvent(NULL, FALSE, FALSE, NULL);

	if (m_hInitDlg != NULL)
	{
		// Create the progress dialog in a separate thread so that it
		// remains responsive

		DWORD dwThreadId;

		HANDLE hThread = CreateThread(
			NULL,
			0,
			ThreadProc,
			this,
			0,
			&dwThreadId
		);

		// Wait until the progress dialog is created and ready to process
		// messages. During creation, the new thread will send window messages 
		// to this thread, and this will cause a deadlock if we use 
		// WaitForSingleObject instead of MsgWaitForMultipleObjects

		if (hThread != NULL)
		{
			while (MsgWaitForMultipleObjects(1, &m_hInitDlg, FALSE, INFINITE, QS_ALLINPUT | QS_ALLPOSTMESSAGE) == WAIT_OBJECT_0 + 1)
			{
				MSG msg;

				while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}

			CloseHandle(hThread);
		}

		CloseHandle(m_hInitDlg);
	}
}

//////////////////////////////////////////////////////////////////////////
//
// ~CProgressDlg::CProgressDlg
//

CProgressDlg::~CProgressDlg()
{
	EndDialog(m_hDlg, IDCLOSE);
}

//////////////////////////////////////////////////////////////////////////
//
// CProgressDlg::QueryInterface
//

STDMETHODIMP CProgressDlg::QueryInterface(REFIID iid, LPVOID *ppvObj)
{
	if (ppvObj == NULL)
	{
		return E_POINTER;
	}

	if (iid == IID_IUnknown)
	{
		*ppvObj = (IUnknown *)this;
	}
	else
	{
		*ppvObj = NULL;
		return E_NOINTERFACE;
	}

	AddRef();
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
//
// CProgressDlg::AddRef
//

STDMETHODIMP_(ULONG) CProgressDlg::AddRef()
{
	return InterlockedIncrement(&m_cRef);
}

//////////////////////////////////////////////////////////////////////////
//
// CProgressDlg::Release
//

STDMETHODIMP_(ULONG) CProgressDlg::Release()
{
	LONG cRef = InterlockedDecrement(&m_cRef);

	if (cRef == 0)
	{
		delete this;
	}

	return cRef;
}

//////////////////////////////////////////////////////////////////////////
//
// CProgressDlg::Cancelled
//

BOOL CProgressDlg::Cancelled() const
{
	return m_bCancelled;
}

//////////////////////////////////////////////////////////////////////////
//
// CProgressDlg::SetTitle
//

VOID CProgressDlg::SetTitle(PCTSTR pszTitle)
{
	SetWindowText(m_hDlg, pszTitle);
}

//////////////////////////////////////////////////////////////////////////
//
// CProgressDlg::SetMessage
//

VOID CProgressDlg::SetMessage(PCTSTR pszMessage)
{
	SetDlgItemText(m_hDlg, IDC_MESSAGE, pszMessage);
}

//////////////////////////////////////////////////////////////////////////
//
// CProgressDlg::SetPercent
//

VOID CProgressDlg::SetPercent(UINT nPercent)
{
	SendDlgItemMessage(m_hDlg, IDC_PROGRESS_BAR, PBM_SETPOS, (WPARAM)nPercent, 0);
}

//////////////////////////////////////////////////////////////////////////
//
// CProgressDlg::ThreadProc
//

DWORD WINAPI CProgressDlg::ThreadProc(PVOID pParameter)
{
	CProgressDlg *that = (CProgressDlg *)pParameter;

	INT_PTR nResult = DialogBoxParam(
		NULL,
		MAKEINTRESOURCE(IDD_PROGRESS),
		that->m_hWndParent,
		DialogProc,
		(LPARAM)that
	);

	return (DWORD)nResult;
}

HWND CProgressDlg::GetDlgWindow()
{
	return m_hDlg;
}

//////////////////////////////////////////////////////////////////////////
//
// CProgressDlg::DialogProc
//

INT_PTR
CALLBACK
CProgressDlg::DialogProc(
	HWND   hDlg,
	UINT   uMsg,
	WPARAM wParam,
	LPARAM lParam
)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		// Retrieve and store the "this" pointer

		CProgressDlg *that = (CProgressDlg *)lParam;

		SetWindowLongPtr(hDlg, DWLP_USER, (LONG_PTR)that);

		that->m_hDlg = hDlg;

		// Initialize progress bar to the range 0 to 100 and set bar colour to purple

		SendDlgItemMessage(hDlg, IDC_PROGRESS_BAR, PBM_SETRANGE32, (WPARAM)0, (LPARAM)100);
		SendDlgItemMessage(hDlg, IDC_PROGRESS_BAR, PBM_SETBARCOLOR, (WPARAM)0, (LPARAM)RGB(130, 63, 158));
		// Signal the main thread that we are ready

		SetEvent(that->m_hInitDlg);

		return TRUE;
	}

	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDCANCEL:
		{
			// If the user presses the cancel button, set the m_bCancelled flag.

			CProgressDlg *that = (CProgressDlg *)GetWindowLongPtr(hDlg, DWLP_USER);

			InterlockedIncrement(&that->m_bCancelled);

			// The cancel operation will probably take some time, so 
			// change the cancel button text to "Wait...", so that 
			// the user will see the cancel command is received and 
			// is being processed

			wchar_t szWait[DEFAULT_STRING_SIZE] = _T("");

			LoadString(NULL, IDS_WAIT, szWait, COUNTOF(szWait));

			SetDlgItemText(hDlg, IDCANCEL, szWait);

			return TRUE;
		}
		}

		break;
	}
	}

	return FALSE;
}


HRESULT
CALLBACK
DefaultProgressCallback(
	LONG   lStatus,
	LONG   lPercentComplete,
	PVOID  pParam
)
{
	CProgressDlg *pProgressDlg = (CProgressDlg *)pParam;

	if (pProgressDlg == NULL)
	{
		return E_POINTER;
	}

	// If the user has pressed the cancel button, abort transfer

	if (pProgressDlg->Cancelled())
	{
		return S_FALSE;
	}

	// Form the message text

	UINT uID;

	switch (lStatus)
	{
	case IT_STATUS_TRANSFER_FROM_DEVICE:
	{
		uID = IDS_STATUS_TRANSFER_FROM_DEVICE;
		break;
	}

	case IT_STATUS_PROCESSING_DATA:
	{
		uID = IDS_STATUS_PROCESSING_DATA;
		break;
	}

	case IT_STATUS_TRANSFER_TO_CLIENT:
	{
		uID = IDS_STATUS_TRANSFER_TO_CLIENT;
		break;
	}

	default:
	{
		return E_INVALIDARG;
	}
	}

	wchar_t szFormat[DEFAULT_STRING_SIZE] = _T("%d");

	LoadString(NULL, uID, szFormat, COUNTOF(szFormat));

	wchar_t szStatusText[DEFAULT_STRING_SIZE];

	_sntprintf_s(szStatusText, COUNTOF(szStatusText), COUNTOF(szStatusText) - 1, szFormat, lPercentComplete);



	// Update the progress bar values

	pProgressDlg->SetMessage(szStatusText);

	pProgressDlg->SetPercent(lPercentComplete);

	return S_OK;
}


#endif