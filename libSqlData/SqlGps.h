//
//  SqlGps.hpp
//  Regards.libSqlData
//
//  Created by figuinha jacques on 29/09/2015.
//  Copyright © 2015 figuinha jacques. All rights reserved.
//

#pragma once
#include "SqlExecuteRequest.h"
#include "SqlResult.h"
#include <PhotoGps.h>

namespace Regards
{
    namespace Sqlite
    {
        class CSqlGps : public CSqlExecuteRequest
        {
        public:
            CSqlGps(CSqlLib * _sqlLibTransaction = nullptr, const bool &useTransaction = 0);
            ~CSqlGps();
            bool InsertGps(const wxString & filepath, const wxString &latitude, const wxString &longitude);
            bool UpdateGps(const wxString & filepath, const wxString &latitude, const wxString &longitude);
            bool DeleteGps(const wxString & filepath);
            void GetGps(PhotoGpsVector * photogpsVector, const wxString & filepath);

        private:
            
            int TraitementResult(CSqlResult * sqlResult);
            int typeResult;
            PhotoGpsVector * photogpsVector;
        };
    }
}
