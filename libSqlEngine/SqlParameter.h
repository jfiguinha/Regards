#pragma once
#include <sqlite3.h>
namespace Regards
{
    namespace Sqlite
    {
        class CSqlParameter
        {
        public:
            explicit CSqlParameter()
            {}

            virtual ~CSqlParameter() = default;
            virtual int Bind(sqlite3_stmt* stmt, int index) const = 0;
        };

        class CSqlInt : public CSqlParameter
        {
        public:
            explicit CSqlInt(int v = 0)
                : CSqlParameter(),
                value(v)
            {}
            int Bind(sqlite3_stmt* stmt, int index) const override
            {
                return sqlite3_bind_int64(
                    stmt,
                    index,
                    value);
            }
            int value;
        };

        class CSqlString : public CSqlParameter
        {
        public:
            explicit CSqlString(const wxString& v = "")
                : CSqlParameter(),
                value(v)
            {}
            int Bind(sqlite3_stmt* stmt, int index) const override
            {
                wxCharBuffer utf8 = value.ToUTF8();

                return sqlite3_bind_text(
                    stmt,
                    index,
                    utf8.data(),
                    -1,
                    SQLITE_TRANSIENT);
            }
            wxString value;
        };
    }
}