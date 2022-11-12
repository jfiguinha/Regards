#pragma once
#include <cstring>

namespace Regards
{
	namespace OpenGL
	{
		class CGLSLParameter
		{
		public:
			CGLSLParameter() { dataType = 0; };

			virtual ~CGLSLParameter()
			{
			};

			void SetLibelle(const string& libelle);

			virtual void Add(GLint progHandle)
			{
			};

			unsigned int GetType()
			{
				return dataType;
			};

		protected:
			unsigned int dataType;
			string libelle;
		};

		class CGLSLParameterInt : public CGLSLParameter
		{
		public:
			CGLSLParameterInt(): value(0)
			{
				dataType = 1;
			}
			;

			~CGLSLParameterInt() override
			{
			};

			void SetValue(const GLint& value)
			{
				this->value = value;
			};
			void Add(GLint progHandle) override;

		private:
			GLint value;
		};

		class CGLSLParameterUInt : public CGLSLParameter
		{
		public:
			CGLSLParameterUInt(): value(0)
			{
				dataType = 2;
			}
			;

			~CGLSLParameterUInt() override
			{
			};

			void SetValue(const GLuint& value)
			{
				this->value = value;
			};

			void Add(GLint progHandle) override;

		private:
			GLuint value;
		};

		class CGLSLParameterFloat : public CGLSLParameter
		{
		public:
			CGLSLParameterFloat(): value(0)
			{
				dataType = 3;
			}
			;

			~CGLSLParameterFloat() override
			{
			};

			void SetValue(const GLfloat& value)
			{
				this->value = value;
			};
			void Add(GLint progHandle) override;

		private:
			GLfloat value;
		};

		class CGLSLParameterTabFloat : public CGLSLParameter
		{
		public:
			CGLSLParameterTabFloat(): size(0)
			{
				dataType = 4;
				value = nullptr;
			}
			;

			~CGLSLParameterTabFloat() override
			{
				Safe_Delete();
			};

			void SetValue(GLfloat* value, int size)
			{
				Safe_Delete();
				this->value = new GLfloat[size];
				std::memcpy(this->value, value, size * sizeof(GLfloat));

				this->size = size;
			};
			void Add(GLint progHandle) override;

		private:
			void Safe_Delete()
			{
				if (value != nullptr)
					delete[] value;

				value = nullptr;
			}

			GLfloat* value;
			int size;
		};

		class CGLSLParameterTabInt : public CGLSLParameter
		{
		public:
			CGLSLParameterTabInt(): size(0)
			{
				dataType = 5;
				value = nullptr;
			}
			;

			~CGLSLParameterTabInt() override
			{
				Safe_Delete();
			};

			void SetValue(GLint* value, int size)
			{
				Safe_Delete();
				this->value = new GLint[size];
				memcpy(this->value, value, size * sizeof(GLint));
				this->size = size;
			};
			void Add(GLint progHandle) override;

		private:
			void Safe_Delete()
			{
				if (value != nullptr)
					delete[] value;

				value = nullptr;
			}

			GLint* value;
			int size;
		};


		class CGLSLParameterTexture : public CGLSLParameter
		{
		public:
			CGLSLParameterTexture(): nTextureID_i(0) { dataType = 6; } ;

			~CGLSLParameterTexture() override
			{
			};

			void SetValue(const GLint& value)
			{
				this->nTextureID_i = value;
			};

			void Add(GLint progHandle) override;

		protected:
			GLint nTextureID_i;
		};
	}
}
