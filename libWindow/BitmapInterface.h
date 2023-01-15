#pragma once

namespace Regards::Window
{
	class CBitmapInterface
	{
	public:
		CBitmapInterface()
		{
		};

		virtual ~CBitmapInterface()
		{
		};

		virtual void TransitionEnd()
		{
		};

		virtual void ImageSuivante()
		{
		};

		virtual void ImagePrecedente()
		{
		};
	};
}
