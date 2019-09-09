#include "PictureCategorieData.h"


// ----------------------------------------------------------------------------------------

rectangle make_random_cropping_rect_resnet(
	const matrix<rgb_pixel>& img,
	dlib::rand& rnd
)
{
	// figure out what rectangle we want to crop from the image
	double mins = 0.466666666, maxs = 0.875;
	auto scale = mins + rnd.get_random_double()*(maxs - mins);
	auto size = scale * std::min(img.nr(), img.nc());
	rectangle rect(size, size);
	// randomly shift the box around
	point offset(rnd.get_random_32bit_number() % (img.nc() - rect.width()),
		rnd.get_random_32bit_number() % (img.nr() - rect.height()));
	return move_rect(rect, offset);
}

// ----------------------------------------------------------------------------------------

void randomly_crop_images(
	const matrix<rgb_pixel>& img,
	dlib::array<matrix<rgb_pixel>>& crops,
	dlib::rand& rnd,
	long num_crops
)
{
	std::vector<chip_details> dets;
	for (long i = 0; i < num_crops; ++i)
	{
		auto rect = make_random_cropping_rect_resnet(img, rnd);
		dets.push_back(chip_details(rect, chip_dims(227, 227)));
	}

	extract_image_chips(img, dets, crops);

	for (auto&& img : crops)
	{
		// Also randomly flip the image
		if (rnd.get_random_double() > 0.5)
			img = fliplr(img);

		// And then randomly adjust the colors.
		apply_random_color_offset(img, rnd);
	}
}

CPictureCategorieData::CPictureCategorieData()
{
	//detector = get_frontal_face_detector();
}

CPictureCategorieData::~CPictureCategorieData()
{

}

void CPictureCategorieData::GetLabel(const int &numLabel, char * labelOut, int size)
{
	string label = labels[numLabel];
	strcpy_s(labelOut, size, label.c_str());
}

int CPictureCategorieData::LoadPredictor(const std::string & file_name)
{
	predicator = file_name;
	// We will also use a face landmarking model to align faces to a standard pose:  (see face_landmark_detection_ex.cpp for an introduction)
	deserialize(file_name) >> net >> labels;
	return 0;
}


std::vector<int> CPictureCategorieData::GetCategorie(const char * filename)
{
	
	std::vector<int> categorie;
	// Make a network with softmax as the final layer.  We don't have to do this
	// if we just want to output the single best prediction, since the anet_type
	// already does this.  But if we instead want to get the probability of each
	// class as output we need to replace the last layer of the network with a
	// softmax layer, which we do as follows:

	softmax<a_net_type::subnet_type> snet;
	snet.subnet() = net.subnet();


	dlib::array<matrix<rgb_pixel>> images;
	matrix<rgb_pixel> img, crop;

	dlib::rand rnd;

	load_image(img, filename);

	const int num_crops = 16;
	// Grab 16 random crops from the image.  We will run all of them through the
	// network and average the results.
	randomly_crop_images(img, images, rnd, num_crops);
	// p(i) == the probability the image contains object of class i.
	matrix<float, 1, 1000> p = sum_rows(mat(snet(images.begin(), images.end()))) / num_crops;

	// Print the 5 most probable labels
	for (int k = 0; k < 5; ++k)
	{
		unsigned long predicted_label = index_of_max(p);
		//cout << p(predicted_label) << ": " << labels[predicted_label] << endl;
		categorie.push_back(predicted_label);
		p(predicted_label) = 0;
	}

	return categorie;
}


std::vector<string> CPictureCategorieData::GetCategorieLabel(const char * filename)
{

	std::vector<string> categorie;
	// Make a network with softmax as the final layer.  We don't have to do this
	// if we just want to output the single best prediction, since the anet_type
	// already does this.  But if we instead want to get the probability of each
	// class as output we need to replace the last layer of the network with a
	// softmax layer, which we do as follows:
	softmax<a_net_type::subnet_type> snet;
	snet.subnet() = net.subnet();

	dlib::array<matrix<rgb_pixel>> images;
	matrix<rgb_pixel> img, crop;

	dlib::rand rnd;

	load_image(img, filename);

	const int num_crops = 16;
	// Grab 16 random crops from the image.  We will run all of them through the
	// network and average the results.
	randomly_crop_images(img, images, rnd, num_crops);
	// p(i) == the probability the image contains object of class i.
	matrix<float, 1, 1000> p = sum_rows(mat(snet(images.begin(), images.end()))) / num_crops;

	// Print the 5 most probable labels
	for (int k = 0; k < 5; ++k)
	{
		unsigned long predicted_label = index_of_max(p);
		//cout << p(predicted_label) << ": " << labels[predicted_label] << endl;
		categorie.push_back(labels[predicted_label]);
		p(predicted_label) = 0;
	}

	return categorie;
}