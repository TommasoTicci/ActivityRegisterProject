//
// Created by Tommaso Ticci on 12/10/2024.
//

#include "gtest/gtest.h"
#include <QApplication>

#include "../ActivityRegister.h"

class ActivityRegisterSuite : public ::testing::Test {

protected:
    void SetUp() override {
        char *argv[] = {nullptr};
        app = new QApplication(argc, argv);
        activityRegister = new ActivityRegister();
    }

    std::string description = "TestDescription";
    QTime startTime = QTime(10, 0);
    QTime endTime = QTime(20, 0);
    QDate date = QDate(2023, 5, 12);
    ActivityRegister* activityRegister;
    int argc = 0;
    QApplication* app;
};

TEST_F(ActivityRegisterSuite, TestValidInput) {
    //Test start time before end time (TRUE)
    ASSERT_TRUE(activityRegister->isValidInputManual(description, startTime, endTime, date));
    //Test start time = end time (TRUE)
    ASSERT_TRUE(activityRegister->isValidInputManual(description, startTime, startTime, date));
    //Test start time after end time (FALSE)
    ASSERT_FALSE(activityRegister->isValidInputManual(description, endTime, startTime, date));
}

TEST_F(ActivityRegisterSuite, TestAddActivityOnSameDate){
    //Test add activity
    activityRegister->addActivityManual(description, startTime, endTime, date);
    ASSERT_EQ(1, activityRegister->getActivitiesPerDay(date));
    //Test add activity with same description
    activityRegister->addActivityManual(description, startTime, endTime, date);
    activityRegister->addActivityManual(description, startTime, endTime, date);
    ASSERT_EQ(3, activityRegister->getActivitiesPerDay(date));
}

TEST_F(ActivityRegisterSuite, TestAddActivityOnDifferentDate){
    //Test add activity
    activityRegister->addActivityManual(description, startTime, endTime, date);
    //Test add activity with different date
    activityRegister->addActivityManual(description, startTime, endTime, QDate(2023, 5, 13));
    ASSERT_EQ(1, activityRegister->getActivitiesPerDay(date));
    ASSERT_EQ(1, activityRegister->getActivitiesPerDay(QDate(2023, 5, 13)));
}

TEST_F(ActivityRegisterSuite, TestMaxActivitiesCap){
    for (int i = 0; i < 9; ++i) {
        activityRegister->addActivityManual(description, startTime, endTime, date);
    }
    //Test add activity with max activities per day - 1
    ASSERT_TRUE(activityRegister->canAcceptActivity(date));
    //Test add activity with max activities per day
    activityRegister->addActivityManual(description, startTime, endTime, date);
    ASSERT_FALSE(activityRegister->canAcceptActivity(date));
}

TEST_F(ActivityRegisterSuite, TestDescriptionFormat){
    //Test description with less than 32 characters
    ASSERT_EQ("TestDescription", activityRegister->setCorrectDescriptionFormat(description));
    //Test description with more than 32 characters
    ASSERT_EQ("TestDescriptionOOOOOOOOOOOOOOOOL", activityRegister->setCorrectDescriptionFormat(description+"OOOOOOOOOOOOOOOOLMORETHAN32CHARACTERS"));
}

TEST_F(ActivityRegisterSuite, TestHourFormat){
    //Test hour with 1 digit (both)
    ASSERT_EQ("Start: 10:00   End: 12:00", activityRegister->setCorrectHourFormat(QTime(10, 0),(QTime(12, 0))));
    //Test hour with 2 digits (both)
    ASSERT_EQ("Start: 10:10   End: 12:10", activityRegister->setCorrectHourFormat(QTime(10, 10),(QTime(12, 10))));
    //Test hour with 1 digit (start)
    ASSERT_EQ("Start: 10:00   End: 12:10", activityRegister->setCorrectHourFormat(QTime(10, 0),(QTime(12, 10))));
    //Test hour with 1 digit (end)
    ASSERT_EQ("Start: 10:10   End: 12:00", activityRegister->setCorrectHourFormat(QTime(10, 10),(QTime(12, 0))));
}


