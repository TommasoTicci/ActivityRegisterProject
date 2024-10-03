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