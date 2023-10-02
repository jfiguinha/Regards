#pragma once

class DateValidation
{
    static const int DAYS_IN_MONTH[];
    std::string date, year, month, day;
    bool isLeapYear(int);
    int  daysInMonth(int, int);
    bool isDigit();

public:
    DateValidation(std::string);  //! コンストラクタ
    bool isValid();           //! 妥当性チェック
};
