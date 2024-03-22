#include "header.h"
#include "colornet.h"

extern ncnn::VulkanDevice* vkdev;

CColorization::CColorization()
{
    net.opt.use_vulkan_compute = true;
    net.opt.num_threads = std::thread::hardware_concurrency();

    if (vkdev == nullptr)
        net.opt.use_vulkan_compute = false;
    else
        net.set_vulkan_device(vkdev);

}

CColorization::~CColorization()
{
   net.clear();
}

bool CColorization::IsLoaded()
{
    return isLoad;
}

int CColorization::load(const std::string& param_path, const std::string& model_path)
{
    int ret = net.load_param(param_path.c_str());
    if (ret < 0)
    {
        fprintf(stderr, "open param file %s failed\n", param_path.c_str());
        return -1;
    }
    ret = net.load_model(model_path.c_str());
    if (ret < 0)
    {
        fprintf(stderr, "open bin file %s failed\n", model_path.c_str());
        return -1;
    }
    isLoad = true;
    return 0;
}

cv::Mat CColorization::colorization(const cv::Mat& bgr)
{
    printf("toto 1 \n");
    cv::Mat out_image;
	try
	{
      //fixed input size for the pretrained network
      const int W_in = 224;
      const int H_in = 224;
      
       printf("toto 2 \n");
      
      cv::Mat Base_img, lab, L, input_img;
      Base_img = bgr.clone();
      
      //normalize levels
      Base_img.convertTo(Base_img, CV_32F, 1.0/255);
      
      //Convert BGR to LAB color space format
      cvtColor(Base_img, lab, cv::COLOR_BGR2Lab);
      
      //Extract L channel
      cv::extractChannel(lab, L, 0);
      
      //Resize to input shape 224x224
      resize(L, input_img, cv::Size(W_in, H_in));
      
      //We subtract 50 from the L channel (for mean centering)
      //input_img -= 50;
      
       printf("toto 3 \n");
      
      //convert to NCNN::MAT
      ncnn::Mat in_LAB_L(input_img.cols, input_img.rows, 1, (void*)input_img.data);
      in_LAB_L = in_LAB_L.clone();
      
      //Set the temperature T for the annealed mean operation. Blob Trecip is the reciprocal of the temperature.
      //filled value = 6/ln(10) # 1/T, set annealing temperature 0.38
      ncnn::Mat in_Trecip(56, 56, 313);
      in_Trecip.fill(2.606f);
      
      ncnn::Extractor ex = net.create_extractor();
      //set input, output lyers
      ex.input("data_l", in_LAB_L);
      ex.input("Trecip", in_Trecip);
      
       printf("toto 4 \n");
      
      //inference network 
      ncnn::Mat out;
      ex.extract("class8_ab", out);
      
      //create LAB material
      cv::Mat colored_LAB(out.h, out.w, CV_32FC2);
      //Extract ab channels from ncnn:Mat out
      memcpy((uchar*)colored_LAB.data, out.data, out.w * out.h * 2 * sizeof(float));
      
      //get separsted LAB channels a&b
      cv::Mat a(out.h, out.w, CV_32F, (float*)out.data);
      cv::Mat b(out.h, out.w, CV_32F, (float*)out.data + out.w * out.h);
      
       printf("toto 5 \n");
      
      //Resize a, b channels to origina image size
      cv::resize(a, a, Base_img.size());
      cv::resize(b, b, Base_img.size());
      
      printf("toto 6 \n");
      
      //merge channels, and convert back to BGR
      cv::Mat color, chn[] = {L, a, b};
      cv::merge(chn, 3, lab);
      
      printf("toto 7 \n");
      
      cvtColor(lab, color, cv::COLOR_Lab2BGR);
      //normalize values to 0->255
      color.convertTo(color, CV_8UC3, 255);
      
      color.copyTo(out_image);
      
      printf("toto 8 \n");
      
	}
	catch (cv::Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "BilateralEffect exception caught: " << err_msg << std::endl;
		std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
	}
    


  return out_image;
}

/*
int main_colorization(int argc, char** argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s [imagepath]\n", argv[0]);
        return -1;
    }

    const char* imagepath = argv[1];

    cv::Mat m = cv::imread(imagepath, 1);
    if (m.empty())
    {
        fprintf(stderr, "cv::imread %s failed\n", imagepath);
        return -1;
    }
    cv::Mat out_image;
    colorization(m, out_image);

    return 0;
}
*/