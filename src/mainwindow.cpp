#include "mainwindow.h"

#include <QLayoutItem>
#include <QMessageBox>
#include <QRegularExpressionValidator>
#include <QStringListModel>

#include "flight.h"
#include "ui_mainwindow.h"

void clearLayt(QLayout *layout);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , dbManager(new db::DBManager("main", "127.0.0.1", "vladislav", "8239", "flights"))
    , completer(new QCompleter(this))
    , dbWindow(new DBSettings(dbManager))
{
    ui->setupUi(this);
    ui->backFlightDEdit->setVisible(false);

    QRegularExpressionValidator *validator = new QRegularExpressionValidator(QRegularExpression("[А-я]{1,20}"), this);
    ui->departureCityLEdit->setValidator(validator);
    ui->arrivalCityLEdit->setValidator(validator);

    completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->departureCityLEdit->setCompleter(completer);
    ui->arrivalCityLEdit->setCompleter(completer);

    connect(dbWindow, &DBSettings::backToMainWindow, this, &MainWindow::enableWindow);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete dbManager;
    delete dbWindow;
}

void MainWindow::on_dbSettingsAct_triggered()
{
    this->setEnabled(false);
    dbWindow->show();
}

void MainWindow::enableWindow()
{
    this->setEnabled(true);
}

void MainWindow::on_departureCityLEdit_textEdited(const QString &departureCity)
{
    if (dbManager)
    {
        dbManager->performQuery("SELECT departureCity FROM flights WHERE departureCity LIKE ? ORDER BY departureCity", {departureCity + '%'});
        QList<QString> list;
        for (auto &i : dbManager->fields(0))
        {
            list << i.toString();
        }
        completer->setModel(new QStringListModel(list, this));
    }
}

void MainWindow::on_arrivalCityLEdit_textEdited(const QString &arrivalCity)
{
    if (dbManager)
    {
        dbManager->performQuery("SELECT arrivalCity FROM flights WHERE arrivalCity LIKE ? ORDER BY arrivalCity", {arrivalCity + '%'});
        QList<QString> list;
        for (auto &i : dbManager->fields(0))
        {
            list << i.toString();
        }
        completer->setModel(new QStringListModel(list, this));
    }
}

void MainWindow::on_changeTBtn_clicked()
{
    QString departureCity = ui->departureCityLEdit->text();
    ui->departureCityLEdit->setText(ui->arrivalCityLEdit->text());
    ui->arrivalCityLEdit->setText(departureCity);
}

void MainWindow::on_backFlightChk_stateChanged(int state)
{
    ui->backFlightDEdit->setVisible(state);
}

void MainWindow::on_searchPBtn_clicked()
{
    clearLayt(ui->flightsLayt);
    if (dbManager && (dbManager->connectionState() == db::DBState::OK))
    {
        if (dbManager->performQuery("SELECT f.flightNumber, f.departureCity, f.arrivalCity, f.departureTime, f.arrivalTime, f.planeType, fc.class, c.price "
                                    "FROM flights AS f JOIN cost AS c ON f.flightNumber=c.flightNumber JOIN crew AS cr ON f.flightNumber=cr.flightNumber "
                                    "JOIN flightClasses AS fc ON c.classId=fc.classId "
                                    "WHERE f.departureCity=? AND f.arrivalCity=? AND f.departureTime LIKE ? AND fc.class=? "
                                    "ORDER BY f.departureCity"
                    , {ui->departureCityLEdit->text(), ui->arrivalCityLEdit->text(), ui->departureDateDEdit->date().toString(Qt::DateFormat::ISODate) + '%'
                    , ui->flightClassCmb->currentText()}) == db::QueryRes::OK)
        {
            QList<QVariant> result = dbManager->nextRow();
            while (!result.empty())
            {
                QList<QVariant> flightData(result.cbegin(), result.cbegin() + 8);
                QList<QVariant> crewData(result.cbegin() + 8, result.cend());
                ui->flightsLayt->addWidget(new Flight(flightData, crewData), 0, Qt::AlignTop);
                result = dbManager->nextRow();
            }
        }
    }
    else
    {
        QMessageBox::warning(this, "Ошибка подключения", "Отсутствует подключение к базе данных (настройка в меню -> настройки бд)");
    }
}

void clearLayt(QLayout *layout)
{
    if (layout != NULL)
    {
        QLayoutItem *item;
        while((item = layout->takeAt(0)))
        {
            if (item->layout())
            {
                clearLayt(item->layout());
                delete item->layout();
            }
            delete item->widget();
            delete item;
        }
    }
}
