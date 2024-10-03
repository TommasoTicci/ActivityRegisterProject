//
// Created by Tommaso Ticci on 03/10/2024.
//

#ifndef ACTIVITYREGISTERPROJECT_ACTIVITY_H
#define ACTIVITYREGISTERPROJECT_ACTIVITY_H

#include <string>
#include <utility>
#include <QTime>

class Activity {
public:
    Activity (std::string &description, QTime &start_time, QTime &end_time , QDate &d): description(description), startTime(start_time), endTime(end_time), date(d) {};
    Activity () = default;

    std::string getDescription() const;
    QTime getStartTime() const;
    QTime getEndTime() const;
    QDate getDate() const;

    void setDescription(std::string &text);
    void setStartTime(QTime &time);
    void setEndTime(QTime &time);
    void setDate(QDate &d);

private:
    std::string description;
    QTime startTime;
    QTime endTime;
    QDate date;
};

#endif //ACTIVITYREGISTERPROJECT_ACTIVITY_H
