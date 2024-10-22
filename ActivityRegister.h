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

    bool canAcceptActivity (const QDate &date) const;

    int getActivitiesPerDay (const QDate &date) const;

    void addActivity (const std::string &description, const QTime &startTime, const QTime &endTime, const QDate &date);

    std::string setCorrectDescriptionFormat (const std::string &description);

    std::string setCorrectHourFormat (const QTime &startTime, const QTime &endTime);

    std::vector<Activity> getActivities();

    int getMaxActivitiesPerDay();

private slots:
    void addActivity (const Activity& newActivity);

    void onNewActivityButtonClicked();

    void refreshRegister ();

    void onAddButtonClicked ();

    void onCancelButtonClicked ();

    void onDeleteButtonClicked (int index);

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
    QLabel *activityLabel= nullptr;
    QLabel *timeLabel= nullptr;
    QPushButton *addActivityButton;
    QPushButton *deleteButton = nullptr;
};


#endif //ACTIVITYREGISTERPROJECT_ACTIVITYREGISTER_H
