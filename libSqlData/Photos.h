#pragma once

class CPhotos
{
public:
	CPhotos();
	~CPhotos();

	void SetId(const int& numId);
	int GetId();

	void SetFolderId(const int& numId);
	int GetFolderId();

	void SetPath(const wxString& path);
	wxString GetPath();

	void SetGpsInfos(const wxString& gpsInfos);
	wxString GetGpsInfos();

	void SetCreateDate(const wxString& createDate);
	wxString GetCreateDate();

	void SetIsCriteriaInsert(const int& criteriaInsert);
	int GetIsCriteriaInsert();

	int day;
	int month;
	int year;
	int dayofweek;
	wxString monthName;
	wxString dayName;
	wxString gpsInfos;

private:
	int numId;
	int numFolderId;
	wxString path;
	wxString createDate;

	int criteriaInsert;
};


using PhotosVector = std::vector<CPhotos>;
