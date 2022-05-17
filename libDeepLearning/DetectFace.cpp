//
//  Hello.cpp
//  Regards
//
//  Created by figuinha jacques on 10/08/2015.
//
//
#include "header.h"
#include "DetectFace.h"
#include <RegardsBitmap.h>
#include <RegardsConfigParam.h>
#include <ParamInit.h>
#include <FileUtility.h>

#ifdef __APPLE__
#include "MyDetectFaceImpl.h"
using namespace cv;

#include <OpenCLContext.h>
using namespace Regards::OpenCL;
#else
using namespace cv;
using namespace dnn;

#include <OpenCLContext.h>
using namespace Regards::OpenCL;
static Net net; // And finally we load the DNN responsible for face recognition.
//std::mutex CDetectFace::muDnnAccess;
bool CDetectFace::isload = false;

#endif



extern COpenCLContext* openclContext;

CDetectFace::CDetectFace(void)
{
#ifdef __APPLE__
	_impl = new MyDetectFaceImpl();
#endif
}


CDetectFace::~CDetectFace(void)
{
#ifdef __APPLE__
	if (_impl) { delete _impl; _impl = NULL; }
#endif
}




//--------------------------------------------------
//Code From https://github.com/spmallick/learnopencv
//--------------------------------------------------
void CDetectFace::DetectFace(const Mat& bitmap, const float & confidenceThreshold, std::vector<CFace>& listOfFace, std::vector<cv::Rect>& pointOfFace)
{
#ifdef __APPLE__

    float bestConfidence = 0;
	vector<FaceRect> listFaceRect;
	int faceDetect = 0;
	if (_impl)
		faceDetect = _impl->DetectRectFace(confidenceThreshold, bestConfidence, bitmap.size().width, bitmap.size().height, bitmap.data, listFaceRect);

	if (faceDetect > 0)
	{
		Mat frameOpenCVDNN = bitmap;
		for (int i = 0; i < listFaceRect.size(); i++)
		{
			CFace face;
			

			FaceRect rect = listFaceRect[i];

			int x1 = rect.x;
			int y1 = rect.y;
			int x2 = rect.x + rect.width;
			int y2 = rect.y + rect.height;

            if (rect.confidence > confidenceThreshold)
			{
                try{
                    face.confidence = rect.confidence;
                    face.myROI = Rect(Point(x1, y1), Point(x2, y2));
					frameOpenCVDNN(face.myROI).copyTo(face.croppedImage);
                    listOfFace.push_back(face);
                    pointOfFace.push_back(face.myROI);   
                }
                catch (Exception& e)
                {
                    const char* err_msg = e.what();
                    std::cout << "exception caught: " << err_msg << std::endl;
                    std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
                }
            }
		}
	}

#else
	if (!isload)
		return;

	//Mat frameOpenCVDNN(bitmap->GetBitmapHeight(), bitmap->GetBitmapWidth(), CV_8UC4, bitmap->GetPtBitmap());
	Mat frameOpenCVDNN;
    
	cvtColor(bitmap, frameOpenCVDNN, COLOR_BGRA2BGR);
	int frameHeight = frameOpenCVDNN.size().height;
	int frameWidth = frameOpenCVDNN.size().width;

	try
	{

		Mat inputBlob = blobFromImage(frameOpenCVDNN, 1.0, Size(300, 300), (104.0, 177.0, 123.0), false, false);
		net.setInput(inputBlob, "data");
		Mat detection = net.forward("detection_out").clone();

		Mat detectionMat(detection.size[2], detection.size[3], CV_32F, detection.ptr<float>());

		for (int i = 0; i < detectionMat.rows; i++)
		{
			float confidence = detectionMat.at<float>(i, 2);
			if (confidence > confidenceThreshold)
			{
				CFace face;
				face.confidence = confidence;

				int x1 = static_cast<int>(detectionMat.at<float>(i, 3) * frameWidth);
				int y1 = static_cast<int>(detectionMat.at<float>(i, 4) * frameHeight);
				int x2 = static_cast<int>(detectionMat.at<float>(i, 5) * frameWidth);
				int y2 = static_cast<int>(detectionMat.at<float>(i, 6) * frameHeight);

				Rect rect = Rect(Point(x1, y1), Point(x2, y2));
				VerifRectSize(rect, frameOpenCVDNN);
				face.myROI = rect;
				frameOpenCVDNN(face.myROI).copyTo(face.croppedImage);
				listOfFace.push_back(face);
				pointOfFace.push_back(face.myROI);
			}
		}

		detectionMat.release();
		inputBlob.release();
	}
	catch (Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "exception caught: " << err_msg << std::endl;
		std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
	}
#endif
}

int CDetectFace::FindNbFace(const Mat& bitmap, const float& confidenceThreshold, float& bestConfidence)
{
#ifdef __APPLE__

	int faceDetect = 0;
	if (_impl)
		faceDetect = _impl->MyDetectFace(confidenceThreshold, bestConfidence, bitmap.size().width, bitmap.size().height, bitmap.data);
	return faceDetect;

#else

	if (!isload)
		return 0;

	Mat frameOpenCVDNN;// (bitmap->GetBitmapHeight(), bitmap->GetBitmapWidth(), CV_8UC4, bitmap->GetPtBitmap());
	int nbFaceDetect = 0;
	cvtColor(bitmap, frameOpenCVDNN, COLOR_BGRA2BGR);
	try
	{
		Mat inputBlob = blobFromImage(frameOpenCVDNN, 1.0, Size(300, 300), (104.0, 177.0, 123.0), false, false);
		net.setInput(inputBlob, "data");
		Mat detection = net.forward("detection_out").clone();

		Mat detectionMat(detection.size[2], detection.size[3], CV_32F, detection.ptr<float>());

		for (int i = 0; i < detectionMat.rows; i++)
		{
			float confidence = detectionMat.at<float>(i, 2);
			if (confidence > confidenceThreshold)
			{
                if(bestConfidence < confidence)
                    bestConfidence = confidence;
				nbFaceDetect++;
			}
		}

		detectionMat.release();
		inputBlob.release();
	}
	catch (Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "exception caught: " << err_msg << std::endl;
		std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
	}

	return nbFaceDetect;
#endif
}

void CDetectFace::LoadModel()
{

#ifdef __APPLE__
#else

	try
	{

#ifndef __WXGTK__

        wxString tensorflowConfigFile = CFileUtility::GetResourcesFolderPath() + "\\model\\opencv_face_detector.pbtxt";
        wxString tensorflowWeightFile = CFileUtility::GetResourcesFolderPath() + "\\model\\opencv_face_detector_uint8.pb";


		bool openCLCompatible = false;
		CRegardsConfigParam* config = CParamInit::getInstance();
		if (config != nullptr)
		{
			if (config->GetIsOpenCLSupport())
				openCLCompatible = true;
		}

		net = readNetFromTensorflow(tensorflowWeightFile.ToStdString(), tensorflowConfigFile.ToStdString());
		net.setPreferableBackend(DNN_BACKEND_DEFAULT);

		if (openCLCompatible)
			net.setPreferableTarget(DNN_TARGET_OPENCL);
		else
			net.setPreferableTarget(DNN_TARGET_CPU);
#else

        wxString tensorflowConfigFile = CFileUtility::GetResourcesFolderPath() + "/model/opencv_face_detector.pbtxt";
        wxString tensorflowWeightFile = CFileUtility::GetResourcesFolderPath() + "/model/opencv_face_detector_uint8.pb";

		net = readNetFromTensorflow(tensorflowWeightFile.ToStdString(), tensorflowConfigFile.ToStdString());
		net.setPreferableBackend(DNN_BACKEND_DEFAULT);
		net.setPreferableTarget(DNN_TARGET_CPU);
#endif

	}
	catch (Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "exception caught: " << err_msg << std::endl;
		std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
	}
    
    isload = true;
#endif
	
}