#pragma once
#include <theme.h>

#define INACTIFICONE 1
#define ACTIFICONE 2
#define SELECTEDICONE 3
#define USEBACKGROUNDCOLOR 4

class CThumbnailData;
class CRegardsConfigParam;

namespace Regards::Window
{
	class CWindowMain;



	class CIcone
	{
	public:
		CIcone(CThumbnailData* data);
		~CIcone(void) = default;
		void StartLoadingPicture();
		void StopLoadingPicture();
		void SetPictureLoading(const wxImage& imageLoading);
		void SetBackgroundColor(const wxColour& backgroundColor);
		void SetNumFrame(int numFrame);
		void ShowSelectButton(const bool& show)
		{
			showSelected = show;
		};

		bool GetVisibility();
		void SetVisibility(const bool& visible);
		void RefreshIcone();
		void ReinitPos();
		void InitPos();
		void SetPos(int x, int y);

		CThumbnailData* GetPtData();

		void SetTheme(CThemeIcone theme);

		void GetBitmapIcone(int& returnValue, const bool& flipHorizontal = false, const bool& flipVertical = false,
			const bool& forceRedraw = false);
		//------------------------------------------
		//Theme
		//------------------------------------------
		void SetSizeIcone(const int& width, const int& height);

		//------------------------------------------
		void SetNumElement(const int& numElement) { this->numElement = numElement; };
		int GetNumElement() { return numElement; };

		void SetWindowPos(int x, int y);
		int OnClick(int x, int y, int posLargeur, int posHauteur);

		int GetXPos() const;
		int GetYPos() const;
		int GetWidth();
		int GetHeight();
		wxRect GetPos();

		wxString GetFilename() { return filename; };

		void SetFilename(const wxString& filename)
		{
			this->filename = filename;
		};

		void SetActive(bool value);
		void SetSelected(bool value);
		void SetChecked(bool value);

		void SetShowDelete(bool value);

		bool GetSelected() const;

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

		bool IsChecked() const;

		bool IsPictureLoad()
		{
			return pictureLoad;
		}

		wxBitmap GetCopyIcone() const;

	private:

		void Invalidate() noexcept;

		void RenderPictureBitmap(wxDC* dc, wxImage& bitmapScale, const int& type);
		void RenderVideoBitmap(wxDC* dc, wxImage& bitmapScale, const int& type);
		void RenderBitmap(wxDC* dc, wxImage& bitmapScale, const int& type);

		void CalculPosition(const wxImage& render);
		void GetBitmapDimension(const int& width, const int& height, int& tailleAffichageBitmapWidth,
			int& tailleAffichageBitmapHeight, float& newRatio);
		float CalculRatio(const int& width, const int& height, const int& tailleBitmapWidth,
			const int& tailleBitmapHeight);
		wxImage LoadImageResource(const wxString& resourceName);
		void GenerateVideoIcone();

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
		std::unique_ptr<CThumbnailData> pThumbnailData;

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

		int thumbnailIconeCache;
		CRegardsConfigParam* config;
		wxString filename;
		int state;
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

	using IconeVector = tbb::concurrent_vector<CIcone*>;
	using pItemCompFonct = bool(*)(int, int, CIcone*, CWindowMain*);
	using pItemCompFonctFace = bool(*)(wxString, int, CIcone*);
	using pItemStringCompFonct = bool(*)(wxString, CIcone*);
}