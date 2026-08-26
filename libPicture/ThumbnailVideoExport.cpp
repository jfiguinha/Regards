// ReSharper disable All
#include <header.h>
#include "ThumbnailVideoExport.h"
#include "ThumbnailDiaporama.h"
#include "DiaporamaFusion.h"
#include "DiaporamaMove.h"
#include "DiaporamaTransition.h"
#include <ConvertUtility.h>
#include <effect_id.h>
using namespace cv;


//**********************************************************************
//
//**********************************************************************
int CThumbnailVideoExport::GenerateVideoFromList(const wxString& outfile, vector<wxString>& listOfFile, int delay,
                                                 int fps, int width, int height, int effect)
{
    std::unique_ptr<CThumbnailVideoOpenCVExportImpl> thumbnailImpl;

    switch (effect)
    {
    case IDM_DIAPORAMA_FUSION:
        thumbnailImpl = std::make_unique<CDiaporamaFusion>();
        break;

    case IDM_DIAPORAMA_TRANSITION:
        thumbnailImpl = std::make_unique<CDiaporamaTransition>();
        break;

    case IDM_DIAPORAMA_MOVE:
        thumbnailImpl = std::make_unique<CDiaporamaMove>();
        break;

    default:
        thumbnailImpl = std::make_unique<CThumbnailVideoOpenCVExportImpl>();
        break;
    }

    const int fourcc = VideoWriter::fourcc('H', '2', '6', '4');

    const Size videoSize(
        static_cast<int>(width),
        static_cast<int>(height));

    thumbnailImpl->outputVideo.open(
        CConvertUtility::ConvertToStdString(outfile),
        fourcc,
        fps,
        videoSize,
        true);

    if (!thumbnailImpl->outputVideo.isOpened())
        return 0;

    const int movieDuration = thumbnailImpl->ExecuteProcess(
        outfile,
        listOfFile,
        delay,
        fps,
        width,
        height,
        effect);

    return thumbnailImpl->endProcess ? 0 : movieDuration;
}
