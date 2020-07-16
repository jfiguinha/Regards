#include "header.h"
#include "FaceRecognition.h"
#include <SqlFacePhoto.h>
#include <PictureData.h>
#include <opencv2/face.hpp>
#include <SqlFaceRecognition.h>
#include <SqlFaceDescriptor.h>
#include <SqlFindFacePhoto.h>
#include <SqlFaceLabel.h>
#include "base64.h"
#include <dlib/dnn.h>
using namespace dlib;
using namespace cv::face;
using namespace cv;
using namespace std;
using namespace cv::dnn;
using namespace Regards::OpenCV;
using namespace Regards::Sqlite;


CFaceRecognition::CFaceRecognition()
{
}

CFaceRecognition::~CFaceRecognition()
{
}

static Mat norm_0_255(InputArray _src) {
	Mat src = _src.getMat();
	// Create and return normalized image:
	Mat dst;
	switch (src.channels()) {
	case 1:
		cv::normalize(_src, dst, 0, 255, NORM_MINMAX, CV_8UC1);
		break;
	case 3:
		cv::normalize(_src, dst, 0, 255, NORM_MINMAX, CV_8UC3);
		break;
	default:
		src.copyTo(dst);
		break;
	}
	return dst;
}

float CFaceRecognition::IsCompatibleFace(std::string const& dataface1, std::string const& dataface2)
{
	std::stringstream ss1;
	matrix<float, 0, 1> face1;
	ss1.str(base64_decode(dataface1));
	deserialize(face1, ss1);

	std::stringstream ss2;
	matrix<float, 0, 1> face2;
	ss2.str(base64_decode(dataface2));
	deserialize(face2, ss2);
	// Faces are connected in the graph if they are close enough.  Here we check if
	// the distance between two face descriptors is less than 0.6, which is the
	// decision threshold the network was trained to use.  Although you can
	// certainly use any other threshold you find useful.

	return (length(face1 - face2));
}

//---------------------------------------------------------------------------------------
//Test si compatible avec une autre Face à déplacer ailleurs
//---------------------------------------------------------------------------------------	
bool CFaceRecognition::FindCompatibility(const int &numFace)
{
	bool findFaceCompatible = false;
	CSqlFaceDescriptor sqlFaceDescriptor;
	CFaceDescriptor * faceDescriptor;
	faceDescriptor = sqlFaceDescriptor.GetFaceDescriptor(numFace);

	if (faceDescriptor != nullptr)
	{
		CSqlFaceLabel sqlfaceLabel;
		
		CSqlFaceRecognition sqlfaceRecognition;
		CSqlFindFacePhoto sqlfindFacePhoto;

		std::vector<CFaceDescriptor*> listFace = sqlfindFacePhoto.GetUniqueFaceDescriptor(faceDescriptor->numFace);
		for (int i = 0; i < listFace.size(); i++)
		{

			CFaceDescriptor * facede = listFace[i];
			float isCompatible = IsCompatibleFace(faceDescriptor->descriptor, facede->descriptor);
			if (isCompatible < 0.6)
			{
				sqlfaceRecognition.InsertFaceRecognition(faceDescriptor->numFace, facede->numFace);
				findFaceCompatible = true;
				break;
			}
		}

		for (int i = 0; i < listFace.size(); i++)
		{
			CFaceDescriptor * facede = listFace[i];
			delete facede;
		}

		if (!findFaceCompatible)
		{
			wxString label = "Face number " + to_string(faceDescriptor->numFace);
			sqlfaceRecognition.InsertFaceRecognition(faceDescriptor->numFace, faceDescriptor->numFace);
			sqlfaceLabel.InsertFaceLabel(faceDescriptor->numFace, label, true);
		}

		if (faceDescriptor != nullptr)
			delete faceDescriptor;
	}
	return findFaceCompatible;
}

int CFaceRecognition::FaceRecognition(CPictureData * pictureData, const int &numFace)
{
	int predictedLabel = 0;
	CSqlFacePhoto facePhoto;
	std::vector<CPictureData *> listPicture = facePhoto.GetAllFace(numFace);
	std::vector<int> labels = facePhoto.GetAllNumFace(numFace);
	CSqlFaceRecognition sqlfaceRecognition;
	CSqlFaceLabel sqlfaceLabel;
	wxString label = "Face number " + to_string(numFace);
	cv::Size size(224, 224);//the dst image size,e.g.100x100

	if (listPicture.size() > 1)
	{
		// These vectors hold the images and corresponding labels.
		std::vector<Mat> images;

		for (CPictureData * picture : listPicture)
		{
			std::vector<char> data = picture->CopyData();
			cv::Mat image = imdecode(Mat(data), 1);
			cv::Mat dst;//dst image
			cv::cvtColor(image, dst, cv::COLOR_BGR2GRAY);
			cv::resize(dst, image, size);
			images.push_back(image);
		}

		std::vector<char> data = pictureData->CopyData();
		cv::Mat testSample = imdecode(Mat(data), 1);

		
		// Quit if there are not enough images for this demo.
		if (images.size() <= 1) {
			string error_message = "This demo needs at least 2 images to work. Please add more images to your data set!";
			CV_Error(Error::StsError, error_message);
		}
		

		// The following line predicts the label of a given
		// test image:
		int predictedLabel = -1;

		try
		{
			/*
			// Load Recognizer
			model->load("../model.xml");
			if (model.empty()) {
				return -1;
			}
			*/

			cv::Mat src;
			cv::resize(testSample, src, size);
			cv::cvtColor(src, testSample, cv::COLOR_BGR2GRAY);

			Ptr<EigenFaceRecognizer> model = EigenFaceRecognizer::create();
			model->train(images, labels);
			predictedLabel = model->predict(testSample);

			//std::cout << "Save Recognizer" << std::endl;
			//model->save("../model.xml");
		}
		catch (cv::Exception& e)
		{
			const char* err_msg = e.what();
			std::cout << "exception caught: " << err_msg << std::endl;
			std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
		}

		for (CPictureData * picture : listPicture)
		{
			delete picture;
		}
		listPicture.clear();

		if (predictedLabel == 0)
		{
			sqlfaceRecognition.InsertFaceRecognition(numFace, numFace);
			predictedLabel = sqlfaceLabel.InsertFaceLabel(numFace, label, true);
		}
		else
		{
			sqlfaceRecognition.InsertFaceRecognition(predictedLabel, numFace);
		}
	}
	else
	{
		sqlfaceRecognition.InsertFaceRecognition(numFace, numFace);
		predictedLabel = sqlfaceLabel.InsertFaceLabel(numFace, label, true);
	}


	return predictedLabel;
}

