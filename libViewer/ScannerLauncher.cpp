#include <header.h>
#include "ScannerLauncher.h"
#include <FileUtility.h>

using namespace Regards::Viewer;

int CScannerLauncher::Launch()
{
    wxString pathProgram;

#if defined(__APPLE__)
    pathProgram = CFileUtility::GetProgramFolderPath() + "/RegardsViewer -p RegardsPDF";
#elif defined(__WXMSW__)
    pathProgram = "RegardsViewer.exe -p RegardsPDF";
#else
    pathProgram = CFileUtility::GetProgramFolderPath() + "/RegardsViewer -p RegardsPDF";
#endif

    wxExecute(pathProgram);
    return 0;
}
