//
// Created by Tommaso Ticci on 03/10/2024.
//

#include "ActivityRegister.h"
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QDateEdit>
#include <QMessageBox>

ActivityRegister::ActivityRegister ()
{
    setFixedSize(800, 600);
    setWindowTitle("Activity Register");
    registerDate = new QDateEdit(QDate::currentDate(),this);
    registerDate->setGeometry(320, 10, 160, 30);
    registerDate->show();
    scrollRegisterArea = new QScrollArea(this);
    scrollRegisterArea->setGeometry(100,90 ,600,400);
    scrollRegisterArea->show();
    addActivityButton = new QPushButton("Add activity", this);
    addActivityButton->setGeometry(350, 50, 100, 30);
    addActivityButton->show();
    programmerLabel = new QLabel("by Tommaso Ticci", this);
    programmerLabel->setGeometry(10, 570, 200, 30);
    programmerLabel->show();
    activityLabel = new QLabel("No activities", scrollRegisterArea);
    activityLabel->setGeometry(10, 10, 400, 20);
    auto * Font = new QFont(activityLabel->font());
    Font->setPointSize(10);
    activityLabel->setFont(*Font);
    activityLabel->show();
    delete Font;
    QObject::connect(addActivityButton, &QPushButton::clicked, this, &ActivityRegister::onNewActivityButtonClicked);
    QObject::connect(registerDate, &QDateEdit::dateChanged, this, &ActivityRegister::refreshRegister);
    show();
}

void ActivityRegister::onNewActivityButtonClicked() {
    tmp_description = "Activity";
    tmp_startTime = QTime::currentTime();
    tmp_endTime = QTime::currentTime();
    tmp_date = registerDate->date();
    NewActivityDialog = new QDialog(this);
    NewActivityDialog->setAttribute ( Qt::WA_DeleteOnClose, true );
    NewActivityDialog->setWindowTitle("New Activity");
    NewActivityDialog->setWindowModality(Qt::WindowModality::ApplicationModal);
    NewActivityDialog->setFixedSize(400, 300);
    NewActivityDialog->show();
    auto *descriptionLabel = new QLabel("Description", NewActivityDialog);
    descriptionLabel->setGeometry(10, 10, 200, 30);
    descriptionLabel->show();
    auto *descriptionArea = new QLineEdit("Activity", NewActivityDialog);
    descriptionArea->setGeometry(10, 50, 200, 30);
    descriptionArea->show();
    auto *timeLabel = new QLabel("Start Time                      End Time", NewActivityDialog);
    timeLabel->setGeometry(10, 90, 200, 30);
    timeLabel->show();
    auto *startTimeArea = new QTimeEdit(QTime::currentTime(),NewActivityDialog);
    startTimeArea->setGeometry(10, 130, 70, 30);
    startTimeArea->show();
    auto *endTimeArea = new QTimeEdit(QTime::currentTime(),NewActivityDialog);
    endTimeArea->setGeometry(130, 130, 70, 30);
    endTimeArea->show();
    auto *addButton = new QPushButton("Add", NewActivityDialog);
    addButton->show();
    addButton->setGeometry(290, 10, 100, 30);
    auto *cancelButton = new QPushButton("Cancel", NewActivityDialog);
    cancelButton->show();
    cancelButton->setGeometry(290, 50, 100, 30);
    auto *dateLabel = new QLabel("Date", NewActivityDialog);
    dateLabel->setGeometry(10, 170, 100, 30);
    dateLabel->show();
    auto *dateArea = new QDateEdit(registerDate->date(), NewActivityDialog);
    dateArea->setGeometry(10, 210, 170, 30);
    dateArea->show();
    connect(addButton, &QPushButton::clicked, this, &ActivityRegister::onAddButtonClicked);
    connect(cancelButton, &QPushButton::clicked, this, &ActivityRegister::onCancelButtonClicked);
    connect(descriptionArea, &QLineEdit::textChanged, this, &ActivityRegister::onDescriptionChanged);
    connect(startTimeArea, &QTimeEdit::timeChanged, this, &ActivityRegister::onStartTimeChanged);
    connect(endTimeArea, &QTimeEdit::timeChanged, this, &ActivityRegister::onEndTimeChanged);
    connect(dateArea, &QDateEdit::dateChanged, this, &ActivityRegister::onDateChanged);
}

void ActivityRegister::refreshRegister() {
    delete scrollRegisterArea;
    scrollRegisterArea = new QScrollArea(this);
    scrollRegisterArea->setGeometry(100,90 ,600,400);
    scrollRegisterArea->show();
    int i = 0;
    for (auto it = activities.begin(); it != activities.end(); ++it)
    {
        if (it->getDate() == registerDate->date())
        {
            i++;
            activityLabel = new QLabel(QString::fromStdString(setCorrectDescriptionFormat(it->getDescription())), scrollRegisterArea);
            activityLabel->setGeometry(10, 10 + 40 * (i-1), 350, 20);
            timeLabel = new QLabel(QString::fromStdString(setCorrectHourFormat(it->getStartTime(),it->getEndTime())), scrollRegisterArea);
            timeLabel->setGeometry(350, 10 + 40 * (i-1), 200, 20);
            auto * Font = new QFont(activityLabel->font());
            Font->setPointSize(10);
            activityLabel->setFont(*Font);
            timeLabel->setFont(*Font);
            deleteButton = new QPushButton("X", scrollRegisterArea);
            deleteButton->setGeometry(560, 10 + 40 * (i-1), 20, 20);
            connect (deleteButton, &QPushButton::clicked, this, [this, i]{onDeleteButtonClicked(i-1);});
            activityLabel->show();
            timeLabel->show();
            deleteButton->show();
            delete Font;
        }
    }
    if (i==0)
    {
        activityLabel = new QLabel("No activities", scrollRegisterArea);
        activityLabel->setGeometry(10, 10, 400, 20);
        auto * Font = new QFont(activityLabel->font());
        Font->setPointSize(10);
        activityLabel->setFont(*Font);
        activityLabel->show();
        delete Font;
    }
}

std::string ActivityRegister::setCorrectDescriptionFormat (const std::string &description) {
    std::string new_description;
    if (description.length()>32) {
        new_description = description.substr(0, 32);
    }
    else {
        new_description = description;
    }
    return new_description;
}

std::string ActivityRegister::setCorrectHourFormat (const QTime &startTime, const QTime &endTime) {
    std::string s;
    if(startTime.minute()<10)
    {
        if(endTime.minute()<10)
        {
            s = "Start: "+std::to_string(startTime.hour())+":0"+std::to_string(startTime.minute()) +"   End: "+std::to_string(endTime.hour())+":0"+std::to_string(endTime.minute());
        }
        else
        {
            s = "Start: "+std::to_string(startTime.hour())+":0"+std::to_string(startTime.minute()) +"   End: "+std::to_string(endTime.hour())+":"+std::to_string(endTime.minute());
        }
    }
    else
    {
        if(endTime.minute()<10)
        {
            s = "Start: "+std::to_string(startTime.hour())+":"+std::to_string(startTime.minute()) +"   End: "+std::to_string(endTime.hour())+":0"+std::to_string(endTime.minute());
        }
        else
        {
            s = "Start: "+std::to_string(startTime.hour())+":"+std::to_string(startTime.minute()) +"   End: "+std::to_string(endTime.hour())+":"+std::to_string(endTime.minute());
        }
    }
    return s;
}

void ActivityRegister::onAddButtonClicked() {
    if (isValidInput()) {
        if (canAcceptActivity(tmp_date)) {
            addActivity();
            auto *MsgBox = new QMessageBox(this);
            MsgBox->setWindowTitle("Success");
            MsgBox->setAttribute(Qt::WA_DeleteOnClose, true);
            MsgBox->setText("Added");
            MsgBox->show();
            NewActivityDialog->close();
        } else {
            auto *MsgBox = new QMessageBox(this);
            MsgBox->setWindowTitle("Error");
            MsgBox->setAttribute(Qt::WA_DeleteOnClose, true);
            MsgBox->setText("Too many activities");
            MsgBox->show();
        }
    }
    else
    {
        auto *MsgBox = new QMessageBox(this);
        MsgBox->setWindowTitle("Error");
        MsgBox->setAttribute ( Qt::WA_DeleteOnClose, true );
        MsgBox->setText("Invalid input");
        MsgBox->show();
    }
}

void ActivityRegister::addActivity() {
    Activity newActivity(tmp_description, tmp_startTime, tmp_endTime, tmp_date);
    activities.push_back(newActivity);
    ActivityRegister::refreshRegister();
}

void ActivityRegister::addActivityManual(const std::string &description, const QTime &startTime, const QTime &endTime,const QDate &date) {
    std::string new_description = description;
    QTime new_startTime = startTime;
    QTime new_endTime = endTime;
    QDate new_date = date;
    Activity newActivity(new_description, new_startTime, new_endTime, new_date);
    activities.push_back(newActivity);
    ActivityRegister::refreshRegister();
}

void ActivityRegister::onCancelButtonClicked () {
    delete NewActivityDialog;
}

void ActivityRegister::onDeleteButtonClicked(int index) {
    for (auto it = activities.begin(); it != activities.end() && index != -1; ++it)
    {
        if (it->getDate() == registerDate->date())
        {
            index--;
            if (index == -1)
            {
                activities.erase(it);
                ActivityRegister::refreshRegister();
            }
        }
    }
}

void ActivityRegister::onDescriptionChanged (const QString &text) {
    tmp_description = text.toStdString();
}

void ActivityRegister::onStartTimeChanged (QTime value) {
    tmp_startTime = value;
}

void ActivityRegister::onEndTimeChanged (QTime value) {
    tmp_endTime = value;
}

void ActivityRegister::onDateChanged (QDate value) {
    tmp_date = value;
}

int ActivityRegister::getActivitiesPerDay(const QDate &date) {
    int i = 0;
    for (auto it = activities.begin(); it != activities.end(); ++it)
    {
        if (it->getDate() == date)
        {
            i++;
        }
    }
    return i;
}

bool ActivityRegister::canAcceptActivity(const QDate &date)
{
    return ActivityRegister::getActivitiesPerDay(date) < maxActivitiesPerDay;
}

bool ActivityRegister::isValidInput() {
    bool isValid = true;
    if (tmp_description.empty()) {
        isValid = false;
    }
    if (tmp_startTime > tmp_endTime) {
        isValid = false;
    }
    return isValid;
}

bool ActivityRegister::isValidInputManual (const std::string &description, const QTime &startTime, const QTime &endTime, const QDate &date) {
    bool isValid = true;
    if (description.empty()) {
        isValid = false;
    }
    if (startTime > endTime) {
        isValid = false;
    }
    return isValid;
}

std::vector<Activity>* ActivityRegister::getActivities() {
    return &activities;
}

int ActivityRegister::getMaxActivitiesPerDay() {
    return maxActivitiesPerDay;
}