//
// Created by Tommaso Ticci on 12/10/2024.
//

#include "gtest/gtest.h"
#include <QApplication>

#include "../ActivityRegister.h"

TEST(ActivityRegister, TestConstructor) {
    int argc = 0;
    char *argv[] = {nullptr};
    QApplication app(argc, argv);

    ActivityRegister a;
    ASSERT_EQ(0, a.getActivities().size());
    ASSERT_EQ(10, a.getMaxActivitiesPerDay());
    std::map<QDate, std::vector<Activity>> activities = a.getActivities();
    ASSERT_EQ(0, activities.size());
}