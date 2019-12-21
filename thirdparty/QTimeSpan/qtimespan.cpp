#include "qtimespan.hpp"

unsigned long  QTimeSpan::hours() const { return _hours; }
unsigned char  QTimeSpan::minutes() const { return _minutes; }
unsigned char  QTimeSpan::seconds() const { return _seconds; }
unsigned short QTimeSpan::milliseconds() const { return _milliseconds; }


void QTimeSpan::Init()
{
    this->_hours = 0;
    this->_minutes = 0;
    this->_seconds = 0;
    this->_milliseconds = 0;
}


QTimeSpan::QTimeSpan()
{
    Init();
}

QTimeSpan::QTimeSpan(unsigned long hours)
{
    Init();
    AddHours(hours);
}

QTimeSpan::QTimeSpan(unsigned long hours, unsigned long minutes)
{
    Init();
    AddHours(hours);
    AddMinutes(minutes);
}

QTimeSpan::QTimeSpan(unsigned long hours, unsigned long minutes, unsigned long seconds)
{
    Init();
    AddHours(hours);
    AddMinutes(minutes);
    AddSeconds(seconds);
}

QTimeSpan::QTimeSpan(unsigned long hours, unsigned long minutes, unsigned long seconds, unsigned long milliseconds)
{
    Init();
    AddHours(hours);
    AddMinutes(minutes);
    AddSeconds(seconds);
    AddMilliseconds(milliseconds);
}

QTimeSpan::QTimeSpan(const QTimeSpan &timeSpan)
{
    this->_hours = timeSpan.hours();
    this->_minutes = timeSpan.minutes();
    this->_seconds = timeSpan.seconds();
    this->_milliseconds = timeSpan.milliseconds();
}


void QTimeSpan::AddHours(unsigned long hours)
{
    this->_hours += hours;
}

void QTimeSpan::AddMinutes(unsigned long minutes)
{
    auto buff = minutes + this->_minutes;

    this->_minutes = static_cast<unsigned char>(buff%60);
    AddHours(buff/60);
}

void QTimeSpan::AddSeconds(unsigned long seconds)
{
    auto buff = seconds + this->_seconds;

    this->_seconds = static_cast<unsigned char>(buff%60);
    AddMinutes(buff/60);
}

void QTimeSpan::AddMilliseconds(unsigned long milliseconds)
{
    auto buff = milliseconds + this->_milliseconds;

    this->_milliseconds = static_cast<unsigned short>(buff%1000);
    AddSeconds(buff/1000);
}


void QTimeSpan::SubHours(unsigned long hours)
{
    if (this->_hours > hours) {
        this->_hours -= hours;
    }
    else {
        if (this->_hours < hours) {
            this->_minutes = 0;
            this->_seconds = 0;
            this->_milliseconds = 0;
        }

        this->_hours = 0;
    }
}

void QTimeSpan::SubMinutes(unsigned long minutes)
{
    if (this->_minutes < minutes) {
        auto buff = minutes - this->_minutes;
        this->_minutes = 60-buff%60;
        SubHours(1+(buff)/60);
    }
    else
        this->_minutes = this->_minutes > minutes ? this->_minutes - static_cast<unsigned char>(minutes) : 0;
}

void QTimeSpan::SubSeconds(unsigned long seconds)
{
    if (this->_seconds < seconds) {
        auto buff = seconds - this->_seconds;
        this->_seconds = 60-buff%60;
        SubMinutes(1+(buff)/60);
    }
    else
        this->_seconds = this->_seconds > seconds ? this->_seconds - static_cast<unsigned char>(seconds) : 0;
}

void QTimeSpan::SubMilliseconds(unsigned long milliseconds)
{
    if (this->_milliseconds < milliseconds) {
        auto buff = milliseconds - this->_milliseconds;
        this->_milliseconds = 1000-buff%1000;
        SubSeconds(1+(buff)/1000);
    }
    else
        this->_milliseconds = this->_milliseconds > milliseconds ? this->_milliseconds - static_cast<unsigned short>(milliseconds) : 0;
}


QTimeSpan QTimeSpan::operator+(const QTimeSpan &timeSpan)
{
    QTimeSpan res(*this);

    res.AddHours(timeSpan.hours());
    res.AddMinutes(timeSpan.minutes());
    res.AddSeconds(timeSpan.seconds());
    res.AddMilliseconds(timeSpan.milliseconds());

    return res;
}

QTimeSpan QTimeSpan::operator-(const QTimeSpan &timeSpan)
{
    QTimeSpan res(*this);

    res.SubMilliseconds(timeSpan.milliseconds());
    res.SubSeconds(timeSpan.seconds());
    res.SubMinutes(timeSpan.minutes());
    res.SubHours(timeSpan.hours());

    return res;
}


std::ostream& operator<<(std::ostream &os, const QTimeSpan &timeSpan)
{
    if (timeSpan.hours() < 10) os << '0';
    os << timeSpan.hours() << ':';

    if (timeSpan.minutes() < 10) os << '0';
    os << int(timeSpan.minutes()) << ':';

    if (timeSpan.seconds() < 10) os << '0';
    os << int(timeSpan.seconds()) << ':';

    if (timeSpan.milliseconds() < 10) os << "00";
    os << timeSpan.milliseconds();

    return os;
}
