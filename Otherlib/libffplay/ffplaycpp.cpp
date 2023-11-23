#include "header.h"
#include "ffplaycpp.h"
#include "ffplay.h"

class CPimplFfplay
{
public:
	CPimplFfplay() 
	{
		ffplay = new CFFPlay();
	}

	~CPimplFfplay()
	{
		delete ffplay;
	}

	void StartMovie(const wxString& filename)
	{
		ffplay->PlayMovie(filename.ToStdString());
	}

private:
	CFFPlay* ffplay;
};

CFFPlayCpp::CFFPlayCpp()
{
	pimpl = new CPimplFfplay();

}
CFFPlayCpp::~CFFPlayCpp()
{
	delete pimpl;
}

void CFFPlayCpp::StartMovie(const wxString& filename)
{
	pimpl->StartMovie(filename);
}