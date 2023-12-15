#include <header.h>
#include "md5check.h"
#include <SqlThumbnail.h>
#include <SqlThumbnailVideo.h>
#include <window_id.h>
#include <wx/filename.h>
using namespace Regards::Sqlite;

CMd5Check::CMd5Check()
{

}

CMd5Check::~CMd5Check()
{

}

//---------------------------------------------------------------
//
//---------------------------------------------------------------
void CMd5Check::CheckMD5(void* param)
{
	auto path = static_cast<CThreadMD5*>(param);
	if (path != nullptr)
	{
		CSqlThumbnail sqlThumbnail;
		CSqlThumbnailVideo sqlThumbnailVideo;
		wxFileName file(path->filename);
		wxULongLong sizeFile = file.GetSize();
		wxString md5file = sizeFile.ToString();

		bool result = sqlThumbnail.TestThumbnail(path->filename, md5file);
		if (!result)
		{
			//Remove thumbnail
			sqlThumbnail.DeleteThumbnail(path->filename);
			sqlThumbnailVideo.DeleteThumbnail(path->filename);
		}

		wxCommandEvent evt(wxEVENT_MD5CHECKING);
		evt.SetClientData(path);
		path->mainWindow->GetEventHandler()->AddPendingEvent(evt);
	}
}