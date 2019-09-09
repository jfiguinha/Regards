#include "FaceDetector.h"
#include "base64.h"
using namespace dlib;



// ----------------------------------------------------------------------------------------

std::vector<matrix<rgb_pixel>> jitter_image(
	const matrix<rgb_pixel>& img
)
{
	// All this function does is make 100 copies of img, all slightly jittered by being
	// zoomed, rotated, and translated a little bit differently. They are also randomly
	// mirrored left to right.
	thread_local dlib::rand rnd;

	std::vector<matrix<rgb_pixel>> crops;
	for (int i = 0; i < 100; ++i)
		crops.push_back(jitter_image(img, rnd));

	return crops;
}


CFaceDetector::CFaceDetector()
{
}


CFaceDetector::~CFaceDetector()
{
}

int CFaceDetector::DetectFace(array2d<rgb_pixel> & img, const int &quality, CFaceData * faceData)
{
	faces.clear();
	// The first thing we are going to do is load all our models.  First, since we need to
	// find faces in the image we will need a face detector:
	frontal_face_detector detector = get_frontal_face_detector();

	//pyramid_up(img);

	// Now tell the face detector to give us a list of bounding boxes
	// around all the faces it can find in the image.
	std::vector<rect_detection> dets = detector(img);

	std::vector<rectangle> final_dets;
	for (unsigned long i = 0; i < dets.size(); ++i)
	{
		face_pertinence.push_back(dets[i].detection_confidence);
		final_dets.push_back(dets[i].rect);
	}


	for (auto face : final_dets)
	{
		auto shape = faceData->sp(img, face);
		matrix<rgb_pixel> face_chip;
		extract_image_chip(img, get_face_chip_details(shape, 150, 0.25), face_chip);
		faces.push_back(move(face_chip));
	}

	if (faces.size() == 0)
	{
		faces.clear();
		//cout << "No faces found in image!" << endl;
		return 0;
	}
	else
	{
		if (quality == 1)
		{
			for (auto i = 0; i < faces.size(); i++)
			{
				matrix<float, 0, 1> face_descriptor = mean(mat(faceData->net(jitter_image(faces[i]))));
				cout << "jittered face descriptor for one face: " << trans(face_descriptor) << endl;
				face_descriptors.push_back(face_descriptor);
			}
		}
		else
		{
			try
			{
				// This call asks the DNN to convert each face image in faces into a 128D vector.
				// In this 128D vector space, images from the same person will be close to each other
				// but vectors from different people will be far apart.  So we can use these vectors to
				// identify if a pair of images are from the same person or from different people.  
				face_descriptors = faceData->net(faces);
			}
			catch(...)
			{
				faces.clear();
				return 0;
			}
		}
	}

	return faces.size();
}


float CFaceDetector::IsCompatibleFace(std::string const& dataface1, std::string const& dataface2)
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

int CFaceDetector::GetNbFaceFound()
{
	return faces.size();
}

std::string CFaceDetector::GetFaceDescriptor(const int &numFace)
{
	if (numFace >= faces.size() || numFace >= face_descriptors.size())
		return "";

	matrix<float, 0, 1> face = face_descriptors[numFace];
	ostringstream sout;
	serialize(face, sout);

	return base64_encode(reinterpret_cast<const unsigned char*>(sout.str().c_str()), sout.str().size());
}

void CFaceDetector::SaveFaceData(const int &numFace, long &width, long &height, double &pertinence, const char * savePath)
{
	if (numFace >= faces.size())
		return;

	matrix<rgb_pixel> face_chip = faces.at(numFace);

	height = face_chip.nr();
	width = face_chip.nc();
	pertinence = face_pertinence[numFace];
	dlib::save_jpeg(face_chip, savePath);
}

void CFaceDetector::GetFaceData(const int &numFace, long &width, long &height, double &pertinence, uint8_t * data, long &size)
{
	if (numFace >= faces.size())
		return;

	matrix<rgb_pixel> face_chip = faces.at(numFace);
	pertinence = face_pertinence[numFace];
	long _height = face_chip.nr();
	long _width = face_chip.nc();
	long image_size = _width * _height;
	if (size == 0)
	{
		size = image_size * 4;
	}
	else if(size == (image_size * 4))
	{
		
		for (auto i = 0, j = 0; i < image_size; ++i, j+=4)
		{
			unsigned r = std::floor(i / _width);
			unsigned c = i % _width;
			
			rgb_pixel& p = face_chip(r,c);

			data[j] = p.red;
			data[j + 1] = p.green;
			data[j + 2] = p.blue;
		}

		width = _width;
		height = _height;
	}
}