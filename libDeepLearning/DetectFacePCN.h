#pragma once
#include "FaceRect.h"
using namespace std;
using namespace cv;

class CRegardsBitmap;


class CDetectFacePCN
{
public:
	CDetectFacePCN(void);
	~CDetectFacePCN(void);

	void LoadModel(const bool& openCLCompatible, const bool& cudaCompatible);
	void DetectFace(const cv::Mat& source, std::vector<CFace>& listOfFace, std::vector<cv::Rect>& pointOfFace);
	int DetectFaceAngle(const cv::Mat& frameOpenCVDNN);

private:
	bool xyValid(int _x, int _y, cv::Mat _img);
	cv::Mat preprocessImg(cv::Mat _img);
	cv::Mat preprocessImg(cv::Mat _img, int _dim);
	cv::Mat resizeImg(cv::Mat _img, float _scale);
	cv::Mat padImg(cv::Mat _img);
	static bool compareFaceByScore(const FaceBox& box1, const FaceBox& box2);
	float IoU(FaceBox& box1, FaceBox& box2);
	std::vector<FaceBox> NMS(std::vector<FaceBox>& _faces, bool _local, float _threshold);
	std::vector<FaceBox> TransformBoxes(cv::Mat _img, cv::Mat _imgPad, std::vector<FaceBox>& _faces);
	std::vector<FaceBox> PCN_1(cv::Mat _img, cv::Mat _paddedImg, cv::dnn::Net _net, float _thresh, int _minFaceSize);
	std::vector<FaceBox> PCN_2(cv::Mat _img, cv::Mat _img180, cv::dnn::Net _net, float _threshold, int _dim,
	                           std::vector<FaceBox> _faces);
	std::vector<FaceBox> PCN_3(cv::Mat _img, cv::Mat _img180, cv::Mat _img90, cv::Mat _imgNeg90, cv::dnn::Net _net,
	                           float _threshold, int _dim, std::vector<FaceBox> _faces);
	cv::Point RotatePoint(int x, int y, float centerX, float centerY, float angle);
	void DrawLine(cv::Mat img, std::vector<cv::Point> pointList);
	void DrawFace(cv::Mat img, FaceBox face);

	static bool isload;
};
