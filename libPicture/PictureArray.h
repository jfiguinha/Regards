#pragma once

namespace Regards
{
	namespace Picture
	{

		class CPictureArray
		{
		public:
			CPictureArray(const cv::_InputArray::KindFlag &type);
			virtual ~CPictureArray() {};
			CPictureArray(cv::Mat& m);
			CPictureArray(cv::cuda::GpuMat& d_mat);
			CPictureArray(cv::UMat& m);
			cv::_InputArray::KindFlag Kind();
			cv::UMat& getUMat();
			cv::Mat& getMat();
			cv::cuda::GpuMat& getGpuMat();
			int getWidth();
			int getHeight();
			void CopyFrom(cv::ogl::Texture2D* tex);
			void copyTo(cv::Mat& m);
			void copyTo(cv::cuda::GpuMat& d_mat);
			void copyTo(cv::UMat& m);

		protected:
			cv::_InputArray::KindFlag kind;
			cv::UMat umat;
			cv::Mat mat;
			cv::cuda::GpuMat gpuMat;
		};
	}
}
