//
// Created by Tommaso Ticci on 06/10/2024.
//

#include "gtest/gtest.h"

#include "../ActivityRegister.h"

class ActivitySuite : public ::testing::Test {

protected:
    virtual void SetUp() {
        std::string description = "TestDescription";
        QTime startTime(10, 0);
        QTime endTime(20, 0);
        QDate date(2023, 5, 12);
        activity.setDescription(description);
        activity.setStartTime(startTime);
        activity.setEndTime(endTime);
        activity.setDate(date);
    }

    Activity activity;
};

TEST_F(ActivitySuite, TestGetDescription) {
ASSERT_EQ("TestDescription", activity.getDescription());
}

TEST_F(ActivitySuite, TestGetStartTime) {
ASSERT_EQ(QTime(10, 0), activity.getStartTime());
}

TEST_F(ActivitySuite, TestGetEndTime) {
ASSERT_EQ(QTime(20, 0), activity.getEndTime());
}

TEST_F(ActivitySuite, TestGetDate) {
ASSERT_EQ(QDate(2023, 5, 12), activity.getDate());
}