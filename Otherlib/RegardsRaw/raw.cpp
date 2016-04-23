#ifdef LIBRAW
#include "raw.h"
#include <FileUtility.h>
#include <ximage.h>
#include <wx/mstream.h>
#include <vector>
#ifdef __APPLE__
#include <dlfcn.h>
#endif

using namespace std;

#ifdef __APPLE__
CRegardsRaw * CRaw::libRegardsRaw = nullptr;
void * CRaw::lib_handle = nullptr;
#else
CRegardsRaw * CRaw::libRegardsRaw = nullptr;
#endif

void CRaw::Init()
{
#ifdef __APPLE__
    
    if(libRegardsRaw == nullptr)
    {
        wxString path = CFileUtility::GetProgramFolderPath();
        path.Append("/libRegardsRaw.dylib");
        lib_handle = dlopen(path.ToStdString().c_str(), RTLD_LOCAL|RTLD_LAZY);
        if (!lib_handle) {
            printf("[%s] Unable to load library: %s\n", __FILE__, dlerror());
            exit(EXIT_FAILURE);
        }
        
        // Get the NewPerson function.
        RegardsRaw_creator * NewRegardsRaw = (RegardsRaw_creator*)dlsym(lib_handle, "NewRegardsRaw");
        if (!NewRegardsRaw) {
            cerr << "[" << __FILE__ << "] main: Unable to find NewPerson method: "
            << dlerror() << "\n";
            exit(EXIT_FAILURE);
        }
        
        
        
        libRegardsRaw = (CRegardsRaw*)NewRegardsRaw();
    }
#else
	libRegardsRaw = new CRegardsRaw();
    
#endif
}

CxImage * CRaw::GetThumbnail(const wxString & fileName)
{
    Init();
    CxImage * image = nullptr;
    if(libRegardsRaw != nullptr)
    {
        int type = 0;
        std::vector<uint8_t> myVector = libRegardsRaw->GetThumbnail(fileName.ToStdString(), type);
        CxMemFile memFile(&myVector[0], myVector.size());
        
        if(type == JPEGOUTPUT)
        {
            image = new CxImage(&memFile, CxImage::GetTypeIdFromName("jpg"));
        }
        else
        {
            image = new CxImage(&memFile, CxImage::GetTypeIdFromName("ppm"));
        }

        myVector.clear();
        
    }
    return image;
}


void CRaw::GetDimensions(const wxString & fileName, int & width, int & height)
{
    Init();
    if(libRegardsRaw != nullptr)
        libRegardsRaw->GetDimensions(fileName.ToStdString(), width, height);
}

CRegardsBitmap * CRaw::GetPicture(const wxString & fileName)
{
    Init();
    CRegardsBitmap * image = nullptr;
    if(libRegardsRaw != nullptr)
    {
        image = new CRegardsBitmap();
        int type = 0;
        int width = 0;
        int height = 0;
        std::vector<uint8_t> myVector = libRegardsRaw->GetPicture(fileName.ToStdString(), width, height);
        image->SetBitmap(&myVector[0], width, height);
        myVector.clear();
    }
    return image;
}
#endif