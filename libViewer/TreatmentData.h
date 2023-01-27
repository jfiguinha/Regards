#pragma once
#include <header.h>
#include "ThumbnailBuffer.h"
#include "ConvertUtility.h"
#include "ThumbnailFolder.h"
using namespace Regards::Viewer;

class CTreatmentData
{
public:
	virtual ~CTreatmentData() = default;

	void MainTreatment(CIconeList* iconeListLocal, CThumbnailFolder* folder,
	                   int& numElement)
	{
		this->numElement = numElement;
		this->iconeListLocal = iconeListLocal;
		int size = CThumbnailBuffer::GetVectorSize();
		for (int i = 0;i < size;i++)
		{
			CPhotos photos = CThumbnailBuffer::GetVectorValue(i);
			if (TestParameter(photos))
			{
				if (!first)
				{
					CreateSeparatorBar(GenerateLibelle(), folder);
				}
				first = false;
				UpdateVariable(photos);
			}
			listPhoto.push_back(photos);
		}

		CreateSeparatorBar(GenerateLibelle(), folder);
	};

	virtual bool TestParameter(const CPhotos& photos) = 0;
	virtual wxString GenerateLibelle() = 0;
	virtual void UpdateVariable(const CPhotos& photos) = 0;

	void CreateSeparatorBar(const wxString& libelle, CThumbnailFolder* folder)
	{
		//int nbElement = 0;
		//copy(listPhoto.begin(), listPhoto.end(), back_inserter(*newPhotosVectorList));
		folder->AddSeparatorBar(iconeListLocal, libelle, numElement);
		listPhoto.clear();
	};

protected:
	PhotosVector listPhoto;
	bool first = true;
	int numElement = 0;
	CIconeList* iconeListLocal = nullptr;
};

class CTreatmentDataYear : public CTreatmentData
{
public:
	bool TestParameter(const CPhotos& photos) override
	{
		return photos.year != year;
	};

	wxString GenerateLibelle() override
	{
		return to_string(year);
	}

	void UpdateVariable(const CPhotos& photos) override
	{
		year = photos.year;
	}

private:
	int year = 0;
};

class CTreatmentDataMonth : public CTreatmentData
{
public:
	bool TestParameter(const CPhotos& photos) override
	{
		return photos.year != year || photos.month != month;
	};

	wxString GenerateLibelle() override
	{
		return monthName + L" " + to_string(year);
	}

	void UpdateVariable(const CPhotos& photos) override
	{
		year = photos.year;
		month = photos.month;
		monthName = photos.monthName;
	}

private:
	int year = 0;
	int month = 0;
	wxString monthName = "";
};

class CTreatmentDataDay : public CTreatmentData
{
public:
	bool TestParameter(const CPhotos& photos) override
	{
		return photos.year != year || photos.month != month || photos.day != day;
	};

	wxString GenerateLibelle() override
	{
		return dayName + L" " + to_string(day) + L" " + monthName + L" , " + to_string(year) + L"@";
	}

	void UpdateVariable(const CPhotos& photos) override
	{
		year = photos.year;
		month = photos.month;
		day = photos.day;
		monthName = photos.monthName;
		dayName = photos.dayName;
	}

private:
	int year = 0;
	int month = 0;
	int day = 0;
	wxString monthName = "";
	wxString dayName = "";
};

class CTreatmentDataLocalisation : public CTreatmentData
{
public:
	bool TestParameter(const CPhotos& photos) override
	{
		return photos.year != year || photos.month != month || photos.day != day || photos.gpsInfos !=
			libelleLocalisation;
	};

	wxString GenerateLibelle() override
	{
		wxString output = "";
		vector<wxString> libelle = CConvertUtility::split(libelleLocalisation, '.');

		for (auto j = static_cast<int>(libelle.size()) - 1; j > 0; j--)
		{
			output.append(libelle.at(j));
			if (j > 1)
				output.append(L", ");
		}

		if (output == "")
			output = libelleLocalisation;

		return dayName + L" " + to_string(day) + L" " + monthName + L" , " + to_string(year) + L"@" + output;
	}

	void UpdateVariable(const CPhotos& photos) override
	{
		year = photos.year;
		month = photos.month;
		day = photos.day;
		monthName = photos.monthName;
		dayName = photos.dayName;
		libelleLocalisation = photos.gpsInfos;
	}

private:
	int year = 0;
	int month = 0;
	int day = 0;
	wxString monthName = "";
	wxString dayName = "";
	wxString libelleLocalisation = L"";
};
