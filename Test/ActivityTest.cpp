//
// Created by Tommaso Ticci on 06/10/2024.
//

#include "gtest/gtest.h"

#include "../ActivityRegister.h"

TEST(Activity, TestConstructor) {
    std::string description = "TestDescription";
    QTime startTime = QTime(10, 0);
    QTime endTime = QTime(20, 0);
    QDate date = QDate(2023, 5, 12);
    Activity activity(description, startTime, endTime, date);
    ASSERT_EQ("TestDescription", activity.getDescription());
    ASSERT_EQ(QTime(10, 0), activity.getStartTime());
    ASSERT_EQ(QTime(20, 0), activity.getEndTime());
    ASSERT_EQ(QDate(2023, 5, 12), activity.getDate());
    ASSERT_THROW(Activity("", startTime, endTime, date), std::invalid_argument);
    ASSERT_THROW(Activity(description, endTime, startTime, date), std::invalid_argument);
}