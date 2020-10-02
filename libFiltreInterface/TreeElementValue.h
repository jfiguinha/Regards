#pragma once

namespace Regards
{
	namespace Window
	{

#define TYPE_ELEMENT_INT 1
#define TYPE_ELEMENT_FLOAT 2
#define TYPE_ELEMENT_BOOL 3

		class CTreeElementValue
		{
		public:
			CTreeElementValue() {};
			virtual ~CTreeElementValue() {};

			virtual int GetType() {
				return 0;
			};

			virtual wxString GetStringValue()
			{
				return "";
			};



		};


		class CTreeElementValueInt : public CTreeElementValue
		{
		public:
			CTreeElementValueInt(const int &value) { this->value = value; };
			~CTreeElementValueInt() {};
			bool operator==(const CTreeElementValueInt &other)
			{
				if (value == other.value)
					return true;
				return false;
			}

			int GetValue()
			{
				return value;
			}

			int GetType()
			{
				return TYPE_ELEMENT_INT;
			}

			void SetValue(const int &value)
			{
				this->value = value;
			}

			wxString GetStringValue()
			{
				return to_string(value);
			}

		private:
			int value;
		};

		class CTreeElementValueFloat : public CTreeElementValue
		{
		public:
			CTreeElementValueFloat(const float &value, const int &nbPointElement = 2) {
				this->value = value;
				this->nbPointElement = nbPointElement;
			};
			~CTreeElementValueFloat() {};
			bool operator==(const CTreeElementValueFloat &other)
			{
				if (value == other.value)
					return true;
				return false;
			}

			float GetValue()
			{
				return value;
			}

			void SetValue(const float &value)
			{
				this->value = value;
			}

			int GetType()
			{
				return TYPE_ELEMENT_FLOAT;
			}

			wxString GetStringValue()
			{
				wxString libelle = to_string(value);
				int pos = libelle.find(".");
				if (pos > 0)
				{
					wxString floatLibelle = libelle.substr(0, pos) + "." + libelle.substr(pos + 1, nbPointElement);
					return floatLibelle;
				}
				return libelle;
			}

		private:
			int nbPointElement;
			float value;
		};

		class CTreeElementValueBool : public CTreeElementValue
		{
		public:
			CTreeElementValueBool(const bool &value) { this->value = value; };
			~CTreeElementValueBool() {};
			bool operator==(const CTreeElementValueBool &other)
			{
				if (value == other.value)
					return true;
				return false;
			}

			bool GetValue()
			{
				return value;
			}

			void SetValue(const bool &value)
			{
				this->value = value;
			}

			int GetType()
			{
				return TYPE_ELEMENT_BOOL;
			}

			wxString GetStringValue()
			{
				return to_string(value);
			}

		private:
			bool value;
		};
	}
}