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

	int GetDayOfWeek()
	{
		wxDateTime date(day, static_cast<wxDateTime::Month>(month - 1), year);
		return date.GetWeekDay();
	}

	int day;
	int month;
	int year;
	int dayOfWeek;

	wxString monthName;
	wxString dayName;
	wxString gpsInfos;
	wxString path;

private:
	int numId;
	int numFolderId;
	
	wxString createDate;

	int criteriaInsert;
};


using PhotosVector = std::deque<CPhotos>;
