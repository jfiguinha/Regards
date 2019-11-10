#pragma once
// The contents of this file are in the public domain. See LICENSE_FOR_EXAMPLE_PROGRAMS.txt
/*
	This example shows how to classify an image into one of the 1000 imagenet
	categories using the deep learning tools from the dlib C++ Library.  We will
	use the pretrained ResNet34 model available on the dlib website.

	The ResNet34 architecture is from the paper Deep Residual Learning for Image
	Recognition by He, Zhang, Ren, and Sun.  The model file that comes with dlib
	was trained using the dnn_imagenet_train_ex.cpp program on a Titan X for
	about 2 weeks.  This pretrained model has a top5 error of 7.572% on the 2012
	imagenet validation dataset.

	For an introduction to dlib's DNN module read the dnn_introduction_ex.cpp and
	dnn_introduction2_ex.cpp example programs.


	Finally, these tools will use CUDA and cuDNN to drastically accelerate
	network training and testing.  CMake should automatically find them if they
	are installed and configure things appropriately.  If not, the program will
	still run but will be much slower to execute.
*/

// This block of statements defines the resnet-34 network

#include <dlib/dnn.h>
#include <iostream>
#include <dlib/data_io.h>
#include <dlib/image_transforms.h>

using namespace std;
using namespace dlib;

// ----------------------------------------------------------------------------------------

// This block of statements defines the resnet-34 network

template <template <int, template<typename>class, int, typename> class block, int N, template<typename>class BN, typename SUBNET>
using residual = add_prev1<block<N, BN, 1, tag1<SUBNET>>>;

template <template <int, template<typename>class, int, typename> class block, int N, template<typename>class BN, typename SUBNET>
using residual_down = add_prev2<avg_pool<2, 2, 2, 2, skip1<tag2<block<N, BN, 2, tag1<SUBNET>>>>>>;

template <int N, template <typename> class BN, int stride, typename SUBNET>
using block = BN<con<N, 3, 3, 1, 1, relu<BN<con<N, 3, 3, stride, stride, SUBNET>>>>>;

template <int N, typename SUBNET> using ares = relu<residual<block, N, affine, SUBNET>>;
template <int N, typename SUBNET> using ares_down = relu<residual_down<block, N, affine, SUBNET>>;

template <typename SUBNET> using level1 = ares<512, ares<512, ares_down<512, SUBNET>>>;
template <typename SUBNET> using level2 = ares<256, ares<256, ares<256, ares<256, ares<256, ares_down<256, SUBNET>>>>>>;
template <typename SUBNET> using level3 = ares<128, ares<128, ares<128, ares_down<128, SUBNET>>>>;
template <typename SUBNET> using level4 = ares<64, ares<64, ares<64, SUBNET>>>;

using a_net_type = loss_multiclass_log<fc<1000, avg_pool_everything<
	level1<
	level2<
	level3<
	level4<
	max_pool<3, 3, 2, 2, relu<affine<con<64, 7, 7, 2, 2,
	input_rgb_image_sized<227>
	>>>>>>>>>>>;

class CPictureCategorieData
{
public:
	CPictureCategorieData();
	~CPictureCategorieData();
	int LoadPredictor(const std::string & file_name);
	std::vector<int> GetCategorie(const char * filename);
	std::vector<int> GetCategorie(const unsigned char * data, const int &size);
	std::vector<string> GetCategorieLabel(const char * filename);
	void GetLabel(const int &numLabel, char * labelOut, int size);

private:
	std::vector<int> GetCategorie(const matrix<rgb_pixel> & img);
	string predicator;
	std::vector<string> labels;
	a_net_type net;
};