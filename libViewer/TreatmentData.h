#pragma once
#include <header.h>
#include "ThumbnailBuffer.h"
#include "InfosSeparationBarExplorer.h"
#include "ConvertUtility.h"
#include <IconeList.h>
#include <wx/filename.h>
#include <map>
using namespace Regards::Viewer;

class ISeparatorClass
{
public:
	virtual CInfosSeparationBarExplorer* AddSeparatorBar(PhotosVector* _pictures, CIconeList* iconeListLocal, const wxString& libelle, int& nbElement) = 0;
};

class CTreatmentData
{
public:
	virtual ~CTreatmentData() = default;

	virtual void SortList(vector<std::pair<wxString, PhotosVector*>> & listMap) {};
	virtual void SortList(PhotosVector * listPhotos) {};

	void MainTreatment(InfosSeparationBarVector* listSeparator, CIconeList* iconeListLocal, ISeparatorClass * folder, int& numElement)
	{
		std::map<wxString, PhotosVector *> listMap;
		this->numElement = numElement;
		this->iconeListLocal = iconeListLocal;
		int size = CThumbnailBuffer::GetVectorSize();
		for (int i = 0; i < size; i++)
		{
			
			CPhotos photos = CThumbnailBuffer::GetVectorValue(i);
			UpdateVariable(photos);
			wxString libelle = GenerateLibelle();
			std::map<wxString, PhotosVector*>::iterator it = listMap.find(libelle);
			if (it != listMap.end())
			{
				PhotosVector* listVector = listMap[libelle];
				listVector->push_back(photos);
			}
			else
			{
				PhotosVector* listVector = new PhotosVector();
				listMap[libelle] = listVector;
				listVector->push_back(photos);

			}
		}

		vector<std::pair<wxString, PhotosVector*>> myvector{ listMap.begin(), listMap.end() };
		SortList(myvector);

		for (auto it : myvector)
		{
			wxString libelle = it.first;
			PhotosVector* listVector = it.second;

			SortList(listVector);

			CInfosSeparationBarExplorer* infosSeparationBar = folder->AddSeparatorBar(listVector, iconeListLocal, libelle, numElement);
			if (infosSeparationBar->listElement.size() > 0)
				listSeparator->push_back(infosSeparationBar);
			listVector->clear();
			delete listVector;
			//CreateSeparatorBar(listSeparator, listVector, GenerateLibelle(), folder);
		}
		listMap.clear();
		myvector.clear();
	};

	virtual bool TestParameter(const CPhotos& photos) = 0;
	virtual wxString GenerateLibelle() = 0;
	virtual void UpdateVariable(const CPhotos& photos) = 0;



protected:
	//PhotosVector listPhoto;
	bool first = true;
	int numElement = 0;
	CIconeList* iconeListLocal = nullptr;
};

class CTreatmentDataFolder : public CTreatmentData
{
public:
	bool TestParameter(const CPhotos& photos) override
	{
		wxFileName fname(photos.path);
		return fname.GetPath() != dirName;
	};

	wxString GenerateLibelle() override
	{
		return dirName;
	}

	// Comparator function to sort pairs 
// according to second value 
	static bool cmp(pair<wxString, PhotosVector*>& a, pair<wxString, PhotosVector*>& b)
	{
		return a.first < b.first;
	}

	static bool cmp_path(CPhotos & a, CPhotos & b)
	{
		return a.GetPath() < b.GetPath();
	}

	void SortList(PhotosVector* listPhotos) override
	{
		std::sort(listPhotos->begin(), listPhotos->end(), cmp_path);
	}

	void SortList(vector<std::pair<wxString, PhotosVector*>>& listMap) override
	{
		std::sort(listMap.begin(), listMap.end(), cmp);
	}

	void UpdateVariable(const CPhotos& photos) override
	{
		wxFileName fname(photos.path);
		dirName = fname.GetPath();
	}

private:
	wxString dirName = "";
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
