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

    std::string setCorrectDescriptionFormat (const std::string &description) const;

    std::string setCorrectHourFormat (const QTime &startTime, const QTime &endTime) const;

    std::map<QDate, std::vector<Activity>> getActivities() const;

    int getMaxActivitiesPerDay() const;

    void setMaxActivitiesPerDay(int max);

    int getSearchNumber(std::string &search) const;

private slots:
    void addActivity (const Activity& newActivity);

    std::vector<Activity*> getSearchActivities(std::string &search);

    void onNewActivityButtonClicked();

    void refreshRegister ();

    void onAddButtonClicked ();

    void onSearchButtonClicked ();

    void onCancelButtonClicked ();

    void onDeleteButtonClicked (int index);

    void onDeleteButtonClicked (int index, std::string search);

    void onModifyButtonClicked (int index);

    void onModifyButtonClicked (int index, std::string search);

    void onDescriptionChanged (const QString &text);

    void onStartTimeChanged (QTime value);

    void onEndTimeChanged (QTime value);

    void onDateChanged (QDate value);

    void onSearchBarChanged (const QString &text);

    void onViewButtonClicked (QDate date);

private:
    std::map<QDate, std::vector<Activity>> activities;
    int maxActivitiesPerDay = 10;
    std::string tmp_description;
    QTime tmp_startTime;
    QTime tmp_endTime;
    QDate tmp_date;
    std::string tmp_search;
    QDialog *NewActivityDialog = nullptr;
    QDateEdit *registerDate;
    QScrollArea *scrollRegisterArea;
    QLineEdit *searchBar;
    QLabel *programmerLabel;
    QLabel *activityLabel= nullptr;
    QLabel *timeLabel= nullptr;
    QPushButton *addActivityButton;
    QPushButton *searchButton;
    QPushButton *deleteButton = nullptr;
    QPushButton *modifyButton = nullptr;
};


#endif //ACTIVITYREGISTERPROJECT_ACTIVITYREGISTER_H
