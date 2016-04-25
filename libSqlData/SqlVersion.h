//
//  SqlVersion.hpp
//  Regards.libSqlData
//
//  Created by figuinha jacques on 29/09/2015.
//  Copyright © 2015 figuinha jacques. All rights reserved.
//
#include "SqlExecuteRequest.h"
#include "SqlResult.h"

namespace Regards
{
    namespace Sqlite
    {
        class CSqlVersion : public CSqlExecuteRequest
        {
        public:
            CSqlVersion(CSqlLib * _sqlLibTransaction = nullptr, const bool &useTransaction = 0);
            ~CSqlVersion();
            bool InsertVersion(const wxString & version);
            bool UpdateVersion(const wxString & version, const wxString &oldValue);
            bool DeleteVersion();
            wxString GetVersion();
            
        private:
            
            int TraitementResult(CSqlResult * sqlResult);
            int typeResult = 0;
            wxString result = "";
        };
    }
}
