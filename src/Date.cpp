#include "Date.hpp"
#include <cctype>
#include <sstream>
#include <cstdio>
using namespace std;
/**
* @brief default constructor
*/
bool isFormat(const std::string s);

Date::Date() {
    m_year = 0;
    m_month = 0;
    m_day = 0;
    m_hour = 0;
    m_minute = 0;
};

/**
* @brief constructor with arguments
*/
Date::Date(int t_year, int t_month, int t_day, int t_hour, int t_minute) {
    m_year = t_year;
    m_month = t_month;
    m_day = t_day;
    m_hour = t_hour;
    m_minute = t_minute;
};

/**
* @brief constructor with a string
*/
Date::Date(const std::string &dateString) { *this = stringToDate(dateString); };
/**
* @brief return the year of a Date
* @return   a integer indicate the year of a date
*/
int Date::getYear(void) const { return m_year; };

/**
* @brief set the year of a date
* @param a integer indicate the new year of a date
*/
void Date::setYear(const int t_year) { m_year = t_year; };

/**
* @brief return the month of a Date
* @return   a integer indicate the month of a date
*/
int Date::getMonth(void) const { return m_month; };

/**
* @brief set the month of a date
* @param a integer indicate the new month of a date
*/
void Date::setMonth(const int t_month) { m_month = t_month; };

/**
* @brief return the day of a Date
* @return   a integer indicate the day of a date
*/
int Date::getDay(void) const { return m_day; };

/**
* @brief set the day of a date
* @param a integer indicate the new day of a date
*/
void Date::setDay(const int t_day) { m_day = t_day; };

/**
* @brief return the hour of a Date
* @return   a integer indicate the hour of a date
*/
int Date::getHour(void) const { return m_hour; };

/**
* @brief set the hour of a date
* @param a integer indicate the new hour of a date
*/
void Date::setHour(const int t_hour) { m_hour = t_hour; };

/**
* @brief return the minute of a Date
* @return   a integer indicate the minute of a date
*/
int Date::getMinute(void) const { return m_minute; };

/**
* @brief set the minute of a date
* @param a integer indicate the new minute of a date
*/
void Date::setMinute(const int t_minute) { m_minute = t_minute; };

/**
*   @brief check whether the date is valid or not
*   @return the bool indicate valid or not
*/
//static
bool Date::isValid(const Date &t_date) {
    if (t_date.getYear()<1000 || t_date.getYear()>9999 || t_date.getMonth()<1 || t_date.getMonth()>12)
        return false;
    if (t_date.getHour()<0 || t_date.getHour()>23 || t_date.getMinute()<0 || t_date.getMinute()>59)
        return false;
    //           0  1   2   3   4   5   6   7   8   9   10  11  12 
    int a[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (  (t_date.getYear() % 400 == 0) || (t_date.getYear() % 4 == 0 && t_date.getYear() % 100 != 0)  )
        a[2] = 29;
    if (  !(t_date.getDay() > 0 && t_date.getDay() <= a[t_date.getMonth()])  )
        return false;
    return true;
};

/**
* @brief convert a string to date, if the format is not correct return
* 0000-00-00/00:00
* @return a date
*/
//static
Date Date::stringToDate(const std::string &t_dateString) {
    if(isFormat(t_dateString)) {
        int y, m, d, h, min;
        char t;
        stringstream ss(t_dateString);
        ss >> y >> t >> m >> t >> d >> t >> h >> t >> min;
        return Date(y, m, d, h, min);
    }
    else
        return Date();
};

/**
* @brief convert a date to string, if the format is not correct return
* 0000-00-00/00:00
*/
//static
std::string Date::dateToString(const Date &t_date) {
    string ret;
    if (isValid(t_date)) {
        char ch[18];
        sprintf(ch, "%.4d", t_date.getYear());
        ret += ch;
        ret += '-';
        sprintf(ch, "%.2d", t_date.getMonth());
        ret += ch;
        ret += '-';
        sprintf(ch, "%.2d", t_date.getDay());
        ret += ch;
        ret += '/';
        sprintf(ch, "%.2d", t_date.getHour());
        ret += ch;
        ret += ':';
        sprintf(ch, "%.2d", t_date.getMinute());
        ret += ch;
    }
    else
        ret = "0000-00-00/00:00";
    return ret;
};

/**
*  @brief overload the assign operator
*/
Date & Date::operator=(const Date &t_date) {
    m_year = t_date.m_year;
    m_month = t_date.m_month;
    m_day = t_date.m_day;
    m_hour = t_date.m_hour;
    m_minute = t_date.m_minute;
    return *this;
};

/**
* @brief check whether the CurrentDate is equal to the t_date
*/
bool Date::operator==(const Date &t_date) const {
    return (m_year == t_date.m_year && 
        m_month == t_date.m_month &&
        m_day == t_date.m_day &&
        m_hour == t_date.m_hour &&
        m_minute == t_date.m_minute)
        ? true : false;

};

/**
* @brief check whether the CurrentDate is  greater than the t_date
*/
bool Date::operator>(const Date &t_date) const {
    return ( m_year > t_date.m_year
         || (m_year == t_date.m_year && m_month > t_date.m_month)
         || (m_year == t_date.m_year && m_month == t_date.m_month && m_day > t_date.m_day)
         || (m_year == t_date.m_year && m_month == t_date.m_month && m_day == t_date.m_day && m_hour > t_date.m_hour)
         || (m_year == t_date.m_year && m_month == t_date.m_month && m_day == t_date.m_day && m_hour == t_date.m_hour && m_minute > t_date.m_minute) );
};

/**
* @brief check whether the CurrentDate is  less than the t_date
*/
bool Date::operator<(const Date &t_date) const {
    return (*this > t_date || *this == t_date) ? false : true;
};

/**
* @brief check whether the CurrentDate is  greater or equal than the t_date
*/
bool Date::operator>=(const Date &t_date) const {
    return (*this < t_date) ? false : true;
};

/**
* @brief check whether the CurrentDate is  less than or equal to the t_date
*/
bool Date::operator<=(const Date &t_date) const {
    return (*this > t_date) ? false : true;
};

//0000-00-00/00:00
bool isFormat(const std::string s) {
    return (isdigit(s[0]) &&
        isdigit(s[1]) &&
        isdigit(s[2]) &&
        isdigit(s[3]) &&
        isdigit(s[5]) &&
        isdigit(s[6]) &&
        isdigit(s[8]) &&
        isdigit(s[9]) &&
        isdigit(s[11]) &&
        isdigit(s[12]) &&
        isdigit(s[14]) &&
        isdigit(s[15]) &&
        s[4]=='-' &&
        s[7]=='-' &&
        s[10]=='/'&& 
        s[13]==':');
}