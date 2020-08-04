#pragma once
#include <theme.h>
#include <OpenCLEngine.h>
using namespace Regards::OpenCL;

class CThumbnailData;
class CRegardsConfigParam;

namespace Regards
{
	namespace Window
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
            void SetPictureLoading(const wxImage &imageLoading);
            void DeleteCache();
			void SetBackgroundColor(const wxColour & backgroundColor);
			void ShowSelectButton(const bool &show)
			{
				showSelected = show;
			};

			CIcone& operator=(const CIcone& other);
			inline bool operator== (const CIcone &n1);

			void SetData(CThumbnailData * thumbnailData, const bool &deleteData = true);
			CThumbnailData * GetData();
			bool DataNeedToBeDelete();
			CThumbnailData * GetCopyData();
			void SetTheme(CThemeIcone theme);

			//------------------------------------------
			//Theme
			//------------------------------------------
			void SetSizeIcone(const int &width, const int &height);

			//------------------------------------------
			void SetNumElement(const int &numElement){ this->numElement = numElement; };
			int GetNumElement(){ return numElement; };

			void SetWindowPos(const int &x, const int &y);
			bool OnClick(const int &x, const int &y, const int &posLargeur, const int &posHauteur);

			int GetXPos();
			int GetYPos();
            int GetWidth();
            int GetHeight();
			wxRect GetPos();

			wxString GetFilename(){ return filename; };
			void SetFilename(const wxString &filename){
				this->filename = filename;
			};

			void SetActive(const bool &value);
			void SetSelected(const bool &value);
			void SetChecked(const bool &value);

			bool GetSelected();

			int GetState()
			{
				return state;
			}

			int RenderIcone(wxDC * dc, const int &posLargeur, const int &posHauteur);

			bool IsChecked();

			bool IsPictureLoad()
			{
				return pictureLoad;
			}

			void DestroyCache();
            

		private:

            wxImage CreateFromSVG(const int & buttonWidth, const int & buttonHeight, const wxString &vector);
			void RenderPictureBitmap(wxDC * dc, const wxImage & bitmapScale, const int &type);
			void RenderVideoBitmap(wxDC * dc, const wxImage & bitmapScale, const int &type);
			void RenderBitmap(wxDC * dc, const wxImage & bitmapScale, const int &type);
			
			void CalculPosition(const wxImage &  render);
			void GetBitmapDimension(const int &width, const int &height, int &tailleAffichageBitmapWidth, int &tailleAffichageBitmapHeight, float &newRatio);
			float CalculRatio(const int &width, const int &height, const int &tailleBitmapWidth, const int &tailleBitmapHeight);
			wxImage LoadImageResource(const wxString & resourceName);
			wxImage GenerateVideoIcone();

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
			CThumbnailData *pThumbnailData;
			
            wxImage bitmapCheckOn;
			wxImage bitmapCheckOff;
			bool useBackgroundColor;
            wxString photoVector;
            wxString checkOnVector;
            wxString checkOffVector;
			wxColour backgroundColor;
            

            wxString tempImageVector;
			bool pictureLoad;
			bool showSelected;
			bool isChecked;
			bool isSelected;
			bool photoDefault;
			int posXThumbnail;
			int posYThumbnail;
			int numElement;
			int x;
			int y;
			bool deleteData;
			int thumbnailIconeCache;
			CRegardsConfigParam * config;
			wxString filename;
			int state;
			int numLib;
            int width;
            int height;
            bool showLoading;
            wxImage pictureLoading;
            wxImage transparent;
			//COpenCLContext * openclContext;

			bool eraseBitmap = true;
			wxBitmap bitmapLocal;
		};
		typedef tbb::concurrent_vector<CIcone *> IconeVector;
		typedef bool(*pItemCompFonct)(int, int, CIcone *, CWindowMain *);
	}
}
