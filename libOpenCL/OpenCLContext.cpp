#include "OpenCLContext.h"
#include "utility.h"
using namespace Regards::OpenCL;



COpenCLContext::COpenCLContext()
{
	platform = nullptr;
	device = nullptr;
	context = nullptr;
	queue = nullptr;
	num_of_devices = 0;
}



COpenCLContext::~COpenCLContext()
{
	try
	{
		// Release objects in the opposite order of creation

		if (queue)
		{
			cl_int err = clReleaseCommandQueue(queue);
            
            Error::CheckError(err);
		}

		if (context)
		{
			cl_int err = clReleaseContext(context);
            
            Error::CheckError(err);
		}
	}
	catch (...)
	{
		//destructorException();
	}
}

int COpenCLContext::GenerateContext(const wxString &platform_name_or_index, const wxString &device_name_or_index, const wxString &device_type)
{
	try
	{
		platform = SelectPlatform(platform_name_or_index);
		if (SelectDevice(device_name_or_index) == 0)
			return -1;
		CreateContext();
		CreateQueue(); 
		return 0;
	}
	catch (...)
	{
		return -1;
	}
}

void COpenCLContext::CreateQueue(cl_command_queue_properties queue_properties)
{
    
   
	if (!device)
	{
		throw Error("Device is not selected");
	}

	cl_int err = 0;
	queue = clCreateCommandQueue(context, device, queue_properties, &err);
	 Error::CheckError(err);
}


cl_platform_id COpenCLContext::SelectPlatform(const wxString& platform_name_or_index)
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


cl_device_type COpenCLContext::ParseDeviceType(const wxString& device_type_name)
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
			device_type |= CL_DEVICE_TYPE_ALL;
			continue;
		}

		if (
			name == "default" ||
			name == "DEFAULT" ||
			name == "CL_DEVICE_TYPE_DEFAULT"
			)
		{
			device_type |= CL_DEVICE_TYPE_DEFAULT;
			continue;
		}

		if (
			name == "cpu" ||
			name == "CPU" ||
			name == "CL_DEVICE_TYPE_CPU"
			)
		{
			device_type |= CL_DEVICE_TYPE_CPU;
			continue;
		}

		if (
			name == "gpu" ||
			name == "GPU" ||
			name == "CL_DEVICE_TYPE_GPU"
			)
		{
			device_type |= CL_DEVICE_TYPE_GPU;
			continue;
		}

		if (
			name == "acc" ||
			name == "ACC" ||
			name == "accelerator" ||
			name == "ACCELERATOR" ||
			name == "CL_DEVICE_TYPE_ACCELERATOR"
			)
		{
			device_type |= CL_DEVICE_TYPE_ACCELERATOR;
			continue;
		}

		throw Error(
			"Cannot recognize " + device_type_name + " as a device type"
			);
	}
	return device_type;
}

int COpenCLContext::SelectDevice(const wxString& device_name_or_index)
{
    
	if (!platform)
	{
		throw Error("Platform is not selected");
	}

	// List devices of a given type only
	wxString device_type_name = "GPU";
	cl_device_type device_type = ParseDeviceType(device_type_name);
	

	cl_int err = clGetDeviceIDs(
		platform,
		device_type,
		0,
		0,
		&num_of_devices
		);

	if (num_of_devices == 0)
	{
		device_type_name = "CPU";

		device_type = ParseDeviceType("CPU");


		err = clGetDeviceIDs(
			platform,
			device_type,
			0,
			0,
			&num_of_devices
			);

		if (num_of_devices == 0)
			return 0;
	}


	Error::CheckError(err);

	vector<cl_device_id> devices(num_of_devices);

	err = clGetDeviceIDs(
		platform,
		device_type,
		num_of_devices,
		&devices[0],
		0
		);
	Error::CheckError(err);

	cl_uint selected_device_index = num_of_devices;
	bool by_index = false;

	if (Error::is_number(device_name_or_index))
	{
		// Select device by index:
		by_index = true;
		
		// Detection is simple: just try to represent x as an int
		try
		{
			// If x is a number, then str_to returns without an exception
			// In case when x cannot be converted to int
			// str_to rises Error exception (see str_to definitin)
			selected_device_index = Error::str_to(device_name_or_index);

		}
		catch (const Error&)
		{
			// fail: x is not a number
			selected_device_index = 0;
		}

		// does not return here; need to look at the complete devices list
	}

	// this is ignored in case when we have device already selected by index
	wxString required_device_subname = device_name_or_index;

	//cout << "Devices (" << num_of_devices;
	if (device_type != CL_DEVICE_TYPE_ALL)
	{
		//cout << "; filtered by type " << device_type_name;
	}
	//cout << "):\n";

	for (cl_uint i = 0; i < num_of_devices; ++i)
	{
		// Get the length for the i-th device name
		size_t device_name_length = 0;
		err = clGetDeviceInfo(
			devices[i],
			CL_DEVICE_NAME,
			0,
			0,
			&device_name_length
			);
		Error::CheckError(err);;

		// Get the name itself for the i-th device
		// use vector for automatic memory management
		vector<char> device_name(device_name_length);
		err = clGetDeviceInfo(
			devices[i],
			CL_DEVICE_NAME,
			device_name_length,
			&device_name[0],
			0
			);
		Error::CheckError(err);;

		//cout << "    [" << i << "] " << &device_name[0];

		// decide if this i-th device is what you are looking for
		// select the first matched skipping the next one if any
		if (
			(
			by_index &&
			selected_device_index == i  // we already selected the device by index
			) ||
			(
			!by_index &&
			wxString(&device_name[0]).find(required_device_subname) != wxString::npos &&
			selected_device_index == num_of_devices   // haven't selected yet
			)
			)
		{
			//cout << " [Selected]";
			selected_device_index = i;
			// do not stop here, just see all available devices
		}

		// TODO Something when more than one device matches a given subname

		//cout << endl;
	}

	if (by_index && selected_device_index >= num_of_devices)
	{
		throw Error(
			"Given index of device (" + device_name_or_index + ") "
			"is out of range of available devices" +
			(device_type != CL_DEVICE_TYPE_ALL ?
			" (among devices of type " + device_type_name + ")" :
			wxString("")
			)
			);
	}

	if (!by_index && selected_device_index >= num_of_devices)
	{
		throw Error(
			"There is no found device with name containing \"" +
			required_device_subname + "\" as a substring\n"
			);
	}

	device = devices[selected_device_index];

	return 1;
}


std::vector<cl_device_id> COpenCLContext::SelectDevices(cl_platform_id platform, const wxString& device_type_name)
{
    
	// List devices of a given type only
	cl_device_type device_type = ParseDeviceType(device_type_name);

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
		// Get the length for the i-th device name
		size_t device_name_length = 0;
		err = clGetDeviceInfo(
			devices[i],
			CL_DEVICE_NAME,
			0,
			0,
			&device_name_length
			);
		Error::CheckError(err);;

		// Get the name itself for the i-th device
		// use vector for automatic memory management
		vector<char> device_name(device_name_length);
		err = clGetDeviceInfo(
			devices[i],
			CL_DEVICE_NAME,
			device_name_length,
			&device_name[0],
			0
			);
		Error::CheckError(err);;

		//cout << "    [" << i << "] " << &device_name[0] << '\n';
	}

	return devices;
}


void COpenCLContext::CreateContext()
{
    
	if (!platform)
	{
		throw Error("Platform is not selected");
	}

	if (!device)
	{
		throw Error("Device is not selected");
	}

	cl_context_properties context_props[] = {
		CL_CONTEXT_PLATFORM,
		cl_context_properties(platform),
		0
	};

	cl_int err = 0;
	context = clCreateContext(context_props, 1, &device, 0, 0, &err);
	Error::CheckError(err);;
}
