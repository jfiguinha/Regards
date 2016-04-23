#pragma once
#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include "GLcontext.h"
#include <string>
#include <vector>
using namespace std;

namespace Regards
{
	namespace OpenGL
	{
		class CGLSLParameter
		{
		public:
			CGLSLParameter(){ dataType = 0; };
			~CGLSLParameter(){};

			void SetLibelle(const wxString &libelle);
			virtual void Add(GLint progHandle){};

			unsigned int GetType(){
				return  dataType;
			};

		protected:

			unsigned int dataType;
			wxString libelle;
		};

		class CGLSLParameterInt : public CGLSLParameter
		{
		public:
			CGLSLParameterInt(){
				dataType = 1;
			 };
			~CGLSLParameterInt(){};

			void SetValue(const GLint &value)
			{
				this->value = value;
			};
			void Add(GLint progHandle);

		private:

			GLint value;
		};

		class CGLSLParameterUInt : public CGLSLParameter
		{
		public:
			CGLSLParameterUInt(){
				dataType = 2;
			};
			~CGLSLParameterUInt(){};

			void SetValue(const GLuint &value)
			{
				this->value = value;
			};

			void Add(GLint progHandle);

		private:

			GLuint value;
		};

		class CGLSLParameterFloat : public CGLSLParameter
		{
		public:
			CGLSLParameterFloat(){
				dataType = 3;
			};
			~CGLSLParameterFloat(){};

			void SetValue(const GLfloat &value)
			{
				this->value = value;
			};
			void Add(GLint progHandle);

		private:

			GLfloat value;
		};

		class CGLSLParameterTabFloat : public CGLSLParameter
		{
		public:
			CGLSLParameterTabFloat(){
				dataType = 4;
				value = nullptr;
			};
			~CGLSLParameterTabFloat()
			{
				Safe_Delete();
			};

			void SetValue(GLfloat * value, int size)
			{
				Safe_Delete();
				this->value = new  GLfloat[size];
				memcpy(this->value, value, size * sizeof(GLfloat));

				this->size = size;
			};
			void Add(GLint progHandle);

		private:

			void Safe_Delete()
			{
				if (value != nullptr)
					delete[] value;

				value = nullptr;
			}

			GLfloat * value;
			int size;
		};

		class CGLSLParameterTabInt : public CGLSLParameter
		{
		public:
			CGLSLParameterTabInt(){
				dataType = 5;
				value = nullptr;
			};
			~CGLSLParameterTabInt()
			{
				Safe_Delete();
			};

			void SetValue(GLint * value, int size)
			{
				Safe_Delete();
				this->value = new  GLint[size];
				memcpy(this->value,value,size * sizeof(GLint));
				this->size = size;
			};
			void Add(GLint progHandle);

		private:

			void Safe_Delete()
			{
				if (value != nullptr)
					delete[] value;

				value = nullptr;
			}

			GLint * value;
			int size;
		};


		class CGLSLParameterTexture : public CGLSLParameter
		{
		public:
			CGLSLParameterTexture(){ dataType = 6; };
			~CGLSLParameterTexture(){};

			void SetValue(const GLint &value)
			{
				this->nTextureID_i = value;
			};

			void Add(GLint progHandle);

		protected:

			GLint nTextureID_i;
		};

	}
}
