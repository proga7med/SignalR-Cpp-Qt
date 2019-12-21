#ifndef QTIMESPAN_HPP
#define QTIMESPAN_HPP

#include <iostream>

class QTimeSpan
{
    unsigned long  _hours;
    unsigned char  _minutes;
    unsigned char  _seconds;
    unsigned short _milliseconds;

    void Init ();

public:

    QTimeSpan();
    QTimeSpan(unsigned long hours);
    QTimeSpan(unsigned long hours, unsigned long minutes);
    QTimeSpan(unsigned long hours, unsigned long minutes, unsigned long seconds);
    QTimeSpan(unsigned long hours, unsigned long minutes, unsigned long seconds, unsigned long milliseconds);
    QTimeSpan(const QTimeSpan &timeSpan);

    unsigned long  hours() const;
    unsigned char  minutes() const;
    unsigned char  seconds() const;
    unsigned short milliseconds() const;

    void AddHours(unsigned long hours);
    void AddMinutes(unsigned long minutes);
    void AddSeconds(unsigned long seconds);
    void AddMilliseconds(unsigned long milliseconds);

    void SubHours(unsigned long hours);
    void SubMinutes(unsigned long minutes);
    void SubSeconds(unsigned long seconds);
    void SubMilliseconds(unsigned long milliseconds);

    QTimeSpan operator+(const QTimeSpan &timeSpan);
    QTimeSpan operator-(const QTimeSpan &timeSpan);

    friend std::ostream& operator<<(std::ostream &os, const QTimeSpan &timeSpan);
};

#endif // QTIMESPAN_H
