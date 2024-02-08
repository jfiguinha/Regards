#pragma once
#include <theme.h>
//#include <OpenCLEngine.h>
//using namespace Regards::OpenCL;

class CThumbnailData;
class CRegardsConfigParam;

namespace Regards::Window
{
	class CWindowMain;

#define INACTIFICONE 1
#define ACTIFICONE 2
#define SELECTEDICONE 3
#define USEBACKGROUNDCOLOR 4

	class CIcone
	{
	public:
		CIcone();
		~CIcone(void);

		void StartLoadingPicture();
		void StopLoadingPicture();
		void SetPictureLoading(const wxImage& imageLoading);
		void SetBackgroundColor(const wxColour& backgroundColor);
		void SetNumFrame(const int& numFrame);
		void ShowSelectButton(const bool& show)
		{
			showSelected = show;
		};

		bool GetVisibility();
		void SetVisibility(const bool& visible);
		void RefreshIcone();
		void ReinitPos();
		void InitPos();
		void SetPos(const int& x, const int& y);

		CIcone& operator=(const CIcone& other);
		inline bool operator==(const CIcone& n1);

		void SetData(CThumbnailData* thumbnailData, const bool& deleteData = true);
		CThumbnailData* GetData();
		bool DataNeedToBeDelete();
		CThumbnailData* GetCopyData();
		void SetTheme(CThemeIcone theme);

		wxBitmap GetBitmapIcone(int& returnValue, const bool& flipHorizontal = false, const bool& flipVertical = false,
		                        const bool& forceRedraw = false);
		//------------------------------------------
		//Theme
		//------------------------------------------
		void SetSizeIcone(const int& width, const int& height);

		//------------------------------------------
		void SetNumElement(const int& numElement) { this->numElement = numElement; };
		int GetNumElement() { return numElement; };

		void SetWindowPos(const int& x, const int& y);
		int OnClick(const int& x, const int& y, const int& posLargeur, const int& posHauteur);

		int GetXPos();
		int GetYPos();
		int GetWidth();
		int GetHeight();
		wxRect GetPos();

		wxString GetFilename() { return filename; };

		void SetFilename(const wxString& filename)
		{
			this->filename = filename;
		};

		void SetActive(const bool& value);
		void SetSelected(const bool& value);
		void SetChecked(const bool& value);

		void SetShowDelete(const bool& value);

		bool GetSelected();

		int GetState()
		{
			return state;
		}

		void SetState(int state)
		{
			this->state = state;
		}

		int RenderIcone(wxDC* dc, const int& posLargeur, const int& posHauteur, const bool& flipHorizontal,
		                const bool& flipVertical, const bool& forceRedraw = false);

		bool IsChecked();

		bool IsPictureLoad()
		{
			return pictureLoad;
		}

		//std::mutex muIcone;

	private:
		void RenderPictureBitmap(wxDC* dc, wxImage& bitmapScale, const int& type);
		void RenderVideoBitmap(wxDC* dc, wxImage& bitmapScale, const int& type);
		void RenderBitmap(wxDC* dc, wxImage& bitmapScale, const int& type);

		void CalculPosition(const wxImage& render);
		void GetBitmapDimension(const int& width, const int& height, int& tailleAffichageBitmapWidth,
		                        int& tailleAffichageBitmapHeight, float& newRatio);
		float CalculRatio(const int& width, const int& height, const int& tailleBitmapWidth,
		                  const int& tailleBitmapHeight);
		wxImage LoadImageResource(const wxString& resourceName);
		wxImage GenerateVideoIcone();
		wxImage ResampleBicubic(wxImage* Src, int width, int height);
		int GetBitmapWidth();
		int GetBitmapHeight();

		//---------------------------------------------------
		//Theme
		//---------------------------------------------------
		CThemeIcone themeIcone;

		//---------------------------------------------------
		//Variable
		//---------------------------------------------------
		//int interpolationMethod;
		CThumbnailData* pThumbnailData;

		static wxImage videoCadre;
		static wxImage photoTemp;
		wxImage bitmapCheckOn;
		wxImage bitmapCheckOff;
		wxImage bitmapDelete;
		bool useBackgroundColor;
		wxColour backgroundColor;

		wxString tempImageVector;
		bool pictureLoad;
		bool showSelected;
		bool showDeleted;
		bool isChecked;
		bool isSelected;
		bool photoDefault = false;
		int posXThumbnail;
		int posYThumbnail;
		int numElement;
		int x;
		int y;
		int oldx;
		int oldy;
		bool deleteData;
		int thumbnailIconeCache;
		CRegardsConfigParam* config;
		wxString filename;
		int state;
		int numLib;
		int width;
		int height;
		bool showLoading;
		wxImage pictureLoading;
		wxImage transparent;
		wxImage scaleBackup;
		wxBitmap localmemBitmap_backup;
		bool redraw = true;
		bool visible = true;
	};

	using IconeVector = tbb::concurrent_vector<std::shared_ptr<CIcone>>;
	using pItemCompFonct = bool(*)(int, int, std::shared_ptr<CIcone>, CWindowMain*);
	using pItemStringCompFonct = bool(*)(wxString, std::shared_ptr<CIcone>);
}
