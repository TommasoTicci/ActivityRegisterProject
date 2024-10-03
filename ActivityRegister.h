//
// Created by Tommaso Ticci on 03/10/2024.
//

#ifndef ACTIVITYREGISTERPROJECT_ACTIVITYREGISTER_H
#define ACTIVITYREGISTERPROJECT_ACTIVITYREGISTER_H

#include "Activity.h"
#include <vector>
#include <QWidget>
#include <QPushButton>
#include <QTime>
#include <QDateEdit>
#include <QScrollArea>
#include <QLabel>

class ActivityRegister : public QWidget {
public:
    ActivityRegister ();

    bool canAcceptActivity (const QDate &date);

    bool isValidInput ();

    int getActivitiesPerDay (const QDate &date);

    void addActivity ();

private slots:
    void onNewActivityButtonClicked();

    void refreshRegister ();

    void onAddButtonClicked ();

    void onCancelButtonClicked ();

    void onDescriptionChanged (const QString &text);

    void onStartTimeChanged (QTime value);

    void onEndTimeChanged (QTime value);

    void onDateChanged (QDate value);

private:
    std::vector<Activity> activities;
    int maxActivitiesPerDay = 10;
    std::string tmp_description;
    QTime tmp_startTime;
    QTime tmp_endTime;
    QDate tmp_date;
    QDialog *NewActivityDialog = nullptr;
    QDateEdit *registerDate;
    QScrollArea *scrollRegisterArea;
    QLabel *programmerLabel;
    QLabel *activityLabel;
    QPushButton *addActivityButton;
};


#endif //ACTIVITYREGISTERPROJECT_ACTIVITYREGISTER_H
