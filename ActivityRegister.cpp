//
// Created by Tommaso Ticci on 03/10/2024.
//

#include "ActivityRegister.h"
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QDateEdit>
#include <QMessageBox>
#include <QVBoxLayout>

ActivityRegister::ActivityRegister ()
{
    uiInitializeMain();
    uiSetUpRegisterConnections();
    uiInitializeMaxActivitiesDialog();
}

void ActivityRegister::onNewActivityButtonClicked() {
    tmp_description = "Activity";
    tmp_startTime = QTime::currentTime();
    tmp_endTime = QTime::currentTime();
    tmp_date = registerDate->date();
    uiInitializeDialogWindow("New Activity", tmp_description, tmp_startTime, tmp_endTime, tmp_date, false);
}

void ActivityRegister::refreshRegister() {
    delete scrollRegisterArea;
    uiCreateRegisterArea(600, 400);
    int i = 0;
    auto it = activities.find(registerDate->date());
    if (it != activities.end()) {
        const std::vector<Activity>& dayActivities = it->second;
        for (const Activity& activity : dayActivities) {
            i++;
            uiShowActivity(i, activity);
        }
    }
    if (i == 0) {
        uiCreateLabel("No activities", 10, 10, 400, 20, scrollRegisterArea);
    }
}

std::string ActivityRegister::setCorrectDescriptionFormat (const std::string &description) const {
    std::string new_description;
    if (description.length()>32) {
        new_description = description.substr(0, 32);
    }
    else {
        new_description = description;
    }
    return new_description;
}

std::string ActivityRegister::setCorrectHourFormat (const QTime &startTime, const QTime &endTime) const {
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
    try {
        Activity newActivity(tmp_description, tmp_startTime, tmp_endTime, tmp_date);

        if (canAcceptActivity(tmp_date)) {
            addActivity(newActivity);
            uiCreateMsgBox("Success","Added");
            NewActivityDialog->close();
        } else {
            uiCreateMsgBox("Error","Too many activities");
        }

    } catch (const std::invalid_argument &e) {
        uiCreateMsgBox("Error",e.what());
    }
}

void ActivityRegister::addActivity(const Activity& newActivity) {
    activities[newActivity.getDate()].push_back(newActivity);
    refreshRegister();
}

void ActivityRegister::addActivity(const std::string &description, const QTime &startTime, const QTime &endTime,const QDate &date) {
    const std::string& new_description = description;
    QTime new_startTime = startTime;
    QTime new_endTime = endTime;
    QDate new_date = date;
    Activity newActivity(new_description, new_startTime, new_endTime, new_date);
    activities[newActivity.getDate()].push_back(newActivity);
    refreshRegister();
}

void ActivityRegister::onSearchButtonClicked() {
    std::vector<Activity*> searchedActivity = getSearchActivities(tmp_search);
    uiSetUpSearchWindow(searchedActivity);
}

std::vector<Activity*> ActivityRegister::getSearchActivities(std::string &search) {
    std::vector<Activity*> searchedActivities;
    for (auto &it : activities) {
        for (auto &activity : it.second) {
            if (activity.getDescription() == search) {
                searchedActivities.push_back(&activity);
            }
        }
    }
    return searchedActivities;
}

void ActivityRegister::onCancelButtonClicked () {
    delete NewActivityDialog;
}

void ActivityRegister::onDeleteButtonClicked(int index) {
    auto it = activities.find(registerDate->date());
    if (it != activities.end()) {
        std::vector<Activity> &dayActivities = it->second;
        if (index >= 0 && index < dayActivities.size()) {
            dayActivities.erase(dayActivities.begin() + index);
            if (dayActivities.empty()) {
                activities.erase(it);
            }
            refreshRegister();
        }
    }
}

void ActivityRegister::onDeleteButtonClicked(int index, std::string search) {
    NewActivityDialog->close();
    auto dateIt = activities.begin();
    int count = 0;
    while (dateIt != activities.end()) {
        auto& activityList = dateIt->second;
        for (auto it = activityList.begin(); it != activityList.end(); ++it) {
            if (it->getDescription() == search) {
                if (count == index) {
                    activityList.erase(it);
                    if (activityList.empty()) {
                        dateIt = activities.erase(dateIt);
                    } else {
                        ++dateIt;
                    }
                    refreshRegister();
                    return;
                }
                ++count;
            }
        }
        ++dateIt;
    }
    refreshRegister();
}

void ActivityRegister::onViewButtonClicked(QDate date) {
    registerDate->setDate(date);
    NewActivityDialog->close();
    refreshRegister();
}

void ActivityRegister::onModifyButtonClicked(int index) {
    auto it = activities.find(registerDate->date());
    if (it != activities.end()) {
        std::vector<Activity> &dayActivities = it->second;
        tmp_description = dayActivities[index].getDescription();
        tmp_startTime = dayActivities[index].getStartTime();
        tmp_endTime = dayActivities[index].getEndTime();
        tmp_date = registerDate->date();

        uiInitializeDialogWindow("Modify Activity", tmp_description, tmp_startTime, tmp_endTime, tmp_date, true);

        modifyButtonManager(dayActivities, index);
    }
}

void ActivityRegister::onModifyButtonClicked(int index, std::string search) {
    NewActivityDialog->close();
    std::vector<Activity*> searchedActivity = getSearchActivities(search);
    tmp_description = searchedActivity[index]->getDescription();
    tmp_startTime = searchedActivity[index]->getStartTime();
    tmp_endTime = searchedActivity[index]->getEndTime();
    tmp_date = searchedActivity[index]->getDate();

    uiInitializeDialogWindow("Modify Activity", tmp_description, tmp_startTime, tmp_endTime, tmp_date, true);

    modifyButtonManager(searchedActivity, index, search);
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

void ActivityRegister::onSearchBarChanged(const QString &text) {
    tmp_search = text.toStdString();
}

int ActivityRegister::getActivitiesPerDay(const QDate &date) const {
    auto it = activities.find(date);
    if (it != activities.end()) {
        return (int) it->second.size();
    }
    return 0;
}

bool ActivityRegister::canAcceptActivity(const QDate &date) const
{
    return ActivityRegister::getActivitiesPerDay(date) < maxActivitiesPerDay;
}

std::map<QDate, std::vector<Activity>> ActivityRegister::getActivities() const {
    return activities;
}

int ActivityRegister::getMaxActivitiesPerDay() const {
    return maxActivitiesPerDay;
}

void ActivityRegister::setMaxActivitiesPerDay(int maxActivities) {
    if (maxActivities > 0 && maxActivities <= 10) {
        maxActivitiesPerDay = maxActivities;
    }
    else
        throw std::invalid_argument("Invalid input");
}

int ActivityRegister::getSearchNumber(std::string &search) const {
    int count = 0;
    for (auto &it : activities) {
        for (auto &activity : it.second) {
            if (activity.getDescription() == search) {
                count++;
            }
        }
    }
    return count;
}

void ActivityRegister::modifyButtonManager(std::vector<Activity>& dayActivities, int index) {
    auto* modButton = uiCreateModifyButton();
    connect(modButton, &QPushButton::clicked, this, [this, &dayActivities, index]() {
        try {
            if (!canAcceptActivity(tmp_date) and dayActivities[index].getDate() != tmp_date) {
                throw std::invalid_argument("Too many activities");
            }
            dayActivities[index].modifyActivity(tmp_description, tmp_startTime, tmp_endTime, tmp_date);
            if (tmp_date != registerDate->date()) {
                activities[tmp_date].push_back(dayActivities[index]);
                activities[registerDate->date()].erase(activities[registerDate->date()].begin() + index);
            }
            refreshRegister();
            NewActivityDialog->close();
        } catch (const std::invalid_argument &e) {
            uiCreateMsgBox("Error",e.what());
        }
    });
}

void ActivityRegister::modifyButtonManager(std::vector<Activity*>& searchedActivity, int index, const std::string& search) {
    auto* modButton = uiCreateModifyButton();
    connect(modButton, &QPushButton::clicked, this, [this, searchedActivity, index]() {
        try {
            QDate oldDate = searchedActivity[index]->getDate();
            Activity *oldActivity = searchedActivity[index];
            if (!canAcceptActivity(tmp_date) and oldDate != tmp_date) {
                throw std::invalid_argument("Too many activities");
            }
            searchedActivity[index]->modifyActivity(tmp_description, tmp_startTime, tmp_endTime, tmp_date);
            if (oldDate != tmp_date) {
                auto dateIt = activities.find(oldDate);
                if (dateIt != activities.end()) {
                    auto& activityList = dateIt->second;
                    activities[tmp_date].push_back(*oldActivity);
                    activityList.erase(std::remove_if(activityList.begin(), activityList.end(),
                                                      [this, &oldActivity](const Activity& activity) {
                                                          return activity.isEqual(*oldActivity);
                                                      }),
                                       activityList.end());
                    if (activityList.empty()) {
                        activities.erase(dateIt);
                    }
                }
            }
            refreshRegister();
            NewActivityDialog->close();
        } catch (const std::invalid_argument &e) {
            uiCreateMsgBox("Error",e.what());
        }
    });
}



// ------------------------- METODI UI -------------------------



void ActivityRegister::uiInitializeMain() {
    setFixedSize(800, 600);
    setWindowTitle("Activity Register");

    uiCreateRegisterArea(600, 400);

    registerDate = new QDateEdit(QDate::currentDate(), this);
    registerDate->setGeometry(320, 10, 160, 30);
    registerDate->show();

    searchBar = new QLineEdit(this);
    searchBar->setGeometry(250, 500, 200, 30);
    searchBar->setPlaceholderText("Search");
    searchBar->show();

    tmp_search = "";

    uiCreateButton("Add activity", 350, 50, 100, 30, this, &ActivityRegister::onNewActivityButtonClicked);

    uiCreateButton("Search", 470, 500, 100, 30, this, &ActivityRegister::onSearchButtonClicked);

    uiCreateLabel("by Tommaso Ticci", 10, 570, 200, 30, this);

    uiCreateLabel("No activities", 10, 10, 400, 20, scrollRegisterArea);

    show();
}

void ActivityRegister::uiSetUpRegisterConnections() {
    QObject::connect(registerDate, &QDateEdit::dateChanged, this, &ActivityRegister::refreshRegister);
    QObject::connect(searchBar, &QLineEdit::textChanged, this, &ActivityRegister::onSearchBarChanged);
}

void ActivityRegister::uiInitializeMaxActivitiesDialog() {
    uiCreateNewActivityDialog("Set Max Activities", 200, 100);

    uiCreateLabel("Max Activities Per Day (<10)", 10, 10, 200, 30, NewActivityDialog);

    auto *maxActivitiesArea = new QLineEdit("10", NewActivityDialog);
    maxActivitiesArea->setGeometry(10, 50, 70, 30);
    maxActivitiesArea->show();

    auto *setButton = new QPushButton("Set", NewActivityDialog);
    setButton->setGeometry(90, 50, 70, 30);
    setButton->show();

    connect(setButton, &QPushButton::clicked, this, [this, maxActivitiesArea] {
        try {
            setMaxActivitiesPerDay(maxActivitiesArea->text().toInt());
            NewActivityDialog->close();
        } catch (const std::invalid_argument &e) {
            uiCreateMsgBox("Error", e.what());
        }
    });
}

void ActivityRegister::uiInitializeDialogWindow(const std::string& title, const std::string& description, QTime startTime, QTime endTime, QDate date, bool modify) {
    uiCreateNewActivityDialog(title, 400, 300);

    uiCreateLabel("Description", 10, 10, 200, 30, NewActivityDialog);

    uiCreateLineEdit(description, 10, 50, 200, 30, NewActivityDialog, &ActivityRegister::onDescriptionChanged);

    uiCreateLabel("Start Time                      End Time", 10, 90, 200, 30, NewActivityDialog);

    uiCreateTimeEdit(startTime, 10, 130, 70, 30, NewActivityDialog, &ActivityRegister::onStartTimeChanged);

    uiCreateTimeEdit(endTime, 130, 130, 70, 30, NewActivityDialog, &ActivityRegister::onEndTimeChanged);

    uiCreateLabel("Date", 10, 170, 100, 30, NewActivityDialog);

    uiCreateDataEdit(date, 10, 210, 170, 30, NewActivityDialog, &ActivityRegister::onDateChanged);

    if (!modify) {
        uiCreateButton("Add", 290, 10, 100, 30, NewActivityDialog, &ActivityRegister::onAddButtonClicked);
    }

    uiCreateButton("Cancel", 290, 50, 100, 30, NewActivityDialog, &ActivityRegister::onCancelButtonClicked);
}

void ActivityRegister::uiSetUpSearchWindow(const std::vector<Activity*>& searchedActivity) {
    uiCreateNewActivityDialog("Search Results", 600, 400);

    QScrollArea *scrollArea = new QScrollArea(NewActivityDialog);
    scrollArea->setWidgetResizable(true);
    QWidget *scrollWidget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(scrollWidget);

    uiShowSearchResults(layout, searchedActivity);

    scrollWidget->setLayout(layout);
    scrollArea->setWidget(scrollWidget);

    QVBoxLayout *dialogLayout = new QVBoxLayout(NewActivityDialog);
    dialogLayout->addWidget(scrollArea);

    NewActivityDialog->setLayout(dialogLayout);
    NewActivityDialog->exec();
}

void ActivityRegister::uiCreateMsgBox(const std::string& title, const std::string& message) {
    auto *MsgBox = new QMessageBox(this);
    MsgBox->setWindowTitle(title.c_str());
    MsgBox->setAttribute(Qt::WA_DeleteOnClose, true);
    MsgBox->setText(message.c_str());
    MsgBox->show();
}

void ActivityRegister::uiCreateNewActivityDialog(std::string title, int sizeX, int sizeY) {
    NewActivityDialog = new QDialog(this);
    NewActivityDialog->setAttribute(Qt::WA_DeleteOnClose, true);
    NewActivityDialog->setWindowTitle(title.c_str());
    NewActivityDialog->setWindowModality(Qt::WindowModality::ApplicationModal);
    NewActivityDialog->setFixedSize(sizeX, sizeY);
    NewActivityDialog->show();
}

void ActivityRegister::uiCreateRegisterArea(int sizeX, int sizeY) {
    scrollRegisterArea = new QScrollArea(this);
    scrollRegisterArea->setGeometry(100, 90, sizeX, sizeY);
    scrollRegisterArea->show();
}

void ActivityRegister::uiCreateButton(const std::string &text, int x, int y, int sizeX, int sizeY, QWidget *parent, void (ActivityRegister::*slot)()) const {
    auto *button = new QPushButton(QString::fromStdString(text), parent);
    button->setGeometry(x, y, sizeX, sizeY);
    button->show();
    QObject::connect(button, &QPushButton::clicked, this, slot);
}

void ActivityRegister::uiCreateTimeEdit(QTime time, int x, int y, int sizeX, int sizeY, QWidget *parent, void (ActivityRegister::*slot)(QTime)) const {
    auto *timeEdit = new QTimeEdit(time, parent);
    timeEdit->setGeometry(x, y, sizeX, sizeY);
    timeEdit->show();
    QObject::connect(timeEdit, &QTimeEdit::timeChanged, this, slot);
}

void ActivityRegister::uiCreateDataEdit(QDate date, int x, int y, int sizeX, int sizeY, QWidget *parent, void (ActivityRegister::*slot)(QDate)) const {
    auto *dateEdit = new QDateEdit(date, parent);
    dateEdit->setGeometry(x, y, sizeX, sizeY);
    dateEdit->show();
    QObject::connect(dateEdit, &QDateEdit::dateChanged, this, slot);
}

void ActivityRegister::uiCreateLineEdit(const std::string &text, int x, int y, int sizeX, int sizeY, QWidget *parent, void (ActivityRegister::*slot)(const QString &)) const {
    auto *lineEdit = new QLineEdit(QString::fromStdString(text), parent);
    lineEdit->setGeometry(x, y, sizeX, sizeY);
    lineEdit->show();
    QObject::connect(lineEdit, &QLineEdit::textChanged, this, slot);
}

void ActivityRegister::uiCreateLabel(const std::string &text, int x, int y, int sizeX, int sizeY, QWidget *parent) const {
    auto *label = new QLabel(text.c_str(), parent);
    label->setGeometry(x, y, sizeX, sizeY);
    label->show();
}

void ActivityRegister::uiShowActivity(int index, const Activity& activity) {
    uiCreateLabel(setCorrectDescriptionFormat(activity.getDescription()), 10, 10 + 40 * (index - 1), 350, 20, scrollRegisterArea);

    uiCreateLabel(setCorrectHourFormat(activity.getStartTime(), activity.getEndTime()), 350, 10 + 40 * (index - 1), 200, 20, scrollRegisterArea);

    auto* deleteButton = new QPushButton("X", scrollRegisterArea);
    deleteButton->setGeometry(560, 10 + 40 * (index - 1), 20, 20);
    connect(deleteButton, &QPushButton::clicked, this, [this, index] { onDeleteButtonClicked(index - 1); });

    auto* modifyButton = new QPushButton("M", scrollRegisterArea);
    modifyButton->setGeometry(530, 10 + 40 * (index - 1), 20, 20);
    connect(modifyButton, &QPushButton::clicked, this, [this, index] { onModifyButtonClicked(index - 1); });

    deleteButton->show();
    modifyButton->show();
}

void ActivityRegister::uiShowSearchResults(QVBoxLayout *layout, const std::vector<Activity*> &activities) {
    for (size_t i = 0; i < activities.size(); i++) {
        const auto& activity = activities[i];
        QHBoxLayout *activityLayout = uiCreateSearchResultsLayout(activity, i);
        layout->addLayout(activityLayout);
    }
    if (activities.empty()) {
        layout->addWidget(new QLabel("No results found"));
    }
}

QHBoxLayout* ActivityRegister::uiCreateSearchResultsLayout(Activity* activity, int index) {
    QHBoxLayout *activityLayout = new QHBoxLayout();

    QLabel *descriptionLabel = new QLabel(QString::fromStdString(activity->getDescription()));
    activityLayout->addWidget(descriptionLabel);

    QLabel *dateLabel = new QLabel(activity->getDate().toString());
    activityLayout->addWidget(dateLabel);

    QPushButton *viewButton = new QPushButton("View");
    connect(viewButton, &QPushButton::clicked, this, [this, activity] {
        onViewButtonClicked(activity->getDate());
    });
    activityLayout->addWidget(viewButton);

    QPushButton *mButton = new QPushButton("M");
    mButton->setToolTip("Modify");
    connect(mButton, &QPushButton::clicked, this, [this, index]() {
        onModifyButtonClicked(index, tmp_search);
    });
    activityLayout->addWidget(mButton);

    QPushButton *xButton = new QPushButton("X");
    connect(xButton, &QPushButton::clicked, this, [this, index]() {
        onDeleteButtonClicked(index, tmp_search);
    });
    activityLayout->addWidget(xButton);

    return activityLayout;
}

QPushButton* ActivityRegister::uiCreateModifyButton() {
    auto* modButton = new QPushButton("Modify", NewActivityDialog);
    modButton->setGeometry(290, 10, 100, 30);
    modButton->show();
    return modButton;
}