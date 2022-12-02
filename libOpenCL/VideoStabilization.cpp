#include "header.h"
#include "VideoStabilization.h"


using namespace Regards::OpenCV;
using namespace cv;

const int SMOOTHING_RADIUS = 50; // In frames. The larger the more stable the video, but less reactive to sudden panning


class COpenCVStabilizationPimpl_
{
public:
	struct TransformParam
	{
		TransformParam(): dx(0), dy(0), da(0)
		{
		}

		TransformParam(double _dx, double _dy, double _da)
		{
			dx = _dx;
			dy = _dy;
			da = _da;
		}

		double dx;
		double dy;
		double da; // angle

		void getTransform(Mat& T)
		{
			// Reconstruct transformation matrix accordingly to new values
			T.at<double>(0, 0) = cos(da);
			T.at<double>(0, 1) = -sin(da);
			T.at<double>(1, 0) = sin(da);
			T.at<double>(1, 1) = cos(da);

			T.at<double>(0, 2) = dx;
			T.at<double>(1, 2) = dy;
		}
	};

	struct Trajectory
	{
		Trajectory(): x(0), y(0), a(0)
		{
		}

		Trajectory(double _x, double _y, double _a)
		{
			x = _x;
			y = _y;
			a = _a;
		}

		double x;
		double y;
		double a; // angle
	};

	void fixBorder(Mat& frame_stabilized)
	{
		Mat T = getRotationMatrix2D(Point2f(frame_stabilized.cols / 2, frame_stabilized.rows / 2), 0, 1.04);
		warpAffine(frame_stabilized, frame_stabilized, T, frame_stabilized.size());
	}

	void fixBorder(UMat& frame_stabilized)
	{
		Mat T = getRotationMatrix2D(Point2f(frame_stabilized.cols / 2, frame_stabilized.rows / 2), 0, 1.04);
		warpAffine(frame_stabilized, frame_stabilized, T, frame_stabilized.size());
	}

	vector<Trajectory> cumsum(vector<TransformParam>& transforms)
	{
		vector<Trajectory> trajectory; // trajectory at all frames
		// Accumulated frame to frame transform
		double a = 0;
		double x = 0;
		double y = 0;

		for (size_t i = 0; i < transforms.size(); i++)
		{
			x += transforms[i].dx;
			y += transforms[i].dy;
			a += transforms[i].da;

			trajectory.push_back(Trajectory(x, y, a));
		}

		return trajectory;
	}

	vector<Trajectory> smooth(vector<Trajectory>& trajectory, int radius)
	{
		vector<Trajectory> smoothed_trajectory;
		for (size_t i = 0; i < trajectory.size(); i++)
		{
			double sum_x = 0;
			double sum_y = 0;
			double sum_a = 0;
			int count = 0;

			for (int j = -radius; j <= radius; j++)
			{
				if (i + j >= 0 && i + j < trajectory.size())
				{
					sum_x += trajectory[i + j].x;
					sum_y += trajectory[i + j].y;
					sum_a += trajectory[i + j].a;

					count++;
				}
			}

			double avg_a = sum_a / count;
			double avg_x = sum_x / count;
			double avg_y = sum_y / count;

			smoothed_trajectory.push_back(Trajectory(avg_x, avg_y, avg_a));
		}

		return smoothed_trajectory;
	}

	void CalculTransformation()
	{
		transforms_smooth.clear();

		// Compute trajectory using cumulative sum of transformations
		vector<Trajectory> trajectory = cumsum(transforms);

		// Smooth trajectory using moving average filter
		vector<Trajectory> smoothed_trajectory = smooth(trajectory, SMOOTHING_RADIUS);


		for (size_t i = 0; i < transforms.size(); i++)
		{
			// Calculate difference in smoothed_trajectory and trajectory
			double diff_x = smoothed_trajectory[i].x - trajectory[i].x;
			double diff_y = smoothed_trajectory[i].y - trajectory[i].y;
			double diff_a = smoothed_trajectory[i].a - trajectory[i].a;

			// Calculate newer transformation array
			double dx = transforms[i].dx + diff_x;
			double dy = transforms[i].dy + diff_y;
			double da = transforms[i].da + diff_a;

			transforms_smooth.push_back(TransformParam(dx, dy, da));
		}
	}

	void AnalyseFrame(const UMat& curr)
	{
		if (first)
		{
			first = false;
			cvtColor(curr, prev_gray, COLOR_BGR2GRAY);
			return;
		}
		UMat curr_gray;
		// Vector from previous and current feature points
		vector<Point2f> curr_pts;

		// Detect features in previous frame

		goodFeaturesToTrack(prev_gray, prev_pts, 200, 0.01, 30);

		// Convert to grayscale
		cvtColor(curr, curr_gray, COLOR_BGR2GRAY);

		// Calculate optical flow (i.e. track feature points)
		vector<uchar> status;
		UMat err;

		try
		{
			calcOpticalFlowPyrLK(prev_gray, curr_gray, prev_pts, curr_pts, status, err);
		}
		catch (...)
		{
		}

		// Filter only valid points
		auto prev_it = prev_pts.begin();
		auto curr_it = curr_pts.begin();
		for (size_t k = 0; k < status.size(); k++)
		{
			if (status[k])
			{
				++prev_it;
				++curr_it;
			}
			else
			{
				prev_it = prev_pts.erase(prev_it);
				curr_it = curr_pts.erase(curr_it);
			}
		}


		// Find transformation matrix
		Mat T = estimateAffinePartial2D(prev_pts, curr_pts);

		// In rare cases no transform is found. 
		// We'll just use the last known good transform.
		//if (T.data == NULL) 
		//	last_T.copyTo(T);
		//T.copyTo(last_T);

		// Extract traslation
		double dx = T.at<double>(0, 2);
		double dy = T.at<double>(1, 2);

		// Extract rotation angle
		double da = atan2(T.at<double>(1, 0), T.at<double>(0, 0));

		while (transforms.size() > nbFrameMax)
			transforms.erase(transforms.begin());

		// Store transformation 
		if (transforms.size() == nbFrameMax)
			transforms.erase(transforms.begin());
		transforms.push_back(TransformParam(dx, dy, da));

		// Move to next frame
		curr_gray.copyTo(prev_gray);
	}

	void AnalyseFrame(const Mat& curr)
	{
		if (first)
		{
			first = false;
			cvtColor(curr, prev_gray, COLOR_BGR2GRAY);
			return;
		}
		UMat curr_gray;
		// Vector from previous and current feature points
		vector<Point2f> curr_pts;

		// Detect features in previous frame

		goodFeaturesToTrack(prev_gray, prev_pts, 200, 0.01, 30);

		// Convert to grayscale
		cvtColor(curr, curr_gray, COLOR_BGR2GRAY);

		// Calculate optical flow (i.e. track feature points)
		vector<uchar> status;
		UMat err;

		try
		{
			calcOpticalFlowPyrLK(prev_gray, curr_gray, prev_pts, curr_pts, status, err);
		}
		catch (...)
		{
		}

		// Filter only valid points
		auto prev_it = prev_pts.begin();
		auto curr_it = curr_pts.begin();
		for (size_t k = 0; k < status.size(); k++)
		{
			if (status[k])
			{
				++prev_it;
				++curr_it;
			}
			else
			{
				prev_it = prev_pts.erase(prev_it);
				curr_it = curr_pts.erase(curr_it);
			}
		}


		// Find transformation matrix
		Mat T = estimateAffinePartial2D(prev_pts, curr_pts);

		// In rare cases no transform is found. 
		// We'll just use the last known good transform.
		//if (T.data == NULL) 
		//	last_T.copyTo(T);
		//T.copyTo(last_T);

		// Extract traslation
		double dx = T.at<double>(0, 2);
		double dy = T.at<double>(1, 2);

		// Extract rotation angle
		double da = atan2(T.at<double>(1, 0), T.at<double>(0, 0));

		while (transforms.size() > nbFrameMax)
			transforms.erase(transforms.begin());

		// Store transformation 
		if (transforms.size() == nbFrameMax)
			transforms.erase(transforms.begin());
		transforms.push_back(TransformParam(dx, dy, da));

		// Move to next frame
		curr_gray.copyTo(prev_gray);
	}

	UMat CorrectedFrame(UMat& frame)
	{
		Mat T(2, 3, CV_64F);
		Mat frame_stabilized;

		if (transforms_smooth.size() > 0)
		{
			int i = transforms_smooth.size() - 1;

			// Extract transform from translation and rotation angle. 
			transforms_smooth[i].getTransform(T);

			// Apply affine wrapping to the given frame
			warpAffine(frame, frame_stabilized, T, frame.size());

			// Scale image to remove black border artifact
			fixBorder(frame_stabilized);

			frame_stabilized.copyTo(frame);
		}
		//frame.copyTo(frame_stabilized);
		return frame;
	}

	void CorrectedFrame(Mat& frame)
	{
		Mat T(2, 3, CV_64F);

		if (transforms_smooth.size() > 0)
		{
			Mat frame_stabilized;

			int i = transforms_smooth.size() - 1;

			// Extract transform from translation and rotation angle. 
			transforms_smooth[i].getTransform(T);

			// Apply affine wrapping to the given frame
			warpAffine(frame, frame_stabilized, T, frame.size());

			// Scale image to remove black border artifact
			fixBorder(frame_stabilized);

			frame_stabilized.copyTo(frame);
		}
	}

	void Init()
	{
		first = true;
		transforms.clear();
		transforms_smooth.clear();
		//prev_gray.deallocate();
		//prev_pts.clear();
	}

	// Define variable for storing frames
	//cv::UMat curr_gray;
	UMat prev_gray;
	vector<Point2f> prev_pts;
	vector<TransformParam> transforms;
	vector<TransformParam> transforms_smooth;

	bool first = true;
	int nbFrameMax = 60;
	//std::map<int, cv::Mat> picture_stabilization;
};

COpenCVStabilization::COpenCVStabilization(const int& nbFrame)
{
	pimpl = new COpenCVStabilizationPimpl_();
	this->nbFrame = nbFrame;
	pimpl->nbFrameMax = nbFrame;
}

COpenCVStabilization::~COpenCVStabilization()
{
	delete pimpl;
}

void COpenCVStabilization::SetNbFrameBuffer(const int& nbFrame)
{
	pimpl->nbFrameMax = nbFrame;
}

int COpenCVStabilization::GetNbFrame()
{
	return nbFrame;
}

void COpenCVStabilization::Init()
{
	nbFrameBuffer = 0;
	pimpl->Init();
}

int COpenCVStabilization::GetNbFrameBuffer()
{
	return nbFrameBuffer;
}

void COpenCVStabilization::AddFrame(const Mat& image)
{
	pimpl->AnalyseFrame(image);
}

void COpenCVStabilization::BufferFrame(const Mat& image)
{
	pimpl->AnalyseFrame(image);
	nbFrameBuffer++;
}

void COpenCVStabilization::AddFrame(const UMat& image)
{
	pimpl->AnalyseFrame(image);
}

void COpenCVStabilization::BufferFrame(const UMat& image)
{
	pimpl->AnalyseFrame(image);
	nbFrameBuffer++;
}

void COpenCVStabilization::CorrectFrame(Mat& image)
{
	pimpl->CalculTransformation();
	pimpl->CorrectedFrame(image);
}

UMat COpenCVStabilization::CorrectFrame(UMat& image)
{
	//cv::Mat frame;
	//image.copyTo(frame);
	pimpl->CalculTransformation();
	return pimpl->CorrectedFrame(image);
	//frame.copyTo(frame);
}
