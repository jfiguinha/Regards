#pragma once
#include <header.h>
#include "ThumbnailBuffer.h"
#include "InfosSeparationBarExplorer.h"
#include "ConvertUtility.h"
#include <IconeList.h>
#include <wx/filename.h>
#include <map>
#include <memory>
using namespace Regards::Window;


class ISeparatorClass
{
public:
	// Retourne un unique_ptr : l'appelant (MainTreatment) devient propriétaire
	// et le déplace ensuite dans InfosSeparationBarVector (vector de unique_ptr).
	virtual std::unique_ptr<CInfosSeparationBarExplorer> AddSeparatorBar(PhotosVector* _pictures, CIconeList* iconeListLocal, const wxString& libelle, int& nbElement) = 0;
};

class CTreatmentData
{
public:
	virtual ~CTreatmentData() = default;
	virtual wxString GenerateValue() = 0;
	virtual void SortList(PhotosVector* listPhotos) {};

	static bool cmp(pair<wxString, PhotosVector*>& a, pair<wxString, PhotosVector*>& b)
	{
		return a.first > b.first;
	}

	virtual void SortList(vector<std::pair<wxString, PhotosVector*>>& listMap)
	{
		std::sort(listMap.begin(), listMap.end(), cmp);
	}

	virtual void SortList(InfosSeparationBarVector* listSeparator)
	{

	}

	void MainTreatment(InfosSeparationBarVector* listSeparator, CIconeList* iconeListLocal, ISeparatorClass* folder, int& numElement)
	{
		// Les PhotosVector sont possédés par la map via unique_ptr : plus de new/delete
		// manuels, et plus de fuite possible si une exception survient en cours de route.
		std::map<wxString, std::unique_ptr<PhotosVector>> listMap;
		std::map<wxString, wxString> listLibelle;
		this->numElement = numElement;

		int size = CThumbnailBuffer::GetVectorSize();
		for (int i = 0; i < size; i++)
		{

			CPhotos photos = CThumbnailBuffer::GetVectorValue(i);
			UpdateVariable(photos);

			wxString libelle = GenerateLibelle();
			wxString value = GenerateValue();

			auto it = listMap.find(value);
			if (it != listMap.end())
			{
				it->second->push_back(photos);
			}
			else
			{
				auto listVector = std::make_unique<PhotosVector>();
				listVector->push_back(photos);
				listLibelle[value] = libelle;
				listMap[value] = std::move(listVector);
			}
		}

		// myvector ne fait que référencer les PhotosVector toujours possédés par listMap ;
		// il sert uniquement au tri, pas à la propriété.
		vector<std::pair<wxString, PhotosVector*>> myvector;
		myvector.reserve(listMap.size());
		for (auto& entry : listMap)
			myvector.emplace_back(entry.first, entry.second.get());

		SortList(myvector);

		for (auto it : myvector)
		{
			wxString value = it.first;
			PhotosVector* listVector = it.second;

			SortList(listVector);

			auto infosSeparationBar = folder->AddSeparatorBar(listVector, iconeListLocal, listLibelle[value], numElement);
			infosSeparationBar->SetLongTitle(value);
			if (infosSeparationBar->listElement.size() > 0)
				// Conversion implicite unique_ptr<CInfosSeparationBarExplorer> -> unique_ptr<CInfosSeparationBar>
				listSeparator->push_back(std::move(infosSeparationBar));
			//CreateSeparatorBar(listSeparator, listVector, GenerateLibelle(), folder);
		}

		SortList(listSeparator);
		// listMap se vide automatiquement ici (sortie de portée) : chaque PhotosVector
		// encapsulé dans un unique_ptr est détruit sans appel explicite à delete.
	};

	virtual bool TestParameter(const CPhotos& photos) = 0;
	virtual wxString GenerateLibelle() = 0;
	virtual void UpdateVariable(const CPhotos& photos) = 0;



protected:
	//PhotosVector listPhoto;
	bool first = true;
	int numElement = 0;

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
		wxString seg = fname.GetDirs().back();
		return seg;
	}

	wxString GenerateValue() override
	{
		return dirName;
	}


	static bool cmp_path(CPhotos& a, CPhotos& b)
	{
		return a.GetPath() < b.GetPath();
	}

	void SortList(PhotosVector* listPhotos) override
	{
		std::sort(listPhotos->begin(), listPhotos->end(), cmp_path);
	}

	void UpdateVariable(const CPhotos& photos) override
	{
		fname = wxFileName(photos.path);
		dirName = fname.GetPath();
	}

	static bool cmp_path_separator(const std::unique_ptr<CInfosSeparationBar>& a, const std::unique_ptr<CInfosSeparationBar>& b)
	{
		return a->GetLongTitle() < b->GetLongTitle();
	}

	void SortList(InfosSeparationBarVector* listSeparator) override
	{
		std::sort(listSeparator->begin(), listSeparator->end(), cmp_path_separator);
	}

private:
	wxString dirName = "";
	wxFileName fname;
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

	wxString GenerateValue() override
	{
		return CConvertUtility::GenerateValue(year, 4);
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
	}

	wxString GenerateLibelle() override
	{
		return monthName + L" " + to_string(year);
	}

	wxString GenerateValue() override
	{
		return CConvertUtility::GenerateValue(year, 4) + CConvertUtility::GenerateValue(month, 2);
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

	wxString GenerateValue() override
	{
		return CConvertUtility::GenerateValue(year, 4) + CConvertUtility::GenerateValue(month, 2) + CConvertUtility::GenerateValue(day, 2);
	}

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

	wxString GenerateValue() override
	{
		return CConvertUtility::GenerateValue(year, 4) + CConvertUtility::GenerateValue(month, 2) + CConvertUtility::GenerateValue(day, 2) + "@" + libelleLocalisation;
	}

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