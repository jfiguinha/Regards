#include <header.h>
#include "utility.h"
#include <ConvertUtility.h>
using namespace std;

#define CASE_CL_CONSTANT(NAME) case NAME: return #NAME;
    
    bool Error::is_number(const wxString& x)
    {
        //return (isdigit(atoi(x.c_str())) == 1) ? true : false;
        try {
            str_to(x);
            return true;
        } catch (const Error &) {
            return false;
        }
    }
    
    wxString Error::opencl_error_to_str(cl_int error)
    {

        
        // Suppose that no combinations are possible.
        // TODO: Test whether all error codes are listed here
        switch (error)
        {
                CASE_CL_CONSTANT(CL_SUCCESS)
                CASE_CL_CONSTANT(CL_DEVICE_NOT_FOUND)
                CASE_CL_CONSTANT(CL_DEVICE_NOT_AVAILABLE)
                CASE_CL_CONSTANT(CL_COMPILER_NOT_AVAILABLE)
                CASE_CL_CONSTANT(CL_MEM_OBJECT_ALLOCATION_FAILURE)
                CASE_CL_CONSTANT(CL_OUT_OF_RESOURCES)
                CASE_CL_CONSTANT(CL_OUT_OF_HOST_MEMORY)
                CASE_CL_CONSTANT(CL_PROFILING_INFO_NOT_AVAILABLE)
                CASE_CL_CONSTANT(CL_MEM_COPY_OVERLAP)
                CASE_CL_CONSTANT(CL_IMAGE_FORMAT_MISMATCH)
                CASE_CL_CONSTANT(CL_IMAGE_FORMAT_NOT_SUPPORTED)
                CASE_CL_CONSTANT(CL_BUILD_PROGRAM_FAILURE)
                CASE_CL_CONSTANT(CL_MAP_FAILURE)
                CASE_CL_CONSTANT(CL_MISALIGNED_SUB_BUFFER_OFFSET)
                CASE_CL_CONSTANT(CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST)
                CASE_CL_CONSTANT(CL_INVALID_VALUE)
                CASE_CL_CONSTANT(CL_INVALID_DEVICE_TYPE)
                CASE_CL_CONSTANT(CL_INVALID_PLATFORM)
                CASE_CL_CONSTANT(CL_INVALID_DEVICE)
                CASE_CL_CONSTANT(CL_INVALID_CONTEXT)
                CASE_CL_CONSTANT(CL_INVALID_QUEUE_PROPERTIES)
                CASE_CL_CONSTANT(CL_INVALID_COMMAND_QUEUE)
                CASE_CL_CONSTANT(CL_INVALID_HOST_PTR)
                CASE_CL_CONSTANT(CL_INVALID_MEM_OBJECT)
                CASE_CL_CONSTANT(CL_INVALID_IMAGE_FORMAT_DESCRIPTOR)
                CASE_CL_CONSTANT(CL_INVALID_IMAGE_SIZE)
                CASE_CL_CONSTANT(CL_INVALID_SAMPLER)
                CASE_CL_CONSTANT(CL_INVALID_BINARY)
                CASE_CL_CONSTANT(CL_INVALID_BUILD_OPTIONS)
                CASE_CL_CONSTANT(CL_INVALID_PROGRAM)
                CASE_CL_CONSTANT(CL_INVALID_PROGRAM_EXECUTABLE)
                CASE_CL_CONSTANT(CL_INVALID_KERNEL_NAME)
                CASE_CL_CONSTANT(CL_INVALID_KERNEL_DEFINITION)
                CASE_CL_CONSTANT(CL_INVALID_KERNEL)
                CASE_CL_CONSTANT(CL_INVALID_ARG_INDEX)
                CASE_CL_CONSTANT(CL_INVALID_ARG_VALUE)
                CASE_CL_CONSTANT(CL_INVALID_ARG_SIZE)
                CASE_CL_CONSTANT(CL_INVALID_KERNEL_ARGS)
                CASE_CL_CONSTANT(CL_INVALID_WORK_DIMENSION)
                CASE_CL_CONSTANT(CL_INVALID_WORK_GROUP_SIZE)
                CASE_CL_CONSTANT(CL_INVALID_WORK_ITEM_SIZE)
                CASE_CL_CONSTANT(CL_INVALID_GLOBAL_OFFSET)
                CASE_CL_CONSTANT(CL_INVALID_EVENT_WAIT_LIST)
                CASE_CL_CONSTANT(CL_INVALID_EVENT)
                CASE_CL_CONSTANT(CL_INVALID_OPERATION)
                CASE_CL_CONSTANT(CL_INVALID_GL_OBJECT)
                CASE_CL_CONSTANT(CL_INVALID_BUFFER_SIZE)
                CASE_CL_CONSTANT(CL_INVALID_MIP_LEVEL)
                CASE_CL_CONSTANT(CL_INVALID_GLOBAL_WORK_SIZE)
                CASE_CL_CONSTANT(CL_INVALID_PROPERTY)
                
            default:
                return "UNKNOWN ERROR CODE " + to_str(error);
        }
        

    }
    
    wxString Error::to_str(const cl_int & x, std::streamsize width, char fill)
    {
        using namespace std;
        ostringstream os;
        os << setw((int)width) << setfill(fill) << x;
        if (!os)
        {
            throw Error("Cannot represent object as a string");
        }
        return os.str();
    }
    
    wxString Error::to_str(const wxString & x, std::streamsize width, char fill)
    {
        using namespace std;
        ostringstream os;
        os << setw((int)width) << setfill(fill) << x;
        if (!os)
        {
            throw Error("Cannot represent object as a string");
        }
        return os.str();
    }
    
    void Error::CheckError(cl_int err)
    {

        if(err != CL_SUCCESS)
        {
			printf("Error : %s happened in file %s at line %s /n",CConvertUtility::ConvertToUTF8(opencl_error_to_str(err)),CConvertUtility::ConvertToUTF8(to_str(__FILE__)),CConvertUtility::ConvertToUTF8(to_str(__LINE__)));
			wxString message = "OpenCL error " +
                        opencl_error_to_str(err) +
                        " happened in file " + to_str(__FILE__) +
                        " at line " + to_str(__LINE__) + ".";
			#ifdef WIN32
            OutputDebugString(message);
            #endif
            throw Error(message);
        }
    }
    
    // Convert from a string to a value of a given type.
    // T should have operator>> defined to be read from stream.
    int Error::str_to(const wxString& s)
    {
        std::istringstream ss(CConvertUtility::ConvertToStdString(s));
        int res;
        ss >> res;
        
        if (!ss || (ss.get(), ss))
        {
            throw Error(
                        "Cannot interpret string "
                        );
        }
        
        return res;
    }


#undef CASE_CL_CONSTANT



