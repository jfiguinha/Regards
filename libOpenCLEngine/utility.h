#pragma once
#include <stdexcept>
#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif
#include <ConvertUtility.h>

// Base class for all exception in samples
class Error : public std::runtime_error
{
public:
	Error(const wxString& msg) :
		std::runtime_error(CConvertUtility::ConvertToStdString(msg))
	{
	}

	static bool is_number(const wxString& x);

	static wxString opencl_error_to_str(cl_int error);


	static wxString to_str(const cl_int& x, std::streamsize width = 0, char fill = ' ');


	static wxString to_str(const wxString& x, std::streamsize width = 0, char fill = ' ');


	static void CheckError(cl_int err);


	// Convert from a string to a value of a given type.
	// T should have operator>> defined to be read from stream.
	static int str_to(const wxString& s);
};
