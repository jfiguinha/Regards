#pragma once
#include "ThumbnailDiaporama.h"

class CDiaporamaFusion : public CThumbnailVideoOpenCVExportImpl
{
public:
	CDiaporamaFusion()
	{
	};

	~CDiaporamaFusion() override
	{
	};
	int ExecuteEffect(cv::Mat& pictureOne, cv::Mat& pictureTwo, const int& nbFrame, int width, int height,
	                  int effect) override;
	int ExecuteProcess(const wxString& outfile, vector<wxString>& listOfFile, int delay, int fps, int width, int height,
	                   int effect) override;
};
