#pragma once

class CPhotoCriteria
{
public:
	CPhotoCriteria();
	~CPhotoCriteria();

	void SetPhotoId(const int& numPhotoId);
	int GetPhotoId();

	void SetCriteriaId(const int& numCriteriaId);
	int GetCriteriaId();

private:
	int numPhotoId;
	int numCriteriaId;
};

using PhotoCriteriaVector = std::vector<CPhotoCriteria>;
