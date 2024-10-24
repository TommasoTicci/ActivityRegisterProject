//
// Created by Tommaso Ticci on 03/10/2024.
//

#include "Activity.h"

std::string Activity::getDescription() const {
    return description;
}

QTime Activity::getStartTime() const {
    return startTime;
}

QTime Activity::getEndTime() const {
    return endTime;
}

QDate Activity::getDate() const {
    return date;
}

bool Activity::isValidInput() const {
    if (description.empty()) {
        return false;
    }
    if (startTime > endTime) {
        return false;
    }
    return true;
}

void Activity::setDescription(std::string &text) {
    description = text;
}

void Activity::setStartTime(QTime &time) {
    startTime = time;
}

void Activity::setEndTime(QTime &time) {
    endTime = time;
}

void Activity::setDate(QDate &d) {
    date = d;
}

void Activity::modifyActivity(std::string &text, QTime &s_time, QTime &e_time, QDate &d) {
    setDescription(text);
    setStartTime(s_time);
    setEndTime(e_time);
    setDate(d);
    if (!isValidInput()) {
        throw std::invalid_argument("Invalid input");
    }
}