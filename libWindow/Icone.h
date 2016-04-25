#pragma once
#include <RegardsBitmap.h>
#include <RGBAQuad.h>
#include <Theme.h>
#include <ConfigParam.h>
#include <vector>
#include <algorithm>
#include <RegardsConfigParam.h>
using namespace std;

class CThumbnailData;

namespace Regards
{
	namespace Window
	{
		#define INACTIFICONE 1
		#define ACTIFICONE 2
		#define SELECTEDICONE 3

		class CIcone
		{
		public:
			CIcone(void);
			~CIcone(void);
            
            void StartLoadingPicture();
            void StopLoadingPicture();
            void SetPictureLoading(const wxImage &imageLoading);
            void DeleteCache();

			void ShowSelectButton(const bool &show)
			{
				showSelected = show;
			};

			inline bool operator== (const CIcone &n1);

			void SetData(CThumbnailData * thumbnailData);
			CThumbnailData * GetData();

			void SetTheme(CThemeIcone theme);

			//------------------------------------------
			//Theme
			//------------------------------------------
			void SetSizeIcone(const int &width, const int &height);

			//------------------------------------------
			void SetNumElement(const int &numElement){ this->numElement = numElement; };
			int GetNumElement(){ return numElement; };

			void SetWindowPos(const int &x, const int &y);
			bool OnClick(const int &x, const int &y);

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

			void RenderIcone(wxDC * dc);

			bool IsChecked();

			bool IsPictureLoad()
			{
				return pictureLoad;
			}

			void DestroyCache();
            
            void Lock()
            {
                isLock = true;
            }
            
            void Unlock()
            {
                isLock = false;
            }
            
            bool IsLock()
            {
                return isLock;
            }
            

		private:

            wxImage CreateFromSVG(const int & buttonWidth, const int & buttonHeight, const wxString &vector);
			void RenderPictureBitmap(wxDC * dc, const wxImage & bitmapScale, const int &type);
			void RenderVideoBitmap(wxDC * dc, const wxImage & bitmapScale, const int &type);
			void RenderBitmap(wxDC * dc, const wxImage & bitmapScale, const int &type);
			
			void CalculPosition(const wxImage &  render);
			void GetBitmapDimension(const int &width, const int &height, int &tailleAffichageBitmapWidth, int &tailleAffichageBitmapHeight, float &newRatio);
			float CalculRatio(const int &width, const int &height, const int &tailleBitmapWidth, const int &tailleBitmapHeight);

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
			int interpolationMethod;
			CThumbnailData * pThumbnailData = nullptr;
			
            static wxImage imagePhoto;
            wxImage bitmapCheckOn;
			wxImage bitmapCheckOff;
            
            wxString photoVector;
            wxString checkOnVector;
            wxString checkOffVector;
            bool isVector = true;
            
           // wxImage image;
			//wxImage scale;
            wxBitmap * memBitmap = nullptr;
            wxString tempImageVector;
			bool pictureLoad;
			bool showSelected = false;
			bool isChecked = false;
			bool isSelected = false;
			bool photoDefault = true;
			int posXThumbnail;
			int posYThumbnail;
			int numElement;
			int x;
			int y;
			int thumbnailIconeCache;
			CRegardsConfigParam * config = nullptr;
			wxString filename;
			int state;
			int numLib;
            bool isLock = false;
            int width = 0;
            int height = 0;
            bool showLoading = false;
            wxImage pictureLoading;
            wxImage transparent;
            
		};
		typedef std::vector<CIcone *> IconeVector;

	}
}
