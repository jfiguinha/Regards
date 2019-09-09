#pragma once

#include "ProgressDlg.h"

#ifndef MAX_GUID_STRING_LEN
#define MAX_GUID_STRING_LEN 39
#endif //MAX_GUID_STRING_LEN


typedef HRESULT(CALLBACK *PFNPROGRESSCALLBACK)(
	LONG   lStatus,
	LONG   lPercentComplete,
	PVOID  pParam
	);

template <class T>
class CComPtrArray
{
public:
	CComPtrArray()
	{
		m_pArray = NULL;
		m_nCount = 0;
	}

	explicit CComPtrArray(int nCount)
	{
		m_pArray = (T **)CoTaskMemAlloc(nCount * sizeof(T *));

		m_nCount = m_pArray == NULL ? 0 : nCount;

		for (int i = 0; i < m_nCount; ++i)
		{
			m_pArray[i] = NULL;
		}
	}

	CComPtrArray(const CComPtrArray& rhs)
	{
		Copy(rhs);
	}

	~CComPtrArray()
	{
		Clear();
	}

	CComPtrArray &operator =(const CComPtrArray& rhs)
	{
		if (this != &rhs)
		{
			Clear();
			Copy(rhs);
		}

		return *this;
	}

	operator T **()
	{
		return m_pArray;
	}

	bool operator!()
	{
		return m_pArray == NULL;
	}

	T ***operator&()
	{
		return &m_pArray;
	}

	LONG &Count()
	{
		return m_nCount;
	}

	void Clear()
	{
		if (m_pArray != NULL)
		{
			for (int i = 0; i < m_nCount; ++i)
			{
				if (m_pArray[i] != NULL)
				{
					m_pArray[i]->Release();
				}
			}

			CoTaskMemFree(m_pArray);

			m_pArray = NULL;
			m_nCount = 0;
		}
	}

	void Copy(const CComPtrArray& rhs)
	{
		m_pArray = NULL;
		m_nCount = 0;

		if (rhs.m_pArray != NULL)
		{
			m_pArray = (T**)CoTaskMemAlloc(rhs.m_nCount * sizeof(T *));

			if (m_pArray != NULL)
			{
				m_nCount = rhs.m_nCount;

				for (int i = 0; i < m_nCount; ++i)
				{
					m_pArray[i] = rhs.m_pArray[i];

					if (m_pArray[i] != NULL)
					{
						m_pArray[i]->AddRef();
					}
				}
			}
		}
	}

private:
	T    **m_pArray;
	LONG  m_nCount;
};





HRESULT ReadPropertyLong(
	IWiaPropertyStorage *pWiaPropertyStorage,
	const PROPSPEC      *pPropSpec,
	LONG                *plResult
);



HRESULT
ReadPropertyGuid(
	IWiaPropertyStorage *pWiaPropertyStorage,
	const PROPSPEC      *pPropSpec,
	GUID                *pguidResult
);


HRESULT
WiaGetImage(
	HWND                 hWndParent,
	LONG                 lDeviceType,
	LONG                 lFlags,
	LONG                 lIntent,
	IWiaDevMgr          *pSuppliedWiaDevMgr,
	IWiaItem            *pSuppliedItemRoot,
	PFNPROGRESSCALLBACK  pfnProgressCallback,
	PVOID                pProgressCallbackParam,
	GUID                *pguidFormat,
	LONG                *plCount,
	IStream             ***pppStream
);


