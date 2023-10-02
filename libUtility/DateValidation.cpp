#include "header.h"
#include "DateValidation.hpp"

using namespace std;


const int DateValidation::DAYS_IN_MONTH[] = {
    31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};


DateValidation::DateValidation(string date)
{
    this->date = date;
    this->year = date.substr(0, 4);
    this->month = date.substr(5, 2);
    this->day = date.substr(8, 2);
}


bool DateValidation::isDigit()
{
    unsigned int i;
    for (i = 0; i < year.length(); i++)
        if (isdigit(year[i]) == 0) return false;
    for (i = 0; i < month.length(); i++)
        if (isdigit(month[i]) == 0) return false;
    for (i = 0; i < day.length(); i++)
        if (isdigit(day[i]) == 0) return false;
    return true;
}


bool DateValidation::isLeapYear(int y)
{
    return (y % 400 == 0) || ((y % 4 == 0) && (y % 100 != 0));
}

int DateValidation::daysInMonth(int y, int m)
{
    if (m < 1 || 12 < m) return 0;
    if (m == 2 && isLeapYear(y) == true) return 29;
    return DAYS_IN_MONTH[m - 1];
}

bool DateValidation::isValid()
{
    int y = atoi(year.c_str());
    int m = atoi(month.c_str());
    int d = atoi(day.c_str());
    if (!isDigit()) return false;
    if (m < 1 || m > 12 || d < 1 || d > daysInMonth(y, m)) return false;
    return true;  // Valid
}