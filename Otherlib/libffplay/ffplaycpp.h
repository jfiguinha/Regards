#pragma once

class CPimplFfplay;

class CFFPlayCpp
{
public:
	CFFPlayCpp();
	~CFFPlayCpp();
	void StartMovie(const wxString &filename);
private:
	CPimplFfplay* pimpl = nullptr;
};