#include <header.h>
#include "OpenCLEngine.h"
#include "utility.h"
#include "OpenCLContext.h"
#include "OpenCLProgram.h"
#include <RegardsConfigParam.h>
#include <ParamInit.h>
#include <IFiltreEffet.h>
#include "config_id.h"
#include <opencv2/core/ocl.hpp>
#define GL_MAJOR_VERSION 0x821B
#define GL_MINOR_VERSION 0x821C
using namespace Regards::OpenCL;

#if defined (__APPLE__) || defined(MACOSX)
 static const char* CL_GL_SHARING_EXT = "cl_APPLE_gl_sharing";
 #else
 static const char* CL_GL_SHARING_EXT = "cl_khr_gl_sharing";
 #endif


vector<OpenCLDevice *> COpenCLDeviceList::listOfDevice;
vector<OpenCLPlatform *> COpenCLPlatformList::listOfPlatform;

void COpenCLPlatformList::GetListOfPlatform()
{
    printf("GetListOfPlatform \n");
    Error error("error");
	cl_uint num_of_platforms = 0;
	// get total number of available platforms:
	cl_int err = clGetPlatformIDs(0, 0, &num_of_platforms);
	error.CheckError(err);

	// use vector for automatic memory management
	vector<cl_platform_id> platforms(num_of_platforms);
	// get IDs for all platforms:
	err = clGetPlatformIDs(num_of_platforms, &platforms[0], 0);
	error.CheckError(err);

	//cl_uint selected_platform_index = num_of_platforms;
	//bool by_index = false;

	//cout << "Platforms (" << num_of_platforms << "):\n";

	// TODO In case of empty platform name select the default platform or 0th platform?

	for (cl_uint i = 0; i < num_of_platforms; ++i)
	{
		// Get the length for the i-th platform name
		size_t platform_name_length = 0;
		err = clGetPlatformInfo(
			platforms[i],
			CL_PLATFORM_NAME,
			0,
			0,
			&platform_name_length
			);
		error.CheckError(err);

		

		// Get the name itself for the i-th platform
		// use vector for automatic memory management
		vector<char> platform_name(platform_name_length);
		err = clGetPlatformInfo(
			platforms[i],
			CL_PLATFORM_NAME,
			platform_name_length,
			&platform_name[0],
			0
			);
		error.CheckError(err);

		OpenCLPlatform * device = new OpenCLPlatform();
		device->platformIndex = i;
		device->platformName = wxString(&platform_name[0]);
		device->platformId = platforms[i];
		listOfPlatform.push_back(device);
		//cout << "    [" << i << "] " << &platform_name[0];
	}
    
    printf("GetListOfPlatform size : %d \n",listOfPlatform.size());
}

cl_platform_id COpenCLDeviceList::SelectPlatform(const wxString& platform_name_or_index)
{

	cl_uint num_of_platforms = 0;
	// get total number of available platforms:
	cl_int err = clGetPlatformIDs(0, 0, &num_of_platforms);
	Error::CheckError(err);

	// use vector for automatic memory management
	vector<cl_platform_id> platforms(num_of_platforms);
	// get IDs for all platforms:
	err = clGetPlatformIDs(num_of_platforms, &platforms[0], 0);
	Error::CheckError(err);

	cl_uint selected_platform_index = num_of_platforms;
	bool by_index = false;

	if (Error::is_number(platform_name_or_index))
	{
		// Select platform by index:
		by_index = true;

		// Detection is simple: just try to represent x as an int
		try
		{
			// If x is a number, then str_to returns without an exception
			// In case when x cannot be converted to int
			// str_to rises Error exception (see str_to definitin)
			selected_platform_index = Error::str_to(platform_name_or_index);

		}
		catch (const Error&)
		{
			// fail: x is not a number
			selected_platform_index = 0;
		}
		// does not return here; need to look at the complete platfrom list
	}

	// this is ignored in case when we have platform already selected by index
	wxString required_platform_subname = platform_name_or_index;

	//cout << "Platforms (" << num_of_platforms << "):\n";

	// TODO In case of empty platform name select the default platform or 0th platform?

	for (cl_uint i = 0; i < num_of_platforms; ++i)
	{
		// Get the length for the i-th platform name
		size_t platform_name_length = 0;
		err = clGetPlatformInfo(
			platforms[i],
			CL_PLATFORM_NAME,
			0,
			0,
			&platform_name_length
			);
		Error::CheckError(err);;

		// Get the name itself for the i-th platform
		// use vector for automatic memory management
		vector<char> platform_name(platform_name_length);
		err = clGetPlatformInfo(
			platforms[i],
			CL_PLATFORM_NAME,
			platform_name_length,
			&platform_name[0],
			0
			);
		Error::CheckError(err);;

		//cout << "    [" << i << "] " << &platform_name[0];

		// decide if this i-th platform is what we are looking for
		// we select the first one matched skipping the next one if any
		//
		if (
			selected_platform_index == i || (// we already selected the platform by index
			wxString(&platform_name[0]).find(required_platform_subname) != wxString::npos &&
			selected_platform_index == num_of_platforms) // haven't selected yet
			)
		{
			//cout << " [Selected]";
			selected_platform_index = i;
			// do not stop here, just want to see all available platforms
		}

		// TODO Something when more than one platform matches a given subname

		//cout << endl;
	}

	if (by_index && selected_platform_index >= num_of_platforms)
	{
		throw Error(
			"Given index of platform (" + platform_name_or_index + ") "
			"is out of range of available platforms"
			);
	}

	if (!by_index && selected_platform_index >= num_of_platforms)
	{
		throw Error(
			"There is no found platform with name containing \"" +
			required_platform_subname + "\" as a substring\n"
			);
	}

	return platforms[selected_platform_index];
}


cl_device_type COpenCLDeviceList::ParseDeviceType(const wxString& device_type_name)
{
	cl_device_type  device_type = 0;
	for (size_t pos = 0, next = 0; next != wxString::npos; pos = next + 1)
	{
		next = device_type_name.find_first_of("+|", pos);
		size_t substr_len = (next != wxString::npos) ? (next - pos) : (string::npos);
		wxString name = device_type_name.substr(pos, substr_len);
		if (
			name == "all" ||
			name == "ALL" ||
			name == "CL_DEVICE_TYPE_ALL"
			)
		{
			return CL_DEVICE_TYPE_ALL;
		}

		if (
			name == "default" ||
			name == "DEFAULT" ||
			name == "CL_DEVICE_TYPE_DEFAULT"
			)
		{
			return CL_DEVICE_TYPE_DEFAULT;
		}

		if (
			name == "cpu" ||
			name == "CPU" ||
			name == "CL_DEVICE_TYPE_CPU"
			)
		{
			return CL_DEVICE_TYPE_CPU;
		}

		if (
			name == "gpu" ||
			name == "GPU" ||
			name == "CL_DEVICE_TYPE_GPU"
			)
		{
			return CL_DEVICE_TYPE_GPU;
		}

		if (
			name == "acc" ||
			name == "ACC" ||
			name == "accelerator" ||
			name == "ACCELERATOR" ||
			name == "CL_DEVICE_TYPE_ACCELERATOR"
			)
		{
			return CL_DEVICE_TYPE_ACCELERATOR;
		}

		throw Error(
			"Cannot recognize " + device_type_name + " as a device type"
			);
	}
	return device_type;
}

char *strnstr(const char *s1,const char *s2, int length) {
    if(s1 == NULL || s2 == NULL) 
		return NULL;
    printf("searching \n\n\"%s\"\n for %.*s\n", s1, length, s2);

    char *ss1 = (char *)malloc(strlen(s1) + 1);
    strcpy(ss1, s1);
    char *ss2 = (char *)malloc(length + 1);
    strncpy(ss2, s2, length);
    char *result = strstr(ss1, ss2);
    free(ss1);
    free(ss2);
    return result;
}


int COpenCLDeviceList::IsExtensionSupported(const char* support_str, const char* ext_string, size_t ext_buffer_size)
{
	size_t offset = 0;
	const char* space_substr = strnstr(ext_string + offset, " ", ext_buffer_size - offset);
	size_t space_pos = space_substr ? space_substr - ext_string : 0;
	while (space_pos < ext_buffer_size)
	{
		if( strncmp(support_str, ext_string + offset, space_pos) == 0 )
		{
			// Device supports requested extension!
			printf("Info: Found extension support %s!\n", support_str);
			return 1;
		}
		// Keep searching -- skip to next token string
		offset = space_pos + 1;
		space_substr = strnstr(ext_string + offset, " ", ext_buffer_size - offset);
		space_pos = space_substr ? space_substr - ext_string : 0;
	}
	printf("Warning: Extension not supported %s!\n", support_str);
	return 0;
}

wxString COpenCLDeviceList::GetDeviceInfo(cl_device_id device, cl_device_info param_name)
{
                    // Get the length for the i-th device name
    size_t device_name_length = 0;
    cl_int err = clGetDeviceInfo(
        device,
        param_name,
        0,
        0,
        &device_name_length
        );
    Error::CheckError(err);;

    // Get the name itself for the i-th device
    // use vector for automatic memory management
    vector<char> device_name(device_name_length);
    err = clGetDeviceInfo(
        device,
        param_name,
        device_name_length,
        &device_name[0],
        0
        );
    Error::CheckError(err);
    
    return wxString(&device_name[0]);
}

void COpenCLDeviceList::GetListOfDevice(vector<OpenCLDevice *> & listOfDevice, cl_platform_id platform, cl_device_type device_type)
{
	cl_uint num_of_devices;

	cl_int err = clGetDeviceIDs(
		platform,
		device_type,
		0,
		0,
		&num_of_devices
		);

	Error::CheckError(err);;

	vector<cl_device_id> devices(num_of_devices);

	err = clGetDeviceIDs(
		platform,
		device_type,
		num_of_devices,
		&devices[0],
		0
		);
	Error::CheckError(err);;

	for (cl_uint i = 0; i < num_of_devices; ++i)
	{
        int supported = 0;
        cl_device_type type;   
        clGetDeviceInfo(devices[i], CL_DEVICE_TYPE, sizeof(type), &type, NULL );
        wxString deviceName = GetDeviceInfo(devices[i], CL_DEVICE_NAME);

#ifdef __TEST_WITHOUTSHARINGOPENGL__

		OpenCLDevice* device = new OpenCLDevice();
		device->deviceIndex = i;
		device->deviceName = deviceName;
		device->platformId = platform;
		device->deviceId = devices[i];
		device->deviceType = type;
		device->openGlSharing = 0;
		listOfDevice.push_back(device);

#else
        if(type == CL_DEVICE_TYPE_GPU)
        {
            wxString supportExt = GetDeviceInfo(devices[i], CL_DEVICE_EXTENSIONS);
            supported = supportExt.find(CL_GL_SHARING_EXT);
            if(supported > 0)
                supported = 1;
            else
                supported = 0;            
        }

        OpenCLDevice * device = new OpenCLDevice();
        device->deviceIndex = i;
        device->deviceName = deviceName;
        device->platformId = platform;
        device->deviceId = devices[i];
        device->deviceType = type;
		device->openGlSharing = supported;
        listOfDevice.push_back(device);

#endif
        
        printf("Device found : %s \n", CConvertUtility::ConvertToUTF8(deviceName));
	}
}

OpenCLDevice * COpenCLEngine::GetDefaultDevice()
{
    wxString platformName;
	OpenCLPlatform * platform = nullptr;
	OpenCLDevice * device = nullptr;
	CRegardsConfigParam * config = CParamInit::getInstance();
	if (config != nullptr)
	{ 
		platformName = config->GetOpenCLPlatformName();
        if(platformName != "")
        {
            platform = COpenCLPlatformList::SelectPlatform(platformName);
            if (platform != nullptr)
            {
                int indexDevice = config->GetOpenCLPlatformIndex();
                device = COpenCLDeviceList::SelectDevice(platform, indexDevice);
            }
        }
        
        if (platform == nullptr)
        {
            printf("GetDefaultDevice Platform is null \n");
            platform = COpenCLPlatformList::SelectPlatform(0);
            if (platform != nullptr)
            {
                device = COpenCLDeviceList::SelectDevice(platform, 0);
            }
        }
	}

    if(device != nullptr)
        printf("GetDefaultDevice Platform Name : %s Device Name : %s \n", CConvertUtility::ConvertToUTF8(platformName), CConvertUtility::ConvertToUTF8(device->deviceName));
    else
        printf("GetDefaultDevice Platform Name : %s Device Name : None \n", CConvertUtility::ConvertToUTF8(platformName));
    return device;
}

COpenCLEngine::COpenCLEngine()
{
	_singleton = nullptr;
	OpenCLPlatform * platform = nullptr;
	OpenCLDevice * device = GetDefaultDevice();

	if (device == nullptr)
	{
		platform = COpenCLPlatformList::SelectPlatform(0);
		device = COpenCLDeviceList::SelectDevice(platform, 0);
	}

	if (device != nullptr)
	{
		_singleton = new COpenCLContext(device->platformId, device->deviceId, device->deviceType, device->openGlSharing);
		int rtnValue = _singleton->GenerateContext();
		if (rtnValue == -1)
		{
			delete _singleton;
			_singleton = nullptr;
		}

		if (platform == nullptr)
		{
			vector<OpenCLPlatform *> listPlatform = COpenCLPlatformList::GetPlatform();

			vector<OpenCLPlatform *>::iterator i = std::find_if(listPlatform.begin(),
				listPlatform.end(),
				[&](const auto& val) { return val->platformId == device->platformId; });

			if (i != listPlatform.end())
				platform = (OpenCLPlatform *)(*i);
			/*
			for (OpenCLPlatform * _plateform : listPlatform)
			{
				if (device->platformId == _plateform->platformId)
					platform = _plateform;
			}
			*/
		}
	}

	if (platform != nullptr && _singleton != nullptr && device != nullptr)
	{
		//Set OpenCV to use OpenCL context
		cv::ocl::setUseOpenCL(true);
		cv::ocl::attachContext(platform->platformName.ToStdString(), platform->platformId, _singleton->GetContext(), device->deviceId);
	}
}

COpenCLContext * COpenCLEngine::GetInstance()
{
	return _singleton;
}


COpenCLEngine::~COpenCLEngine()
{
	if (nullptr != _singleton)
	{
		delete _singleton;
		_singleton = nullptr;
	}

}

OpenCLDevice * COpenCLDeviceList::SelectDevice(const wxString &deviceName)
{
    if(listOfDevice.size() == 0)
        GetAllDevice();
    
   // OpenCLDevice * openCLDeviceSelect = nullptr;
    //int i = 0;

	vector<OpenCLDevice *>::iterator i = std::find_if(listOfDevice.begin(),
		listOfDevice.end(),
		[&](const auto& val) { return val->deviceName == deviceName; });

	if (i != listOfDevice.end())
		return (OpenCLDevice *)(*i);
	/*
    for (OpenCLDevice * openCL : listOfDevice)
    {
        if(deviceName == openCL->deviceName)
        {
            return openCL;
        }
    }
	*/
    return nullptr;
}

void COpenCLDeviceList::GetAllDevice()
{
    printf("GetAllDevice \n");
    vector<OpenCLPlatform *> platformList = COpenCLPlatformList::GetPlatform();
    for(OpenCLPlatform * platform : platformList)
    {
        if (platform != nullptr)
        {
             printf("Platform Name : %s \n",CConvertUtility::ConvertToUTF8(platform->platformName));
            cl_device_type deviceType = ParseDeviceType("ALL");
            try
            {
                GetListOfDevice(listOfDevice, platform->platformId, deviceType);
            }
            catch(...)
            {
                printf("Platform Error Name : %s \n",CConvertUtility::ConvertToUTF8(platform->platformName));
            }
           //deviceType = ParseDeviceType("CPU");
           //GetListOfDevice(listOfDevice, platform->platformId, deviceType);
        }
    }
    
}

OpenCLDevice * COpenCLDeviceList::SelectDevice(OpenCLPlatform * platform, const int &index)
{       
    //OpenCLDevice * deviceSelected = nullptr;

    if (listOfDevice.size() == 0)
        GetAllDevice();
	
	vector<OpenCLDevice *>::iterator i = std::find_if(listOfDevice.begin(),
		listOfDevice.end(),
		[&](const auto& openCL) { return openCL->deviceIndex == index && platform->platformId == openCL->platformId; });

	if (i != listOfDevice.end())
		return (OpenCLDevice *)(*i);	
       
	return nullptr;	   
	/*
    for (OpenCLDevice * openCL : listOfDevice)
    {
        if(openCL->deviceIndex == index && platform->platformId == openCL->platformId)
        {
            deviceSelected = openCL;
            break;
        }
    }
	

    return deviceSelected;
	*/
}

vector<OpenCLDevice *> COpenCLDeviceList::GetPlatformDevice(OpenCLPlatform * platform)
{
    vector<OpenCLDevice *> listSelectDevice;
    
    if (listOfDevice.size() == 0)
        GetAllDevice();
        
    for (OpenCLDevice * openCL : listOfDevice)
    {
        if(platform->platformId == openCL->platformId)
        {
            listSelectDevice.push_back(openCL);
        }
    }

    return listSelectDevice;
}
