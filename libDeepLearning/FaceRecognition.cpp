#include "header.h"
#include "FaceRecognition.h"
#include <SqlFacePhoto.h>
#include <opencv2/face.hpp>
#include <SqlFaceRecognition.h>
#include <SqlFaceDescriptor.h>
#include <SqlFindFacePhoto.h>
#include <SqlFaceLabel.h>
#include "base64.h"
#include <RegardsBitmap.h>
#include <FileUtility.h>
#include <dlib/dnn.h>
#include <dlib/clustering.h>
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
#ifdef WHISPER_CHINESE
	CSqlFaceLabel sqlfaceLabel;
	CSqlFaceRecognition sqlfaceRecognition;
	bool findFaceCompatible = false;
	CSqlFaceDescriptor sqlFaceDescriptor;
	CSqlFindFacePhoto sqlfindFacePhoto;
	std::vector<CFaceDescriptor*> face_descriptors = sqlfindFacePhoto.GetAllFaceDescriptor();

	// In particular, one simple thing we can do is face clustering.  This next bit of code
	// creates a graph of connected faces and then uses the Chinese whispers graph clustering
	// algorithm to identify how many people there are and which faces belong to whom.
	std::vector<sample_pair> edges;
	for (size_t i = 0; i < face_descriptors.size(); ++i)
	{
		for (size_t j = i; j < face_descriptors.size(); ++j)
		{
			// Faces are connected in the graph if they are close enough.  Here we check if
			// the distance between two face descriptors is less than 0.6, which is the
			// decision threshold the network was trained to use.  Although you can
			// certainly use any other threshold you find useful.
			if (IsCompatibleFace(face_descriptors[i]->descriptor,face_descriptors[j]->descriptor) < 0.6)
				edges.push_back(sample_pair(i, j));
		}
	}
	std::vector<unsigned long> labels;
	const auto num_clusters = chinese_whispers(edges, labels);

	
	sqlfaceRecognition.DeleteFaceRecognitionDatabase();

	for (size_t j = 0; j < labels.size(); ++j)
	{
		sqlfaceRecognition.InsertFaceRecognition(face_descriptors[j]->numFace, face_descriptors[labels[j]]->numFace);
	}
	
	sqlfaceLabel.DeleteFaceLabelDatabase();

	for (size_t cluster_id = 0; cluster_id < num_clusters; ++cluster_id)
	{
		for (size_t j = 0; j < labels.size(); ++j)
		{
			if (cluster_id == labels[j])
			{
				wxString label = "Face number " + to_string(face_descriptors[labels[j]]->numFace);
				sqlfaceLabel.InsertFaceLabel(face_descriptors[labels[j]]->numFace, label, true);
			}
		}
	}
		
	return true;
#else
	bool findFaceCompatible = false;
	CSqlFaceDescriptor sqlFaceDescriptor;
	CFaceDescriptor* faceDescriptor;
	faceDescriptor = sqlFaceDescriptor.GetFaceDescriptor(numFace);

	if (faceDescriptor != nullptr)
	{
		CSqlFaceLabel sqlfaceLabel;
		CSqlFaceRecognition sqlfaceRecognition;
		CSqlFindFacePhoto sqlfindFacePhoto;
		std::vector<CFaceDescriptor*> listFace = sqlfindFacePhoto.GetUniqueFaceDescriptor(faceDescriptor->numFace);
		//std::vector<facedetectlength> listDataLength;
		//listDataLength.resize(listFace.size() + 1);
		float lenghmin = 0.6;
		int numFace = 0;

		for (int i = 0; i < listFace.size(); i++)
		{
			//Find best Compatible Face
			if (listFace[i]->numFace != faceDescriptor->numFace)
			{
				float lengthDiff = IsCompatibleFace(faceDescriptor->descriptor, listFace[i]->descriptor);
				if (lenghmin > lengthDiff)
				{
					numFace = listFace[i]->numFace;
					lenghmin = lengthDiff;
				}
			}

		}

		if (lenghmin < 0.6)
		{
			sqlfaceRecognition.InsertFaceRecognition(faceDescriptor->numFace, numFace);
			findFaceCompatible = true;
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
#endif
	
}

// Compare two images by getting the L2 error (square-root of sum of squared error).
double CFaceRecognition::GetSimilarity(const cv::Mat &A, const cv::Mat &B)
{
	if (A.rows > 0 && A.rows == B.rows && A.cols > 0 && A.cols == B.cols) {
		// Calculate the L2 relative error between the 2 images.
		double errorL2 = norm(A, B, NORM_L2);
		// Convert to a reasonable scale, since L2 error is summed across all pixels of the image.
		double similarity = errorL2 / (double)(A.rows * A.cols);
		return similarity;
	}
	else {
		//cout << "WARNING: Images have a different size in 'getSimilarity()'." << endl;
		return 0;  // Return a bad value
	}
}

int CFaceRecognition::FaceRecognition(const int& numFace)
{
	int predictedLabel = -1;
	double confidence = 0.0;
	double maxConfidence = 0.0;

	bool findFaceCompatible = false;
	CSqlFacePhoto facePhoto;
	CSqlFaceRecognition sqlfaceRecognition;
	std::vector<CFaceRecognitionData> faceRecognitonVec = facePhoto.GetAllNumFaceRecognition();
	Mat imageSrc = imread(CFileUtility::GetFaceThumbnailPath(numFace).ToStdString(), IMREAD_GRAYSCALE);
	equalizeHist(imageSrc, imageSrc);
	imageSrc = norm_0_255(imageSrc);
	if (faceRecognitonVec.size() > 1)
	{
		for (CFaceRecognitionData picture : faceRecognitonVec)
		{
			Mat image = imread(CFileUtility::GetFaceThumbnailPath(picture.numFace).ToStdString(), IMREAD_GRAYSCALE);
			equalizeHist(image, image);
			image = norm_0_255(image);
			confidence = GetSimilarity(imageSrc, image);
			if (maxConfidence < confidence)
			{
				predictedLabel = picture.numFaceCompatible;
				maxConfidence = confidence;
			}
				
		}


		if (maxConfidence > 0.6)
		{
			sqlfaceRecognition.InsertFaceRecognition(numFace, predictedLabel);
			findFaceCompatible = true;
		}

		if (!findFaceCompatible)
		{
			CSqlFaceLabel sqlfaceLabel;
			wxString label = "Face number " + to_string(numFace);
			sqlfaceRecognition.InsertFaceRecognition(numFace, numFace);
			sqlfaceLabel.InsertFaceLabel(numFace, label, true);
		}
	}
	else
	{
		CSqlFaceLabel sqlfaceLabel;
		wxString label = "Face number " + to_string(numFace);
		sqlfaceRecognition.InsertFaceRecognition(numFace, numFace);
		sqlfaceLabel.InsertFaceLabel(numFace, label, true);
	}


	return findFaceCompatible;
}


int CFaceRecognition::FaceRecognitionEigen(const int &numFace)
{
	int predictedLabel = -1;
	double confidence = 0.0;
	bool findFaceCompatible = false;
	CSqlFacePhoto facePhoto;
	std::vector<Mat> images;
	std::vector<int> labels;
	CSqlFaceRecognition sqlfaceRecognition;
	std::vector<CFaceRecognitionData> faceRecognitonVec = facePhoto.GetAllNumFaceRecognition();
	Mat imageSrc = imread(CFileUtility::GetFaceThumbnailPath(numFace).ToStdString(), IMREAD_GRAYSCALE);

	if (faceRecognitonVec.size() > 1)
	{


		for (CFaceRecognitionData picture : faceRecognitonVec)
		{
			Mat image = imread(CFileUtility::GetFaceThumbnailPath(picture.numFace).ToStdString(), IMREAD_GRAYSCALE);
			images.push_back(norm_0_255(image));
			labels.push_back(picture.numFaceCompatible);
		}

		// The following line predicts the label of a given
		// test image:
		int predictedLabel = -1;

		try
		{
			images.pop_back();
			labels.pop_back();
			Ptr<EigenFaceRecognizer> model = EigenFaceRecognizer::create();
			model->train(images, labels);
			model->predict(imageSrc, predictedLabel, confidence);
		}
		catch (cv::Exception& e)
		{
			const char* err_msg = e.what();
			std::cout << "exception caught: " << err_msg << std::endl;
			std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
		}

		if (confidence < 0.6)
		{
			sqlfaceRecognition.InsertFaceRecognition(numFace, predictedLabel);
			findFaceCompatible = true;
		}

		if (!findFaceCompatible)
		{
			CSqlFaceLabel sqlfaceLabel;
			wxString label = "Face number " + to_string(numFace);
			sqlfaceRecognition.InsertFaceRecognition(numFace, numFace);
			sqlfaceLabel.InsertFaceLabel(numFace, label, true);
		}
	}
	else
	{
		CSqlFaceLabel sqlfaceLabel;
		wxString label = "Face number " + to_string(numFace);
		sqlfaceRecognition.InsertFaceRecognition(numFace, numFace);
		sqlfaceLabel.InsertFaceLabel(numFace, label, true);
	}


	return findFaceCompatible;
}


