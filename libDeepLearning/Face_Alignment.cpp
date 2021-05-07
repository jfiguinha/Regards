/************************************************************************************************************************
 * Author     : Manu BN
 * Description: Face Alignment using Dlib & OpenCV for a single face.
 *              Haar Cascade is used only for cropping the face at the end
 *              Download "shape_predictor_68_face_landmarks.dat" file from "http://dlib.net/files/shape_predictor_68_face_landmarks.dat.bz2" *
 * Contact    : manubn88@gmail.com
 ***********************************************************************************************************************/

#include <header.h>
#include "face_alignement.h"
#include <opencv2/objdetect/objdetect.hpp>
#include <dlib/image_processing/shape_predictor.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <dlib/image_processing.h>
#include <dlib/opencv/cv_image.h>
#include <iostream>
//#include "opencv2/photo.hpp"
#include <dlib/opencv.h>
#include <dlib/image_processing/frontal_face_detector.h>
//#include <dlib/image_processing/render_face_detections.h>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
using namespace dlib;


//function declarations
void Rect2rectangle(Rect& r,dlib::rectangle& rec);

void dlib_point2cv_Point(full_object_detection& S, std::vector<Point>& L,double& scale);

dlib::rectangle openCVRectToDlib(cv::Rect r);

int face_alignement(const string&filename, const string & landmarkPath){


    // Image Path
    Mat image = imread(filename);
    //Declaring a variable "image" to store input image given.
    Mat gray,resized,detected_edges,Laugh_L,Laugh_R;

    //try block, tries to read the image and if there is a problem reading the image,it throws an exception.

    //image = imread("/home/user/Cartoon/Indian Face/SHIBAN QADARI.jpg");
    if(image.empty()){
        throw(0);
        cout<<"Exception: Image is not read properly, please check image path"<<endl;
        return -1;
    }

    // Resize
    // cv::resize(image,image,Size(480,640));


    //scale for resizing.
    double scale = 1;

   // frontal_face_detector detector = get_frontal_face_detector();
   // shape_predictor pose_model;

    //converts original image to gray scale and stores it in "gray".
    cvtColor(image,gray,COLOR_BGR2GRAY);

    //resize the gray scale image for speeding the face detection.
    cv::resize(gray,resized,Size(),scale,scale);


    //converts original image to gray scale and stores it in "gray".
    cvtColor(image,gray,COLOR_BGR2GRAY);

    //resize the gray scale image for speeding the face detection.
    cv::resize(gray,resized,Size(),scale,scale);

    //cout<<"Resized Image"<<" Rows "<< resized.rows<<" Cols "<<resized.cols<<endl;
    // cout<<"Original Image"<<" Rows "<< image.rows<<" Cols "<<image.cols<<endl;
    //Histogram equalization is performed on the resized image to improve the contrast of the image which can help in detection.
    equalizeHist(resized,resized);

    // Canny( resized, detected_edges, 50,200, 3 );

    /**Object of Shape predictor class "sp" is created to load "shape_predictor_68_face_landmarks.dat" file which is a pre-trained
       cascade of regression tree implemented using "One Millisecond face alignment with an ensemble of regression trees"*/
    shape_predictor sp;
    ///home/user/Alignment/face/shape_predictor_68_face_landmarks.dat
    deserialize(landmarkPath) >> sp;

    // Conver OpenCV image Dlib image i.e. cimg
    cv_image<rgb_pixel> cimg(image);

    // Detect faces using DLib
   // std::vector<dlib::rectangle> face_DLib = detector(cimg);
    /*
    if(face_DLib.empty()){
        cout<<"No face is deteced by DLib"<<endl;
    }
    */
    // Convert DLib Rect to OpenCV Rect

    dlib::rectangle rectDlib;
    cv::Rect R;
    R.x = 0;
    R.y = 0;
    R.width = 150;
    R.height = 150;

    rectDlib = openCVRectToDlib(R);

    // Draw the Dlib detected face on the image
    //  cv::rectangle(image,R, Scalar(0,255, 0), 1, 1);




    // for(unsigned int i=0; i<shapes.size();++i)             // if running on many faces
    // {
    //landmarks vector is declared to store the 68 landmark points. The rest are for individual face components
    std::vector<cv::Point> landmarks,R_Eyebrow,L_Eyebrow,L_Eye,R_Eye,Mouth,Jaw_Line,Nose;

    /**at each index of "shapes" vector there is an object of full_object_detection class which stores the 68 landmark
    points in dlib::point from, which needs to be converted back to cv::Point for displaying.*/
    //std::vector<full_object_detection> shape;
    full_object_detection shape = sp(dlib::cv_image<unsigned char>(resized), rectDlib);
    dlib_point2cv_Point(shape,landmarks,scale);

    // Extract each part using the pre fixed indicies
    for(size_t s=0;s<landmarks.size();s++){
        // circle(image,landmarks[s], 2.0, Scalar( 255,0,0 ), 1, 8 );
        //putText(image,to_string(s),landmarks[s],FONT_HERSHEY_PLAIN,0.8,Scalar(0,0,0));

        // Right Eyebrow indicies
        if(s>=22 && s<=26){
            R_Eyebrow.push_back(landmarks[s]);
            //circle( image,landmarks[s], 2.0, Scalar( 0, 0, 255 ), 1, 8 );
        }
            // Left Eyebrow indicies
        else if(s>=17 && s<=21){
            L_Eyebrow.push_back(landmarks[s]);
        }
            // Left Eye indicies
        else if(s>=36 && s<=41){
            L_Eye.push_back(landmarks[s]);
        }
            // Right Eye indicies
        else if(s>=42 && s<=47){
            R_Eye.push_back(landmarks[s]);
        }
            // Mouth indicies
        else if(s>=48 && s<=67){
            Mouth.push_back(landmarks[s]);
        }
            // Jawline Indicies
        else if(s>=0 && s<=16){
            Jaw_Line.push_back(landmarks[s]);
        }
            // Nose Indicies
        else if(s>=27 && s<=35){
            Nose.push_back(landmarks[s]);
        }
    }


    // 2D image points. If you change the image, you need to change vector
    std::vector<cv::Point2d> image_points;
    image_points.push_back(landmarks[30]);    // Nose tip
    image_points.push_back(landmarks[8]);    // Chin
    image_points.push_back(landmarks[45]);     // Left eye left corner
    image_points.push_back(landmarks[36]);    // Right eye right corner
    image_points.push_back(landmarks[54]);    // Left Mouth corner
    image_points.push_back(landmarks[48]);    // Right mouth corner

    // 3D model points.
    std::vector<cv::Point3d> model_points;
    model_points.push_back(cv::Point3d(0.0f, 0.0f, 0.0f));               // Nose tip
    model_points.push_back(cv::Point3d(0.0f, -330.0f, -65.0f));          // Chin
    model_points.push_back(cv::Point3d(-225.0f, 170.0f, -135.0f));       // Left eye left corner
    model_points.push_back(cv::Point3d(225.0f, 170.0f, -135.0f));        // Right eye right corner
    model_points.push_back(cv::Point3d(-150.0f, -150.0f, -125.0f));      // Left Mouth corner
    model_points.push_back(cv::Point3d(150.0f, -150.0f, -125.0f));       // Right mouth corner

    // Camera internals
    double focal_length = image.cols; // Approximate focal length.
    Point2d center = cv::Point2d(image.cols/2,image.rows/2);
    cv::Mat camera_matrix = (cv::Mat_<double>(3,3) << focal_length, 0, center.x, 0 , focal_length, center.y, 0, 0, 1);
    cv::Mat dist_coeffs = cv::Mat::zeros(4,1,cv::DataType<double>::type); // Assuming no lens distortion

    // cout << "Camera Matrix " << endl << camera_matrix << endl ;
    // Output rotation and translation
    cv::Mat rotation_vector; // Rotation in axis-angle form
    cv::Mat translation_vector;

    // Solve for pose
    cv::solvePnP(model_points, image_points, camera_matrix, dist_coeffs, rotation_vector, translation_vector);

    // Draw line between two eyes
    //cv::line(image,landmarks[45],landmarks[36],Scalar(0,255,0),4);

    // Access the last element in the Rotation Vector
    double rot = rotation_vector.at<double>(0,2);

    // Conver to degrees
    double theta_deg = rot/M_PI*180;

    cout<<theta_deg<<" In degrees"<<endl;


    Mat dst;
    // Rotate around the center
    Point2f pt(image.cols/2., image.rows/2.);
    Mat r = getRotationMatrix2D(pt, theta_deg, 1.0);

    // determine bounding rectangle
    cv::Rect bbox = cv::RotatedRect(pt,image.size(), theta_deg).boundingRect();

    // adjust transformation matrix
    r.at<double>(0,2) += bbox.width/2.0 - center.x;
    r.at<double>(1,2) += bbox.height/2.0 - center.y;

    // Apply affine transform
    warpAffine(image, dst, r, bbox.size());

    imwrite("d:\\test1.jpg", image);
    imwrite("d:\\test2.jpg", dst);

   

    /*
    // Now detect face using Haar Cascade in the image
    CascadeClassifier face_cascade;
    face_cascade.load( "/home/user/OpenCV_Installed/opencv-3.2.0/data/haarcascades/haarcascade_frontalface_alt2.xml" );
    std::vector<Rect> face;
    face_cascade.detectMultiScale(dst,face, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE);


    cv::rectangle(dst,face[0], Scalar(0,0,0), 1, 1);

    // Now crop the face
    Mat Cropped_Face = dst(face[0]);

    // Display image.
   // cv::imshow("Output", dst);

    imshow("Original Image",image);

    imshow("Cropped Face",Cropped_Face);

    waitKey(0);

    */
    return 0;

}

dlib::rectangle openCVRectToDlib(cv::Rect r)
{
    return dlib::rectangle((long)r.tl().x, (long)r.tl().y, (long)r.br().x - 1, (long)r.br().y - 1);
}
/**-------------------------------------------------------------------------------*/

//Function Definitions.

/** This function converts cv::Rect to dlib::rectangle.
    This function is needed because in this implementation I have used opencv and dlib bothand they
    both have their own image processing library so when using a dlib method, its arguments should be
    as expected */
void Rect2rectangle(Rect& r,dlib::rectangle& rec)
{
    rec = dlib::rectangle((long)r.tl().x, (long)r.tl().y, (long)r.br().x - 1, (long)r.br().y - 1);
}


/** This function converts dlib::point to cv::Point and stores in a vector of landmarks
    This function is needed because in this implementation I have used opencv and dlib bothand they
    both have their own image processing library so when using a dlib method, its arguments should be
    as expected */
void dlib_point2cv_Point(full_object_detection& S,std::vector<Point>& L,double& scale)
{
    for(unsigned int i = 0; i<S.num_parts();++i)
    {
        L.push_back(Point(S.part(i).x()*(1/scale),S.part(i).y()*(1/scale)));
    }
}






