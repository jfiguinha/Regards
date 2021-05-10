#pragma once
#include <header.h>
#include "ThumbnailFolder.h"
using namespace Regards::Viewer;

class CTreatmentData
{
public:

	void MainTreatment(const PhotosVector * photoVector, PhotosVector * newPhotosVectorList, CThumbnailFolder* folder, int & numElement)
	{
		this->numElement = numElement;
		this->newPhotosVectorList = newPhotosVectorList;

		for (CPhotos photos : *photoVector)
		{
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
		int nbElement = 0;
		copy(listPhoto.begin(), listPhoto.end(), back_inserter(*newPhotosVectorList));
		folder->AddSeparatorBar(libelle, &listPhoto, numElement);
		listPhoto.clear();
	};

protected:
	PhotosVector* newPhotosVectorList;
	PhotosVector listPhoto;
	bool first = true;
	int numElement;
};

class CTreatmentDataYear : public CTreatmentData
{
public:

	virtual bool TestParameter(const CPhotos& photos)
	{
		return photos.year != year;
	};
	virtual wxString GenerateLibelle()
	{
		return to_string(year);
	}

	virtual void UpdateVariable(const CPhotos& photos)
	{
		year = photos.year;
	}

private:
	int year = 0;
};

class CTreatmentDataMonth : public CTreatmentData
{
public:

	virtual bool TestParameter(const CPhotos& photos)
	{
		return photos.year != year || photos.month != month;
	};
	virtual wxString GenerateLibelle()
	{
		return monthName + L" " + to_string(year);
	}

	virtual void UpdateVariable(const CPhotos& photos)
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

	virtual bool TestParameter(const CPhotos& photos)
	{
		return photos.year != year || photos.month != month || photos.day != day;
	};
	virtual wxString GenerateLibelle()
	{
		return dayName + L" " + to_string(day) + L" " + monthName + L" , " + to_string(year) + L"@";
	}

	virtual void UpdateVariable(const CPhotos& photos)
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

	virtual bool TestParameter(const CPhotos& photos)
	{
		return photos.year != year || photos.month != month || photos.day != day || photos.gpsInfos != libelleLocalisation;
	};
	virtual wxString GenerateLibelle()
	{
		wxString output = "";
		vector<wxString> libelle = CConvertUtility::split(libelleLocalisation, '.');

		for (auto j = (int)libelle.size() - 1; j > 0; j--)
		{
			output.append(libelle.at(j));
			if (j > 1)
				output.append(L", ");
		}

		if (output == "")
			output = libelleLocalisation;

		return dayName + L" " + to_string(day) + L" " + monthName + L" , " + to_string(year) + L"@" + output;
	}

	virtual void UpdateVariable(const CPhotos& photos)
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