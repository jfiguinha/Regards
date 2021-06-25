#pragma once
namespace Regards
{
	namespace Window
	{
		class CSliderInterface
		{
		public:
			CSliderInterface()
			{
			};

			virtual ~CSliderInterface()
			{
			};

			virtual void ZoomPos(const int& position) = 0;
			virtual void SlidePosChange(const int& position, const wxString& key) = 0;
			virtual void MoveSlider(const int64_t& position) = 0;
			virtual void ClickButton(const int& id) = 0;
			virtual void SetTrackBarPosition(const int& iPos) = 0;
		};
	}
}
