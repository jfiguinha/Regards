// stdafx.h : fichier Include pour les fichiers Include système standard,
// ou les fichiers Include spécifiques aux projets qui sont utilisés fréquemment,
// et sont rarement modifiés
//

#pragma once

#include "RenderBitmapInterface.h"
#include <GLTexture.h>
#include <GLSLShader.h>
#include <vector>
#include "wx/glcanvas.h"
using namespace std;
using namespace Regards::OpenGL;

class CRegardsBitmap;

namespace Regards
{
	namespace Render
	{
		class COpenGLTextureManager : public CTextureManager
		{
		public:

			COpenGLTextureManager(){};
			~COpenGLTextureManager(){};

			GLTexture texture;
		};
        
        class COpenGLShader
        {
        public:
            COpenGLShader(){};
            ~COpenGLShader(){};
            
            GLSLShader * m_pShader;
            wxString shaderName;
        };

		class CRenderBitmapInterfaceOpenGL : public CRenderBitmapInterface
		{
		public:

			CRenderBitmapInterfaceOpenGL();
			virtual ~CRenderBitmapInterfaceOpenGL();
            wxImage GetwxImage();

			void CreateScreenRender(wxDC * deviceContext, const CRgbaquad &color);
			void AddAlphaBlendingBitmap(const wxString &textureName, const int &left, const int &top);
			int AddTextureBitmap(CRegardsBitmap * bitmap, const wxString &entry);
			bool TestIsTextureExist(const wxString &entry);
			void CreateScreenTexture(const int &width, const int &height);
			void AddBitmap(const wxString &textureName, const int &left, const int &top);
			void AddBitmapToScale(const wxString &textureName, const int &widthScale, const int &heightScale, const int &left, const int &top);
			void AddBitmapToScalePosition(const wxString &textureName, const int &widthScale, const int &heightScale, const wxRect &rc, const int &left, const int &top);
			void RenderToScreen(wxDC * deviceContext);
			void Transition(const wxString &textureName, const int &step);
			void PhotoFiltre(const CRgbaquad &color, const int &intensity);
			void RGBFiltre(const int &red, const int &green, const int &blue);
			void RotateFree(const int &angle);
			void RedEye(float &strength, const wxRect& rSelectionBox);
			void Swirl(const float &radius, const float &angle, const float &bitmapWidth, const float &bitmapHeight);
			void BrightnessAndContrast(const int &lightness, const int &contrast);
			void Posterize(const float &level, const float &gamma);
			void Solarize(const long &threshold);
			void Clouds(const CRgbaquad &color1, const CRgbaquad &color2, const float &amplitude, const float &frequence, const int &octave);
			int GetType(){ return RENDEROPENGL; };
			void MotionBlur(const double &radius, const double &sigma, const double &angle);
			void LensFlare(const int &iPosX, const int &iPosY, const int &iPuissance, const int &iType, const int &iIntensity, const int &iColor, const int &iColorIntensity);
			int GetTextureID(const wxString &textureName);
			GLTexture * GetTexture(const wxString &textureName);

            CRegardsBitmap * GetRenderBitmap();
            void UpdateRenderTexture(CRegardsBitmap * image);
            
		private:
		
            GLSLShader * FindShader(const wxString &shaderName);
            GLSLShader * CreateShader(const wxString &shaderName);
            
			void RenderQuad();
			void RenderToTexture();
			float CalculPictureRatio(const int &pictureWidth, const int &pictureHeight);
            vector<COpenGLShader *> listShader;
			int renderFromBitmap = 0;
			GLTexture * textureSource = nullptr;
			CRgbaquad colorBackground;
			wxDC * deviceContext = nullptr;
			int width;
			int height;
			int left = 0;
			int top = 0;
		};
	}
}