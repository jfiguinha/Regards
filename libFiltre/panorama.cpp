#include <header.h>
#include "panorama.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/stitching.hpp>
using namespace std;
using namespace Regards::OpenCV;
using namespace cv;
using namespace xfeatures2d;

//1、 Use feature detection algorithm to find similar points in two images , Calculate the transformation matrix 
//2、 The image right Pictures and images obtained after perspective transformation left Splicing 
class CPanoramaPimp
{
public:
	CPanoramaPimp()
	{
	};

	~CPanoramaPimp()
	{
	};

	bool Image_Stitching(const Mat& image_left, const Mat& image_right, Mat& H, Mat& WarpImg, Mat& DstImg, bool draw)
	{
		// establish SURF Characteristic detector 
		int Hessian = 800;
		Ptr<SURF> detector = SURF::create(Hessian);

		// Carry out image feature detection 、 Feature description 
		vector<KeyPoint> keypoint_left, keypoint_right;
		Mat descriptor_left, descriptor_right;
		detector->detectAndCompute(image_left, Mat(), keypoint_left, descriptor_left);
		detector->detectAndCompute(image_right, Mat(), keypoint_right, descriptor_right);

		// Use FLANN The algorithm matches the feature descriptors 
		FlannBasedMatcher matcher;
		vector<DMatch> matches;
		matcher.match(descriptor_left, descriptor_right, matches);

		double Max = 0.0;
		for (int i = 0; i < matches.size(); i++)
		{
			//float distance –> Represents this pair of matched feature point descriptors （ The essence is a vector ） Euclidean distance of , The smaller the value, the more similar the two feature points are .
			double dis = matches[i].distance;
			if (dis > Max)
			{
				Max = dis;
			}
		}

		// Filter out the key points with high matching degree 
		vector<DMatch> goodmatches;
		vector<Point2f> goodkeypoint_left, goodkeypoint_right;
		for (int i = 0; i < matches.size(); i++)
		{
			double dis = matches[i].distance;
			if (dis < 0.15 * Max)
			{
				/*  Make perspective transformation with the right figure   On the left ->queryIdx: Query point index （ Query image ）  Right picture ->trainIdx: The index of the queried point （ Target image ） */
				// notes ： Yes image_right Perspective transformation of the image , so goodkeypoint_left Corresponding queryIdx,goodkeypoint_right Corresponding trainIdx
				//int queryIdx –> Is the feature point descriptor of the test image （descriptor） The subscript , It is also the feature point corresponding to the descriptor （keypoint) The subscript .
				goodkeypoint_left.push_back(keypoint_left[matches[i].queryIdx].pt);
				//int trainIdx –>  Is the subscript of the feature point descriptor of the sample image , It is also the subscript of the corresponding feature point .
				goodkeypoint_right.push_back(keypoint_right[matches[i].trainIdx].pt);
				goodmatches.push_back(matches[i]);
			}
		}

		// Draw feature points 
		if (draw)
		{
			Mat result;
			drawMatches(image_left, keypoint_left, image_right, keypoint_right, goodmatches, result);
			//imshow(" Feature matching ", result);

			Mat temp_left = image_left.clone();
			for (int i = 0; i < goodkeypoint_left.size(); i++)
			{
				circle(temp_left, goodkeypoint_left[i], 3, Scalar(0, 255, 0), -1);
			}
			//imshow("goodkeypoint_left", temp_left);

			Mat temp_right = image_right.clone();
			for (int i = 0; i < goodkeypoint_right.size(); i++)
			{
				circle(temp_right, goodkeypoint_right[i], 3, Scalar(0, 255, 0), -1);
			}
			//imshow("goodkeypoint_right", temp_right);
		}

		//findHomography Calculating the homography matrix requires at least 4 A little bit 
		/*  Calculate the optimal single mapping transformation matrix between multiple two-dimensional point pairs H（3x3）, Use MSE or RANSAC Method , Find the transformation matrix between two planes  */
		if (goodkeypoint_left.size() < 4 || goodkeypoint_right.size() < 4) return false;


		// Get images right To image left Projection mapping matrix , Size is 3*3
		// Order of attention ,srcPoints Corresponding goodkeypoint_right,dstPoints Corresponding goodkeypoint_left
		H = findHomography(goodkeypoint_right, goodkeypoint_left, RANSAC);

		// Yes image_right Do perspective transformation 
		warpPerspective(image_right, WarpImg, H, Size(image_right.cols + image_left.cols, image_right.rows));
		//imshow(" Perspective transformation ", WarpImg);

		DstImg = WarpImg.clone();
		// take image_left Copy to the perspective transformed picture , Complete image stitching 
		image_left.copyTo(DstImg(Rect(0, 0, image_left.cols, image_left.rows)));
		//imshow(" Image panoramic mosaic ", DstImg);

		return true;
	}


	// Image fusion , Make the splicing natural 
	bool OptimizeSeam(const Mat& H, const Mat& image_left, Mat& WarpImg, Mat& DstImg)
	{
		// Perspective transform upper left corner (0,0,1)
		Mat V2 = (Mat_<double>(3, 1) << 0.0, 0.0, 1.0);
		Mat V1 = H * V2;
		int left_top = V1.at<double>(0, 0) / V1.at<double>(2, 0);
		if (left_top < 0)left_top = 0;

		// Perspective transform lower left corner (0,src.rows,1)
		V2 = (Mat_<double>(3, 1) << 0.0, image_left.rows, 1.0);
		V1 = H * V2;
		int left_bottom = V1.at<double>(0, 0) / V1.at<double>(2, 0);
		if (left_bottom < 0)left_bottom = 0;

		int start = MAX(left_top, left_bottom);
		// Starting position , That is, the left boundary of the overlapping area  

		double Width = (image_left.cols - start); // The width of the overlapping area  

		//line(WarpImg, Point(start, 0), Point(start, WarpImg.rows), Scalar(0, 0, 255), 2);
		//line(WarpImg, Point(image_left.cols, 0), Point(image_left.cols, WarpImg.rows), Scalar(0, 0, 255), 2);

		// Image weighted fusion , By changing alpha modify image_left And WarpImg Pixel weight , Achieve the fusion effect 
		double alpha = 1.0;
		for (int i = 0; i < DstImg.rows; i++)
		{
			for (int j = start; j < image_left.cols; j++)
			{
				for (int c = 0; c < 3; c++)
				{
					// If the image WarpImg The pixel is 0, Then a full copy image_left
					if (WarpImg.at<Vec3b>(i, j)[c] == 0)
					{
						alpha = 1.0;
					}
					else
					{
						double l = Width - (j - start);
						// The distance from a pixel in the overlapping area to the seam 
						alpha = l / Width;
					}
					DstImg.at<Vec3b>(i, j)[c] = image_left.at<Vec3b>(i, j)[c] * alpha + WarpImg.at<Vec3b>(i, j)[c] * (
						1.0 - alpha);
				}
			}
		}

		//imshow(" Image fusion ", DstImg);
		return true;
	}

	Mat OpenCV_Stitching(const Mat& image_left, const Mat& image_right, bool& isOk)
	{
		// Put the pictures to be spliced into the container 
		vector<Mat> images;
		images.push_back(image_left);
		images.push_back(image_right);

		// establish Stitcher Model 
		Ptr<Stitcher> stitcher = Stitcher::create();

		Mat result;
		Stitcher::Status status = stitcher->stitch(images, result); //  Use stitch Function 

		if (status != Stitcher::OK)
			isOk = false;

		//imshow("OpenCV Image panoramic mosaic ", result);

		return result;
	}
};


Mat CImagePanorama::CreatePanorama(const Mat& image_left, const Mat& image_right)
{
	CPanoramaPimp pimpl;
	Mat out;

	if (image_left.empty() || image_right.empty())
	{
		cout << "No Image!" << endl;
		system("pause");
		return Mat();
	}

	Mat H, WarpImg, DstImg;
	if (pimpl.Image_Stitching(image_left, image_right, H, WarpImg, DstImg, false))
	{
		if (!pimpl.OptimizeSeam(H, image_left, WarpImg, DstImg))
		{
			throw("Image fusion is not possible! ");
		}
	}
	else
	{
		throw("can not stitching the image!");
	}
	bool isOk = true;
	out = pimpl.OpenCV_Stitching(image_left, image_right, isOk);
	if (!isOk)
	{
		throw("can not stitching the image!");
	}
	return out;
}
