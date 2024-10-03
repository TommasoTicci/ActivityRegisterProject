//
// Created by Tommaso Ticci on 03/10/2024.
//

#include "ActivityRegister.h"
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QDateEdit>
#include <QMessageBox>
#include <QLayoutItem>


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
    delete Font;
    activityLabel->show();
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
            std::string s = "Description: "+it->getDescription()+"     Inizio: "+std::to_string(it->getStartTime().hour())+":"+std::to_string(it->getStartTime().minute()) +" Fine: "+std::to_string(it->getEndTime().hour())+":"+std::to_string(it->getEndTime().minute());
            activityLabel = new QLabel(QString::fromStdString(s), scrollRegisterArea);
            activityLabel->setGeometry(10, 10 + 40 * (i-1), 350, 20);
            auto * Font = new QFont(activityLabel->font());
            Font->setPointSize(10);
            activityLabel->setFont(*Font);
            delete Font;
            activityLabel->show();
        }
    }
    if (i==0)
    {
        activityLabel = new QLabel("No activities", scrollRegisterArea);
        activityLabel->setGeometry(10, 10, 400, 20);
        auto * Font = new QFont(activityLabel->font());
        Font->setPointSize(10);
        activityLabel->setFont(*Font);
        delete Font;
        activityLabel->show();
    }
}

void ActivityRegister::onAddButtonClicked() {
    if (isValidInput()) {
        if (canAcceptActivity(tmp_date)) {
            addActivity();
            auto *MsgBox = new QMessageBox(this);
            MsgBox->setWindowTitle("Success");
            MsgBox->setAttribute(Qt::WA_DeleteOnClose, true);
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

void ActivityRegister::onCancelButtonClicked () {
    delete NewActivityDialog;
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

bool ActivityRegister::isValidInput () {
    bool isValid = true;
    if (tmp_description.empty()) {
        isValid = false;
    }
    if (tmp_startTime > tmp_endTime) {
        isValid = false;
    }
    return isValid;
}