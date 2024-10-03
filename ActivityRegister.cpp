//
// Created by Tommaso Ticci on 03/10/2024.
//

#include "ActivityRegister.h"
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QDateEdit>
#include <QLayoutItem>

ActivityRegister::ActivityRegister ()
{
    setFixedSize(800, 600);
    show();
    setWindowTitle("Activity Register");
    registerDate = new QDateEdit(QDate::currentDate(),this);
    registerDate->setGeometry(320, 10, 160, 30);
    registerDate->show();
    auto *addActivityButton = new QPushButton("Add activity", this);
    addActivityButton->setGeometry(350, 50, 100, 30);
    addActivityButton->show();
    auto *programmerLabel = new QLabel("by Tommaso Ticci", this);
    programmerLabel->setGeometry(10, 570, 200, 30);
    programmerLabel->show();
    scrollRegisterArea = new QScrollArea(this);
    scrollRegisterArea->setGeometry(100,90 ,600,400);
    scrollRegisterArea->show();
    QObject::connect(addActivityButton, &QPushButton::clicked, this, &ActivityRegister::onNewActivityButtonClicked);
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
}