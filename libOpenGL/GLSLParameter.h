#pragma once
#include "GLcontext.h"
#include <cstring>

namespace Regards
{
	namespace OpenGL
	{
		class CGLSLParameter
		{
		public:
			CGLSLParameter(){ dataType = 0; };
			virtual ~CGLSLParameter(){};

			void SetLibelle(const string &libelle);
			virtual void Add(GLint progHandle){};

			unsigned int GetType(){
				return  dataType;
			};

		protected:

			unsigned int dataType;
			string libelle;
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
				value = NULL;
			};
			~CGLSLParameterTabFloat()
			{
				Safe_Delete();
			};

			void SetValue(GLfloat * value, int size)
			{
				Safe_Delete();
				this->value = new  GLfloat[size];
				std::memcpy(this->value, value, size * sizeof(GLfloat));

				this->size = size;
			};
			void Add(GLint progHandle);

		private:

			void Safe_Delete()
			{
				if (value != NULL)
					delete[] value;

				value = NULL;
			}

			GLfloat * value;
			int size;
		};

		class CGLSLParameterTabInt : public CGLSLParameter
		{
		public:
			CGLSLParameterTabInt(){
				dataType = 5;
				value = NULL;
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
				if (value != NULL)
					delete[] value;

				value = NULL;
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
