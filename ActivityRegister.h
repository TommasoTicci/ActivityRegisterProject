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
#include <QVBoxLayout>

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

    std::vector<Activity*> getSearchActivities (std::string &search);

    void onNewActivityButtonClicked ();

    void refreshRegister ();

    void onAddButtonClicked ();

    void onSearchButtonClicked ();

    void onCancelButtonClicked ();

    void onDeleteButtonClicked (int index);

    void onDeleteButtonClicked (int index, std::string search);

    void onModifyButtonClicked (int index);

    void onModifyButtonClicked (int index, std::string search);

    void modifyButtonManager (std::vector<Activity>& dayActivities, int index);

    void modifyButtonManager (std::vector<Activity*>& searchedActivity, int index, const std::string& search);

    void onDescriptionChanged (const QString &text);

    void onStartTimeChanged (QTime value);

    void onEndTimeChanged (QTime value);

    void onDateChanged (QDate value);

    void onSearchBarChanged (const QString &text);

    void onViewButtonClicked (QDate date);

private:

    // ---- METODI UI ----

    void uiInitializeMain ();

    void uiSetUpRegisterConnections ();

    void uiInitializeMaxActivitiesDialog();

    void uiInitializeDialogWindow(const std::string& title, const std::string& description, QTime startTime, QTime endTime, QDate date, bool modify);

    void uiSetUpSearchWindow(const std::vector<Activity*>& searchedActivity);

    void uiCreateMsgBox (const std::string& title, const std::string& message);

    void uiCreateNewActivityDialog (std::string title, int sizeX, int sizeY);

    void uiCreateRegisterArea (int sizeX, int sizeY);

    void uiCreateButton (const std::string &text, int x, int y, int sizeX, int sizeY, QWidget *parent, void (ActivityRegister::*slot)()) const;

    void uiCreateTimeEdit (QTime time, int x, int y, int sizeX, int sizeY, QWidget *parent, void (ActivityRegister::*slot)(QTime)) const;

    void uiCreateDataEdit (QDate date, int x, int y, int sizeX, int sizeY, QWidget *parent, void (ActivityRegister::*slot)(QDate)) const;

    void uiCreateLineEdit (const std::string &text, int x, int y, int sizeX, int sizeY, QWidget *parent, void (ActivityRegister::*slot)(const QString &)) const;

    void uiCreateLabel (const std::string &text, int x, int y, int sizeX, int sizeY, QWidget *parent) const;

    void uiShowActivity (int index, const Activity &activity);

    void uiShowSearchResults(QVBoxLayout *layout, const std::vector<Activity*> &activities);

    QHBoxLayout* uiCreateSearchResultsLayout(Activity* activity, int index);

    QPushButton* uiCreateModifyButton();

private:
    std::map<QDate, std::vector<Activity>> activities;
    int maxActivitiesPerDay = 10;
    std::string tmp_description;
    QTime tmp_startTime;
    QTime tmp_endTime;
    QDate tmp_date;
    std::string tmp_search;
    QDialog *NewActivityDialog = nullptr;
    QDateEdit *registerDate = nullptr;
    QScrollArea *scrollRegisterArea = nullptr;
    QLineEdit *searchBar = nullptr;
};


#endif //ACTIVITYREGISTERPROJECT_ACTIVITYREGISTER_H
