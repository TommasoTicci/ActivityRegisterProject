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
    Activity(const std::string &description, const QTime &start_time, const QTime &end_time, const QDate &date)
            : description(description), startTime(start_time), endTime(end_time), date(date) {
        if (!isValidInput()) {
            throw std::invalid_argument("Invalid input");
        }
    }
    Activity() = default;

    std::string getDescription() const;
    QTime getStartTime() const;
    QTime getEndTime() const;
    QDate getDate() const;

    bool isValidInput () const;

    void setDescription(std::string &text);
    void setStartTime(QTime &time);
    void setEndTime(QTime &time);
    void setDate(QDate &d);

    void modifyActivity(std::string &text, QTime &s_time, QTime &e_time, QDate &d);

    bool isEqual(const Activity &activity) const;

private:
    std::string description;
    QTime startTime;
    QTime endTime;
    QDate date;
};

#endif //ACTIVITYREGISTERPROJECT_ACTIVITY_H
