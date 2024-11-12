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

TEST_F(ActivitySuite, TestModifyActivity) {
    std::string description = "NewDescription";
    std::string emptyDescription = "";
    QTime newStartTime = QTime(12, 0);
    QTime newEndTime = QTime(18, 0);
    QDate newDate(2023, 5, 13);
    activity.modifyActivity(description, newStartTime, newEndTime, newDate);
    ASSERT_EQ("NewDescription", activity.getDescription());
    ASSERT_EQ(QTime(12, 0), activity.getStartTime());
    ASSERT_EQ(QTime(18, 0), activity.getEndTime());
    ASSERT_EQ(QDate(2023, 5, 13), activity.getDate());
    ASSERT_THROW(activity.modifyActivity(description, newEndTime, newStartTime, newDate), std::invalid_argument);
    ASSERT_THROW(activity.modifyActivity(emptyDescription, newStartTime, newEndTime, newDate), std::invalid_argument);
}

TEST_F(ActivitySuite, TestIsEqual) {
    Activity activity2("TestDescription", startTime, endTime, QDate(2023, 5, 12));
    ASSERT_TRUE(activity.isEqual(activity2));
    std::string newDescription = "NewDescription";
    activity2.setDescription(newDescription);
    ASSERT_FALSE(activity.isEqual(activity2));
}