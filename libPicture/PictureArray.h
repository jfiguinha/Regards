#pragma once


namespace Regards
{
	namespace Picture
	{

		class CPictureArray
		{
		public:
            CPictureArray() {};
			CPictureArray(const cv::_InputArray::KindFlag &type);
			virtual ~CPictureArray() {};
			CPictureArray(cv::Mat& m);
			CPictureArray(cv::UMat& m);
            void SetArray(cv::Mat& m);
            void SetArray(cv::UMat& m);
			cv::_InputArray::KindFlag Kind();
			cv::UMat& getUMat();
			cv::Mat& getMat();
			int getWidth();
			int getHeight();
			void copyTo(cv::Mat& m);
			void copyTo(cv::UMat& m);
			bool empty();
			void Release();

		protected:
			cv::_InputArray::KindFlag kind;
			cv::UMat umat;
			cv::Mat mat;
		};
	}
}
