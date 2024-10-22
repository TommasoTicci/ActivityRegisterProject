//
// Created by Tommaso Ticci on 06/10/2024.
//

#include "gtest/gtest.h"

#include "../ActivityRegister.h"

class ActivitySuite : public ::testing::Test {

protected:
    virtual void SetUp() {
        std::string description = "TestDescription";
        startTime = QTime(10, 0);
        endTime = QTime(20, 0);
        QDate date(2023, 5, 12);
        activity.setDescription(description);
        activity.setStartTime(startTime);
        activity.setEndTime(endTime);
        activity.setDate(date);
    }

    Activity activity;
    QTime startTime;
    QTime endTime;
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

TEST_F(ActivitySuite, TestValidInput) {
    //Test start time before end time (TRUE)
    ASSERT_TRUE(activity.isValidInput());
    //Test start time = end time (TRUE)
    activity.setEndTime(startTime);
    ASSERT_TRUE(activity.isValidInput());
    //Test start time after end time (FALSE)
    activity.setStartTime(endTime);
    ASSERT_FALSE(activity.isValidInput());
}