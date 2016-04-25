// stdafx.h : fichier Include pour les fichiers Include système standard,
// ou les fichiers Include spécifiques aux projets qui sont utilisés fréquemment,
// et sont rarement modifiés
//

#pragma once
#include <RegardsBitmap.h>
#include <EffectParameter.h>
#include <ParamInit.h>
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <string>
#include <vector>
using namespace std;

#define RENDERGDI 2
#define RENDEROPENGL 3

namespace Regards
{
	namespace Render
	{
		class CTextureManager
		{
		public:
			wxString filename;
		};

		class CRenderBitmapInterface
		{
		public:

			CRenderBitmapInterface()
			{
				createList = true;
				listTexture = new vector<CTextureManager *>();
				config = CParamInit::getInstance();
			};
			virtual ~CRenderBitmapInterface()
			{
				if (createList)
				{
					for (vector<CTextureManager *>::const_iterator it = listTexture->begin(); it != listTexture->end(); it++)
					{
						CTextureManager * texture = (CTextureManager *)*it;
						delete texture;
						texture = nullptr;
					}

					listTexture->clear();
					delete listTexture;
				}
			};

			vector<CTextureManager *> * GetTextureList()
			{
				return listTexture;
			}

			virtual int GetTextureID(const wxString &textureName){
				return 0;
			};

			virtual void CreateScreenRender(wxDC * deviceContext, const CRgbaquad &color) = 0;
			virtual void CreateScreenTexture(const int &width, const int &height) = 0;
			virtual bool TestIsTextureExist(const wxString &entry){ return false; };
			virtual int AddTextureBitmap(CRegardsBitmap * bitmap, const wxString &entry){ return 0; };
			virtual void AddAlphaBlendingBitmap(const wxString &textureName, const int &left, const int &top)= 0;
			virtual void AddBitmap(const wxString &textureName, const int &left, const int &top)= 0;
			virtual void AddBitmapToScale(const wxString &textureName, const int &widthScale, const int &heightScale, const int &left, const int &top)= 0;
			virtual void AddBitmapToScalePosition(const wxString &textureName, const int &widthScale, const int &heightScale, const wxRect &rc, const int &left, const int &top) = 0;
			virtual void RenderToScreen(wxDC * deviceContext)= 0;
			virtual void Transition(const wxString &textureName, const int &step)= 0;
            
            virtual void RenderEffect(const int &numEffect, CEffectParameter * effectParameter)= 0;
            void MotionBlur(const double &radius, const double &sigma, const double &angle);
            void LensFlare(const int &iPosX, const int &iPosY, const int &iPuissance, const int &iType, const int &iIntensity, const int &iColor, const int &iColorIntensity);
            virtual void RotateFree(const int &angle) = 0;
			virtual int GetType(){
				return 0;
			};



		protected:


			CTextureManager * FindTexture(const wxString &name)
			{
				//bool find = false;
				for (vector<CTextureManager *>::const_iterator it = listTexture->begin(); it != listTexture->end(); it++)
				{
					CTextureManager * texture = *it;
					if (texture->filename == name)
					{
						return texture;
					}
				}

				return nullptr;
			}

			CRegardsBitmap * renderBitmap = nullptr;
			CRegardsBitmap * bitmapScale = nullptr;
			CRgbaquad colorBackground;
			vector<CTextureManager *> * listTexture;
			bool createList;
			wxPoint bitmapZone;
			CRegardsConfigParam * config = nullptr;
		};
	}
}