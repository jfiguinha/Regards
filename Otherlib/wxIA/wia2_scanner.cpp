#include <header.h>
#ifdef __WXMSW__
#include "wia2_scanner.h"

HRESULT  CWia2Scanner::CreateWiaDeviceManager(IWiaDevMgr2 **ppWiaDevMgr) //Vista or later
{
	//
	// Validate arguments
	//
	if (NULL == ppWiaDevMgr)
	{
		return E_INVALIDARG;
	}

	//
	// Initialize out variables
	//
	*ppWiaDevMgr = NULL;

	//Vista or later:
	HRESULT hr = CoCreateInstance(CLSID_WiaDevMgr2, NULL, CLSCTX_LOCAL_SERVER, IID_IWiaDevMgr2, (void**)ppWiaDevMgr);

	//
	// Return the result of creating the device manager
	//
	return hr;
}

HRESULT CWia2Scanner::EnumerateWiaDevices(IWiaDevMgr2 *pWiaDevMgr) //Vista or later

{
	//
	// Validate arguments
	//
	if (NULL == pWiaDevMgr)
	{
		return E_INVALIDARG;
	}

	//
	// Get a device enumerator interface
	//
	IEnumWIA_DEV_INFO *pWiaEnumDevInfo = NULL;
	HRESULT hr = pWiaDevMgr->EnumDeviceInfo(WIA_DEVINFO_ENUM_LOCAL, &pWiaEnumDevInfo);
	if (SUCCEEDED(hr))
	{
		//
		// Loop until you get an error or pWiaEnumDevInfo->Next returns
		// S_FALSE to signal the end of the list.
		//
		while (S_OK == hr)
		{
			//
			// Get the next device's property storage interface pointer
			//
			IWiaPropertyStorage *pWiaPropertyStorage = NULL;
			hr = pWiaEnumDevInfo->Next(1, &pWiaPropertyStorage, NULL);

			//
			// pWiaEnumDevInfo->Next will return S_FALSE when the list is
			// exhausted, so check for S_OK before using the returned
			// value.
			//
			if (hr == S_OK)
			{
				//
				// Do something with the device's IWiaPropertyStorage*
				//

				//
				// Release the device's IWiaPropertyStorage*
				//
				pWiaPropertyStorage->Release();
				pWiaPropertyStorage = NULL;
			}
		}

		//
		// If the result of the enumeration is S_FALSE (which
		// is normal), change it to S_OK.
		//
		if (S_FALSE == hr)
		{
			hr = S_OK;
		}

		//
		// Release the enumerator
		//
		pWiaEnumDevInfo->Release();
		pWiaEnumDevInfo = NULL;
	}

	//
	// Return the result of the enumeration
	//
	return hr;
}

HRESULT CWia2Scanner::ReadSomeWiaProperties(IWiaPropertyStorage *pWiaPropertyStorage)
{
	//
	// Validate arguments
	//
	if (NULL == pWiaPropertyStorage)
	{
		return E_INVALIDARG;
	}

	//
	// Declare PROPSPECs and PROPVARIANTs, and initialize them to zero.
	//
	PROPSPEC PropSpec[3] = { 0 };
	PROPVARIANT PropVar[3] = { 0 };

	//
	// How many properties are you querying for?
	//
	const ULONG c_nPropertyCount = sizeof(PropSpec) / sizeof(PropSpec[0]);

	//
	// Define which properties you want to read:
	// Device ID.  This is what you would use to create
	// the device.
	//
	PropSpec[0].ulKind = PRSPEC_PROPID;
	PropSpec[0].propid = WIA_DIP_DEV_ID;

	//
	// Device Name
	//
	PropSpec[1].ulKind = PRSPEC_PROPID;
	PropSpec[1].propid = WIA_DIP_DEV_NAME;

	//
	// Device description
	//
	PropSpec[2].ulKind = PRSPEC_PROPID;
	PropSpec[2].propid = WIA_DIP_DEV_DESC;

	//
	// Ask for the property values
	//
	HRESULT hr = pWiaPropertyStorage->ReadMultiple(c_nPropertyCount, PropSpec, PropVar);
	if (SUCCEEDED(hr))
	{
		//
		// IWiaPropertyStorage::ReadMultiple will return S_FALSE if some
		// properties could not be read, so you have to check the return
		// types for each requested item.
		//

		//
		// Check the return type for the device ID
		//
		if (VT_BSTR == PropVar[0].vt)
		{
			//
			// Do something with the device ID
			//
			_tprintf(TEXT("WIA_DIP_DEV_ID: %ws\n"), PropVar[0].bstrVal);
		}

		//
		// Check the return type for the device name
		//
		if (VT_BSTR == PropVar[1].vt)
		{
			//
			// Do something with the device name
			//
			_tprintf(TEXT("WIA_DIP_DEV_NAME: %ws\n"), PropVar[1].bstrVal);
		}

		//
		// Check the return type for the device description
		//
		if (VT_BSTR == PropVar[2].vt)
		{
			//
			// Do something with the device description
			//
			_tprintf(TEXT("WIA_DIP_DEV_DESC: %ws\n"), PropVar[2].bstrVal);
		}

		//
		// Free the returned PROPVARIANTs
		//
		FreePropVariantArray(c_nPropertyCount, PropVar);
	}

	//
	// Return the result of reading the properties
	//
	return hr;
}

//Vista or later:
HRESULT CWia2Scanner::CreateWiaDevice(IWiaDevMgr2 *pWiaDevMgr, BSTR bstrDeviceID, IWiaItem2 **ppWiaDevice)
{
	//
	// Validate arguments
	//
	if (NULL == pWiaDevMgr || NULL == bstrDeviceID || NULL == ppWiaDevice)
	{
		return E_INVALIDARG;
	}

	//
	// Initialize out variables
	//
	*ppWiaDevice = NULL;

	//
	// Create the WIA Device
	//
	HRESULT hr = pWiaDevMgr->CreateDevice(NULL, bstrDeviceID, ppWiaDevice);

	//
	// Return the result of creating the device
	//
	return hr;
}

HRESULT CWia2Scanner::EnumerateItems(IWiaItem2 *pWiaItem) //Vista or later
{
	//
	// Validate arguments
	//
	if (NULL == pWiaItem)
	{
		return E_INVALIDARG;
	}

	//
	// Get the item type for this item.
	//
	LONG lItemType = 0;
	HRESULT hr = pWiaItem->GetItemType(&lItemType);
	if (SUCCEEDED(hr))
	{
		//
		// If it is a folder, or it has attachments, enumerate its children.
		//
		if (lItemType & WiaItemTypeFolder || lItemType & WiaItemTypeHasAttachments)
		{
			//
			// Get the child item enumerator for this item.
			//
			IEnumWiaItem2 *pEnumWiaItem = NULL; //Vista or later

			hr = pWiaItem->EnumChildItems(NULL, &pEnumWiaItem);
			if (SUCCEEDED(hr))
			{
				//
				// Loop until you get an error or pEnumWiaItem->Next returns
				// S_FALSE to signal the end of the list.
				//
				while (S_OK == hr)
				{
					//
					// Get the next child item.
					//
					IWiaItem2 *pChildWiaItem = NULL; //Vista or later

					hr = pEnumWiaItem->Next(1, &pChildWiaItem, NULL);

					//
					// pEnumWiaItem->Next will return S_FALSE when the list is
					// exhausted, so check for S_OK before using the returned
					// value.
					//
					if (S_OK == hr)
					{
						//
						// Recurse into this item.
						//
						hr = EnumerateItems(pChildWiaItem);

						//
						// Release this item.
						//
						pChildWiaItem->Release();
						pChildWiaItem = NULL;
					}
				}

				//
				// If the result of the enumeration is S_FALSE (which
				// is normal), change it to S_OK.
				//
				if (S_FALSE == hr)
				{
					hr = S_OK;
				}

				//
				// Release the enumerator.
				//
				pEnumWiaItem->Release();
				pEnumWiaItem = NULL;
			}
		}
	}
	return  hr;
}

#endif

