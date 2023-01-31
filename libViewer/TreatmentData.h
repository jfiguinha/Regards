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

	void MainTreatment(InfosSeparationBarVector * listSeparator, PhotosVector * _pictures, CIconeList* iconeListLocal, CThumbnailFolder* folder, int& numElement)
	{
		this->numElement = numElement;
		this->iconeListLocal = iconeListLocal;
		bool first = true;
		int size = _pictures->size();
		for (int i = 0;i < size;i++)
		{
			CPhotos photos = _pictures->at(i);
			if (TestParameter(photos))
			{
				if (!first)
				{
					CreateSeparatorBar(listSeparator, &listPhoto, GenerateLibelle(), folder);
				}
				first = false;
				UpdateVariable(photos);
			}
			listPhoto.push_back(photos);
		}

		CreateSeparatorBar(listSeparator, &listPhoto, GenerateLibelle(), folder);
	};


	void MainTreatment(InfosSeparationBarVector* listSeparator, CIconeList* iconeListLocal, CThumbnailFolder* folder, int& numElement)
	{
		this->numElement = numElement;
		this->iconeListLocal = iconeListLocal;
		int size = CThumbnailBuffer::GetVectorSize();
		for (int i = 0; i < size; i++)
		{
			CPhotos photos = CThumbnailBuffer::GetVectorValue(i);
			if (TestParameter(photos))
			{
				if (!first)
				{
					CreateSeparatorBar(listSeparator, GenerateLibelle(), folder);
				}
				first = false;
				UpdateVariable(photos);
			}
			listPhoto.push_back(photos);
		}

		CreateSeparatorBar(listSeparator,  GenerateLibelle(), folder);
	};

	virtual bool TestParameter(const CPhotos& photos) = 0;
	virtual wxString GenerateLibelle() = 0;
	virtual void UpdateVariable(const CPhotos& photos) = 0;

	void CreateSeparatorBar(InfosSeparationBarVector * listSeparator, PhotosVector * _pictures, const wxString& libelle, CThumbnailFolder* folder)
	{
		CInfosSeparationBarExplorer* infosSeparationBar = folder->AddSeparatorBar(_pictures, iconeListLocal, libelle, numElement);
		if (infosSeparationBar->listElement.size() > 0)
			listSeparator->push_back(infosSeparationBar);
		listPhoto.clear();
	};

	void CreateSeparatorBar(InfosSeparationBarVector* listSeparator, const wxString& libelle, CThumbnailFolder* folder)
	{
		CInfosSeparationBarExplorer* infosSeparationBar = folder->AddSeparatorBar(&listPhoto, iconeListLocal, libelle, numElement);
		if (infosSeparationBar->listElement.size() > 0)
			listSeparator->push_back(infosSeparationBar);
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
