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
#ifdef __APPLE__
#include "MyDetectFaceImpl.h"
using namespace cv;
#else
using namespace cv;
using namespace dnn;

static Net net; // And finally we load the DNN responsible for face recognition.
//std::mutex CDetectFace::muDnnAccess;
bool CDetectFace::isload = false;

#endif



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

bool CDetectFace::LockOpenCLDnn()
{
	return true;
}

bool CDetectFace::UnlockOpenCLDnn()
{
	return true;
}



//--------------------------------------------------
//Code From https://github.com/spmallick/learnopencv
//--------------------------------------------------
void CDetectFace::DetectFace(CRegardsBitmap * bitmap, const int& confidenceThreshold, std::vector<CFace>& listOfFace, std::vector<cv::Rect>& pointOfFace)
{
#ifdef __APPLE__

	vector<FaceRect> listFaceRect;
	int faceDetect = 0;
	if (_impl)
		faceDetect = _impl->DetectRectFace(bitmap->GetBitmapWidth(), bitmap->GetBitmapHeight(), bitmap->GetPtBitmap(), listFaceRect);

	if (faceDetect > 0)
	{
		Mat frameOpenCVDNN(bitmap->GetBitmapHeight(), bitmap->GetBitmapWidth(), CV_8UC4, bitmap->GetPtBitmap());
		cvtColor(frameOpenCVDNN, frameOpenCVDNN, COLOR_BGRA2BGR);


		for (int i = 0; i < listFaceRect.size(); i++)
		{
			CFace face;
			face.confidence = 1.0;

			FaceRect rect = listFaceRect[i];

			int x1 = rect.x;
			int y1 = rect.y;
			int x2 = rect.x + rect.width;
			int y2 = rect.y + rect.height;

            try{
                face.myROI = Rect(Point(x1, y1), Point(x2, y2));
                face.croppedImage = frameOpenCVDNN(face.myROI);
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

#else
	if (!isload)
		return;

	Mat frameOpenCVDNN(bitmap->GetBitmapHeight(), bitmap->GetBitmapWidth(), CV_8UC4, bitmap->GetPtBitmap());
	cvtColor(frameOpenCVDNN, frameOpenCVDNN, COLOR_BGRA2BGR);
	int frameHeight = frameOpenCVDNN.rows;
	int frameWidth = frameOpenCVDNN.cols;

	try
	{
#ifdef CAFFE
		cv::Mat inputBlob = cv::dnn::blobFromImage(frameOpenCVDNN, 1.0, cv::Size(300, 300), (104.0, 177.0, 123.0), false, false);
#else

		Mat inputBlob = blobFromImage(frameOpenCVDNN, 1.0, Size(300, 300), (104.0, 177.0, 123.0), false, false);

#endif
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

				face.myROI = Rect(Point(x1, y1), Point(x2, y2));
				face.croppedImage = frameOpenCVDNN(face.myROI);
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

int CDetectFace::FindNbFace(CRegardsBitmap* bitmap, const int& confidenceThreshold, float& bestConfidence, const float& confidence)
{
#ifdef __APPLE__

	int faceDetect = 0;
	if (_impl)
		faceDetect = _impl->MyDetectFace(bitmap->GetBitmapWidth(), bitmap->GetBitmapHeight(), bitmap->GetPtBitmap());
	return faceDetect;

#else

	if (!isload)
		return;

	Mat frameOpenCVDNN(bitmap->GetBitmapHeight(), bitmap->GetBitmapWidth(), CV_8UC4, bitmap->GetPtBitmap());
	int nbFaceDetect = 0;
	cvtColor(frameOpenCVDNN, frameOpenCVDNN, COLOR_BGRA2BGR);
	try
	{
#ifdef CAFFE
		cv::Mat inputBlob = cv::dnn::blobFromImage(frameOpenCVDNN, 1.0, cv::Size(300, 300), (104.0, 177.0, 123.0), false, false);
#else

		Mat inputBlob = blobFromImage(frameOpenCVDNN, 1.0, Size(300, 300), (104.0, 177.0, 123.0), false, false);

#endif
		net.setInput(inputBlob, "data");
		Mat detection = net.forward("detection_out").clone();

		Mat detectionMat(detection.size[2], detection.size[3], CV_32F, detection.ptr<float>());

		for (int i = 0; i < detectionMat.rows; i++)
		{
			float confidence = detectionMat.at<float>(i, 2);
			if (confidence > confidenceThreshold)
			{
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

void CDetectFace::LoadModel(const string& config_file, const string& weight_file)
{
#ifdef __APPLE__
#else
#ifdef CAFFE
	const std::string caffeConfigFile = config_file;//"C:\\developpement\\git_gcc\\Rotnet\\Rotnetcpp\\model\\deploy.prototxt";
	const std::string caffeWeightFile = weight_file;// "C:\\developpement\\git_gcc\\Rotnet\\Rotnetcpp\\model\\res10_300x300_ssd_iter_140000_fp16.caffemodel";
#else
	const std::string tensorflowConfigFile = config_file;
	// "C:\\developpement\\git_gcc\\Rotnet\\Rotnetcpp\\model\\opencv_face_detector.pbtxt";
	const std::string tensorflowWeightFile = weight_file;
	// "C:\\developpement\\git_gcc\\Rotnet\\Rotnetcpp\\model\\opencv_face_detector_uint8.pb";
#endif

	try
	{
		/*
			"{ backend     |  0 | Choose one of computation backends: "
						 "0: automatically (by default), "
						 "1: Halide language (http://halide-lang.org/), "
						 "2: Intel's Deep Learning Inference Engine (https://software.intel.com/openvino-toolkit), "
						 "3: OpenCV implementation }"
			"{ target      | 0 | Choose one of target computation devices: "
						 "0: CPU target (by default), "
						 "1: OpenCL, "
						 "2: OpenCL fp16 (half-float precision), "
						 "3: VPU }"

		*/

		/*
		DNN_TARGET_CPU = 0,
		DNN_TARGET_OPENCL,
		DNN_TARGET_OPENCL_FP16,
		DNN_TARGET_MYRIAD,
		DNN_TARGET_VULKAN,
		DNN_TARGET_FPGA,  //!< FPGA device with CPU fallbacks using Inference Engine's Heterogeneous plugin.
		DNN_TARGET_CUDA,
		DNN_TARGET_CUDA_FP16
		*/

#ifndef __WXGTK__
		bool openCLCompatible = false;
		CRegardsConfigParam* config = CParamInit::getInstance();
		if (config != nullptr)
		{
			if (config->GetIsOpenCLSupport())
				openCLCompatible = true;
		}

#ifdef CAFFE
		net = cv::dnn::readNetFromCaffe(caffeConfigFile, caffeWeightFile);
#else
		net = readNetFromTensorflow(tensorflowWeightFile, tensorflowConfigFile);
#endif
		net.setPreferableBackend(DNN_BACKEND_DEFAULT);
		if (openCLCompatible)
			net.setPreferableTarget(DNN_TARGET_OPENCL);
		else
			net.setPreferableTarget(DNN_TARGET_CPU);

#else
#ifdef CAFFE
		net = cv::dnn::readNetFromCaffe(caffeConfigFile, caffeWeightFile);
#else
		net = cv::dnn::readNetFromTensorflow(tensorflowWeightFile, tensorflowConfigFile);
#endif
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


