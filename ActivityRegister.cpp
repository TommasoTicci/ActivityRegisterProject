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
    searchBar = new QLineEdit(this);
    searchBar->setGeometry(250, 500, 200, 30);
    searchBar->setPlaceholderText("Search");
    searchBar->show();
    tmp_search = "";
    searchButton = new QPushButton("Search", this);
    searchButton->setGeometry(470, 500, 100, 30);
    searchButton->show();
    activityLabel = new QLabel("No activities", scrollRegisterArea);
    activityLabel->setGeometry(10, 10, 400, 20);
    auto * Font = new QFont(activityLabel->font());
    Font->setPointSize(10);
    activityLabel->setFont(*Font);
    activityLabel->show();
    delete Font;
    QObject::connect(addActivityButton, &QPushButton::clicked, this, &ActivityRegister::onNewActivityButtonClicked);
    QObject::connect(registerDate, &QDateEdit::dateChanged, this, &ActivityRegister::refreshRegister);
    QObject::connect(searchBar, &QLineEdit::textChanged, this, &ActivityRegister::onSearchBarChanged);
    QObject::connect(searchButton, &QPushButton::clicked, this, &ActivityRegister::onSearchButtonClicked);
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
    scrollRegisterArea->setGeometry(100, 90, 600, 400);
    scrollRegisterArea->show();

    int i = 0;

    // Cerca la data nel map e ottiene il vettore di attività corrispondente
    auto it = activities.find(registerDate->date());
    if (it != activities.end()) {
        const std::vector<Activity>& dayActivities = it->second;

        // Itera attraverso le attività per la data corrente
        for (const Activity& activity : dayActivities) {
            i++;
            activityLabel = new QLabel(QString::fromStdString(setCorrectDescriptionFormat(activity.getDescription())), scrollRegisterArea);
            activityLabel->setGeometry(10, 10 + 40 * (i - 1), 350, 20);
            timeLabel = new QLabel(QString::fromStdString(setCorrectHourFormat(activity.getStartTime(), activity.getEndTime())), scrollRegisterArea);
            timeLabel->setGeometry(350, 10 + 40 * (i - 1), 200, 20);

            auto* Font = new QFont(activityLabel->font());
            Font->setPointSize(10);
            activityLabel->setFont(*Font);
            timeLabel->setFont(*Font);

            deleteButton = new QPushButton("X", scrollRegisterArea);
            deleteButton->setGeometry(560, 10 + 40 * (i - 1), 20, 20);
            connect(deleteButton, &QPushButton::clicked, this, [this, i] { onDeleteButtonClicked(i - 1); });

            modifyButton = new QPushButton("M", scrollRegisterArea);
            modifyButton->setGeometry(530, 10 + 40 * (i - 1), 20, 20);
            connect(modifyButton, &QPushButton::clicked, this, [this, i] { onModifyButtonClicked(i - 1); });

            activityLabel->show();
            timeLabel->show();
            deleteButton->show();
            modifyButton->show();
            delete Font;
        }
    }

    // Se non ci sono attività per quella data, mostra "No activities"
    if (i == 0) {
        activityLabel = new QLabel("No activities", scrollRegisterArea);
        activityLabel->setGeometry(10, 10, 400, 20);
        auto* Font = new QFont(activityLabel->font());
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
    try {
        Activity newActivity(tmp_description, tmp_startTime, tmp_endTime, tmp_date);

        if (canAcceptActivity(tmp_date)) {
            addActivity(newActivity);
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

    } catch (const std::invalid_argument &e) {
        auto *MsgBox = new QMessageBox(this);
        MsgBox->setWindowTitle("Error");
        MsgBox->setAttribute(Qt::WA_DeleteOnClose, true);
        MsgBox->setText(e.what());
        MsgBox->show();
    }
}

void ActivityRegister::addActivity(const Activity& newActivity) {
    activities[newActivity.getDate()].push_back(newActivity);
    ActivityRegister::refreshRegister();
}

void ActivityRegister::addActivity(const std::string &description, const QTime &startTime, const QTime &endTime,const QDate &date) {
    const std::string& new_description = description;
    QTime new_startTime = startTime;
    QTime new_endTime = endTime;
    QDate new_date = date;
    Activity newActivity(new_description, new_startTime, new_endTime, new_date);
    activities[newActivity.getDate()].push_back(newActivity);
    ActivityRegister::refreshRegister();
}

void ActivityRegister::onSearchButtonClicked() {
    std::vector<Activity*> searchedActivity = getSearchActivities(tmp_search);

    // Crea il dialogo per i risultati della ricerca
    NewActivityDialog = new QDialog(this);
    NewActivityDialog->setAttribute(Qt::WA_DeleteOnClose, true);
    NewActivityDialog->setWindowTitle("Search Results");
    NewActivityDialog->setWindowModality(Qt::WindowModality::ApplicationModal);
    NewActivityDialog->setFixedSize(600, 400);

    // Crea un'area di scorrimento
    QScrollArea *scrollArea = new QScrollArea(NewActivityDialog);
    scrollArea->setWidgetResizable(true); // Permette all'area di scorrimento di adattarsi al contenuto

    // Crea un widget contenitore per le attività
    QWidget *scrollWidget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(scrollWidget);

    // Aggiungi le attività come etichette al layout
    for (int i = 0; i < searchedActivity.size(); i++) {
        const auto& activity = searchedActivity[i]; // Utilizza l'indice per accedere all'attività

        QHBoxLayout *activityLayout = new QHBoxLayout(); // Layout orizzontale per attività

        // Etichetta per la descrizione
        QLabel *descriptionLabel = new QLabel(QString::fromStdString(activity->getDescription()));
        activityLayout->addWidget(descriptionLabel);

        // Etichetta per la data
        QLabel *dateLabel = new QLabel(activity->getDate().toString()); // Assicurati che `getDate` ritorni una stringa
        activityLayout->addWidget(dateLabel);

        // Pulsante View
        QPushButton *viewButton = new QPushButton("View");
        connect(viewButton, &QPushButton::clicked, this, [this, activity] { onViewButtonClicked(activity->getDate()); });
        activityLayout->addWidget(viewButton);

        // Pulsante M
        QPushButton *mButton = new QPushButton("M");
        mButton->setToolTip("Modify");
        connect(mButton, &QPushButton::clicked, this, [this, i, searchedActivity]() {
            onModifyButtonClicked(i, tmp_search); // Passa l'indice dell'attività da modificare
        });
        activityLayout->addWidget(mButton);

        // Pulsante X
        QPushButton *xButton = new QPushButton("X");
        connect(xButton, &QPushButton::clicked, this, [this, i, searchedActivity]() {
            onDeleteButtonClicked(i, tmp_search); // Passa l'indice dell'attività da modificare
        });
        activityLayout->addWidget(xButton);

        // Aggiungi il layout dell'attività al layout principale
        layout->addLayout(activityLayout);
    }

    scrollWidget->setLayout(layout); // Imposta il layout sul widget contenitore
    scrollArea->setWidget(scrollWidget); // Imposta il widget nell'area di scorrimento

    // Aggiungi l'area di scorrimento al dialogo
    QVBoxLayout *dialogLayout = new QVBoxLayout(NewActivityDialog);
    dialogLayout->addWidget(scrollArea);
    NewActivityDialog->setLayout(dialogLayout);

    // Mostra il dialogo
    NewActivityDialog->exec(); // Mostra il dialogo come modale
}

std::vector<Activity*> ActivityRegister::getSearchActivities(std::string &search) {
    std::vector<Activity*> searchedActivities;
    for (auto &it : activities) {
        for (auto &activity : it.second) {
            if (activity.getDescription() == search) {
                searchedActivities.push_back(&activity); // Directly push back the pointer
            }
        }
    }
    return searchedActivities;
}

void ActivityRegister::onCancelButtonClicked () {
    delete NewActivityDialog;
}

void ActivityRegister::onDeleteButtonClicked(int index) {
    // Trova le attività associate alla data corrente
    auto it = activities.find(registerDate->date());

    // Se troviamo la data nella mappa
    if (it != activities.end()) {
        // Ottieni il vettore di attività per quella data
        std::vector<Activity> &dayActivities = it->second;

        // Controlla se l'indice è valido
        if (index >= 0 && index < dayActivities.size()) {
            // Rimuovi l'attività dal vettore in base all'indice
            dayActivities.erase(dayActivities.begin() + index);

            // Se il vettore è vuoto dopo la rimozione, elimina la chiave dalla mappa
            if (dayActivities.empty()) {
                activities.erase(it);
            }

            // Aggiorna l'interfaccia grafica
            ActivityRegister::refreshRegister();
        }
    }
}

void ActivityRegister::onDeleteButtonClicked(int index, std::string search) {
    NewActivityDialog->close();

    // Trova tutte le attività con la stessa descrizione
    auto dateIt = activities.begin();
    int count = 0;

    while (dateIt != activities.end()) {
        auto& activityList = dateIt->second;

        // Cerchiamo l'attività con la descrizione corrispondente
        for (auto it = activityList.begin(); it != activityList.end(); ++it) {
            if (it->getDescription() == search) {
                if (count == index) {
                    // Rimuovi l'attività
                    activityList.erase(it);

                    // Se la lista di attività diventa vuota, rimuovi la data dalla mappa
                    if (activityList.empty()) {
                        dateIt = activities.erase(dateIt); // Rimuove l'elemento e ritorna il nuovo iteratore
                    } else {
                        ++dateIt; // Solo incrementa se non abbiamo rimosso l'elemento
                    }

                    refreshRegister();
                    return; // Cancellazione avvenuta con successo
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
    NewActivityDialog = new QDialog(this);
    NewActivityDialog->setAttribute ( Qt::WA_DeleteOnClose, true );
    NewActivityDialog->setWindowTitle("New Activity");
    NewActivityDialog->setWindowModality(Qt::WindowModality::ApplicationModal);
    NewActivityDialog->setFixedSize(400, 300);
    NewActivityDialog->show();
    auto it = activities.find(registerDate->date());
    if (it != activities.end()) {
        std::vector<Activity> &dayActivities = it->second;
        tmp_description = dayActivities[index].getDescription();
        tmp_startTime = dayActivities[index].getStartTime();
        tmp_endTime = dayActivities[index].getEndTime();
        tmp_date = registerDate->date();
        auto *descriptionLabel = new QLabel("Description", NewActivityDialog);
        descriptionLabel->setGeometry(10, 10, 200, 30);
        descriptionLabel->show();
        auto *descriptionArea = new QLineEdit(QString::fromStdString(tmp_description), NewActivityDialog);
        descriptionArea->setGeometry(10, 50, 200, 30);
        descriptionArea->show();
        auto *timeLabel = new QLabel("Start Time                      End Time", NewActivityDialog);
        timeLabel->setGeometry(10, 90, 200, 30);
        timeLabel->show();
        auto *startTimeArea = new QTimeEdit(tmp_startTime,NewActivityDialog);
        startTimeArea->setGeometry(10, 130, 70, 30);
        startTimeArea->show();
        auto *endTimeArea = new QTimeEdit(tmp_endTime,NewActivityDialog);
        endTimeArea->setGeometry(130, 130, 70, 30);
        endTimeArea->show();
        auto *modButton = new QPushButton("Modify", NewActivityDialog);
        modButton->show();
        modButton->setGeometry(290, 10, 100, 30);
        auto *cancelButton = new QPushButton("Cancel", NewActivityDialog);
        cancelButton->show();
        cancelButton->setGeometry(290, 50, 100, 30);
        auto *dateLabel = new QLabel("Date", NewActivityDialog);
        dateLabel->setGeometry(10, 170, 100, 30);
        dateLabel->show();
        auto *dateArea = new QDateEdit(registerDate->date(), NewActivityDialog);
        dateArea->setGeometry(10, 210, 170, 30);
        dateArea->show();
        connect(modButton, &QPushButton::clicked, this, [this, &dayActivities, index]() {
            try {
                dayActivities[index].modifyActivity(tmp_description, tmp_startTime, tmp_endTime, tmp_date);
                if (tmp_date != registerDate->date()) {
                    activities[tmp_date].push_back(dayActivities[index]);
                    activities[registerDate->date()].erase(activities[registerDate->date()].begin() + index);
                }
                refreshRegister();
                NewActivityDialog->close();
            } catch (const std::invalid_argument &e) {
                auto *MsgBox = new QMessageBox(this);
                MsgBox->setWindowTitle("Error");
                MsgBox->setAttribute(Qt::WA_DeleteOnClose, true);
                MsgBox->setText(e.what());
                MsgBox->show();
            }
        });
        connect(cancelButton, &QPushButton::clicked, this, &ActivityRegister::onCancelButtonClicked);
        connect(descriptionArea, &QLineEdit::textChanged, this, &ActivityRegister::onDescriptionChanged);
        connect(startTimeArea, &QTimeEdit::timeChanged, this, &ActivityRegister::onStartTimeChanged);
        connect(endTimeArea, &QTimeEdit::timeChanged, this, &ActivityRegister::onEndTimeChanged);
        connect(dateArea, &QDateEdit::dateChanged, this, &ActivityRegister::onDateChanged);
    }
}

void ActivityRegister::onModifyButtonClicked(int index, std::string search) {
    NewActivityDialog->close();
    NewActivityDialog = new QDialog(this);
    NewActivityDialog->setAttribute ( Qt::WA_DeleteOnClose, true );
    NewActivityDialog->setWindowTitle("New Activity");
    NewActivityDialog->setWindowModality(Qt::WindowModality::ApplicationModal);
    NewActivityDialog->setFixedSize(400, 300);
    NewActivityDialog->show();
    std::vector<Activity*> searchedActivity = getSearchActivities(search);
        tmp_description = searchedActivity[index]->getDescription();
        tmp_startTime = searchedActivity[index]->getStartTime();
        tmp_endTime = searchedActivity[index]->getEndTime();
        tmp_date = searchedActivity[index]->getDate();
        auto *descriptionLabel = new QLabel("Description", NewActivityDialog);
        descriptionLabel->setGeometry(10, 10, 200, 30);
        descriptionLabel->show();
        auto *descriptionArea = new QLineEdit(QString::fromStdString(tmp_description), NewActivityDialog);
        descriptionArea->setGeometry(10, 50, 200, 30);
        descriptionArea->show();
        auto *timeLabel = new QLabel("Start Time                      End Time", NewActivityDialog);
        timeLabel->setGeometry(10, 90, 200, 30);
        timeLabel->show();
        auto *startTimeArea = new QTimeEdit(tmp_startTime,NewActivityDialog);
        startTimeArea->setGeometry(10, 130, 70, 30);
        startTimeArea->show();
        auto *endTimeArea = new QTimeEdit(tmp_endTime,NewActivityDialog);
        endTimeArea->setGeometry(130, 130, 70, 30);
        endTimeArea->show();
        auto *modButton = new QPushButton("Modify", NewActivityDialog);
        modButton->show();
        modButton->setGeometry(290, 10, 100, 30);
        auto *cancelButton = new QPushButton("Cancel", NewActivityDialog);
        cancelButton->show();
        cancelButton->setGeometry(290, 50, 100, 30);
        auto *dateLabel = new QLabel("Date", NewActivityDialog);
        dateLabel->setGeometry(10, 170, 100, 30);
        dateLabel->show();
        auto *dateArea = new QDateEdit(tmp_date, NewActivityDialog);
        dateArea->setGeometry(10, 210, 170, 30);
        dateArea->show();
        connect(modButton, &QPushButton::clicked, this, [this, searchedActivity, index]() {
            try {
                QDate oldDate = searchedActivity[index]->getDate();
                Activity *oldActivity = searchedActivity[index];
                searchedActivity[index]->modifyActivity(tmp_description, tmp_startTime, tmp_endTime, tmp_date);

                // Se la data è cambiata, aggiorna anche la chiave nella mappa
                if (oldDate != tmp_date) {
                    // Trova la posizione della vecchia attività nella mappa
                    auto dateIt = activities.find(oldDate);
                    if (dateIt != activities.end()) {
                        auto& activityList = dateIt->second;

                        activities[tmp_date].push_back(*oldActivity);

                        // Rimuovi l'attività dalla vecchia data
                        activityList.erase(std::remove_if(activityList.begin(), activityList.end(),
                                                          [searchedActivity, index, &oldActivity](const Activity& activity) {
                                                              return activity.getStartTime() == oldActivity->getStartTime() &&
                                                                     activity.getEndTime() == oldActivity->getEndTime() &&
                                                                     activity.getDescription() == oldActivity->getDescription();
                                                          }), activityList.end());

                        // Se la lista di attività per la vecchia data è vuota, rimuovi la chiave dalla mappa
                        if (activityList.empty()) {
                            activities.erase(dateIt);
                        }
                    }

                    // Aggiungi l'attività modificata nella nuova data

                } else {
                    // Se la data non è cambiata, semplicemente aggiorna i dettagli
                    refreshRegister();
                }
                refreshRegister();
                NewActivityDialog->close();
            } catch (const std::invalid_argument &e) {
                auto *MsgBox = new QMessageBox(this);
                MsgBox->setWindowTitle("Error");
                MsgBox->setAttribute(Qt::WA_DeleteOnClose, true);
                MsgBox->setText(e.what());
                MsgBox->show();
            }
        });
    connect(cancelButton, &QPushButton::clicked, this, &ActivityRegister::onCancelButtonClicked);
    connect(descriptionArea, &QLineEdit::textChanged, this, &ActivityRegister::onDescriptionChanged);
    connect(startTimeArea, &QTimeEdit::timeChanged, this, &ActivityRegister::onStartTimeChanged);
    connect(endTimeArea, &QTimeEdit::timeChanged, this, &ActivityRegister::onEndTimeChanged);
    connect(dateArea, &QDateEdit::dateChanged, this, &ActivityRegister::onDateChanged);
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
        return it->second.size();
    }
    return 0;
}

bool ActivityRegister::canAcceptActivity(const QDate &date) const
{
    return ActivityRegister::getActivitiesPerDay(date) < maxActivitiesPerDay;
}

std::map<QDate, std::vector<Activity>> ActivityRegister::getActivities() {
    return activities;
}

int ActivityRegister::getMaxActivitiesPerDay() {
    return maxActivitiesPerDay;
}