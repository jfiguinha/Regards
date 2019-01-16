cmake_minimum_required(VERSION 2.8.11)

project(libOpenCL)



include_directories(
    /home/figuinha/dev/Regards/libOpenCL/
    /home/figuinha/dev/Regards/libOpenCL/
    /home/figuinha/dev/Regards/libSqlite
    /home/figuinha/dev/Regards/libDataStructure
    /home/figuinha/dev/Regards/libFiltreInterface
    /home/figuinha/dev/Regards/libUtility
    /home/figuinha/dev/Regards/libextern/exiv2-0.24/src
    /home/figuinha/dev/Regards/libextern/exiv2-0.24/xmpsdk/include
    /home/figuinha/dev/Regards/libResource
    /home/figuinha/dev/Regards/libParameter
    /opt/intel/intel-opencl-1.2-6.0.0.1049/opencl-sdk/include
    /home/figuinha/dev/Regards/libextern/libRapidXML
    /home/figuinha/dev/Regards/include
    /home/figuinha/dev/Regards/libextern/wxWidgets-3.0.2/include
    /home/figuinha/dev/Regards/libextern/wxWidgets-3.0.2/lib/wx/include/gtk2-unicode-static-3.0

)

add_definitions(
    -DNDEBUG
    -D__WXGTK__
)

# Linker options
set(CMAKE_LDFLAGS "${CMAKE_LDFLAGS}  -s")

# Library path
set(CMAKE_LDFLAGS "${CMAKE_LDFLAGS} -L\".\" ")

# Define the CXX sources
set ( CXX_SRCS
    OpenCLContext.cpp
    OpenCLEffect.cpp
    OpenCLEngine.cpp
    OpenCLExecuteProgram.cpp
    OpenCLInfos.cpp
    OpenCLParameter.cpp
    OpenCLProgram.cpp
    utility.cpp
)

set_source_files_properties(
    ${CXX_SRCS} PROPERTIES COMPILE_FLAGS 
    " -fopenmp -std=c++11 -Wall")

add_library(libOpenCL ${CXX_SRCS} ${C_SRCS})

