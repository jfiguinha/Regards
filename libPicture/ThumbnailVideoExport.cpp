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
	CThumbnailVideoOpenCVExportImpl* thumbnailImpl = nullptr;


	switch (effect)
	{
	case IDM_DIAPORAMA_FUSION:
		thumbnailImpl = new CDiaporamaFusion();
		break;
	case IDM_DIAPORAMA_TRANSITION:
		thumbnailImpl = new CDiaporamaTransition();
		break;
	case IDM_DIAPORAMA_MOVE:
		thumbnailImpl = new CDiaporamaMove();
		break;
	default:
		thumbnailImpl = new CThumbnailVideoOpenCVExportImpl();
		break;
	}

	int movie_duration = 0;
	int fourcc = VideoWriter::fourcc('H', '2', '6', '4');
	Size S = Size((int)width, // Acquire input size
	              (int)height);

	if (thumbnailImpl != nullptr)
	{
		// Open the output
		thumbnailImpl->outputVideo.open(CConvertUtility::ConvertToStdString(outfile), fourcc, fps, S, true);

		if (thumbnailImpl->outputVideo.isOpened())
		{
			movie_duration = thumbnailImpl->ExecuteProcess(outfile, listOfFile, delay, fps, width, height, effect);

			thumbnailImpl->outputVideo.release();

			if (thumbnailImpl->endProcess)
				movie_duration = 0;
		}
	}


	delete thumbnailImpl;
	return movie_duration;
}
