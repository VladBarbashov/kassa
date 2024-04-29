#include "mainwindow.h"

#include <QLayoutItem>
#include <QStringListModel>

#include "dbsettings.h"
#include "flight.h"
#include "ui_mainwindow.h"

void clearLayt(QLayout *layout);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , validator (new QRegularExpressionValidator(QRegularExpression("[А-я]{1,20}"), this))
    , completer (new QCompleter)
    , dbManager (new db::DBManager("main", "127.0.0.1", "vladislav", "8239", "flights"))
{
    ui->setupUi(this);
    ui->backFlightDEdit->setVisible(false);

    ui->departureCityLEdit->setValidator(validator);
    ui->arrivalCityLEdit->setValidator(validator);

    completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->departureCityLEdit->setCompleter(completer);
    ui->arrivalCityLEdit->setCompleter(completer);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete validator;
    delete completer;
    delete dbManager;
}

void MainWindow::on_dbSettingsAct_triggered()
{
    DBSettings *dbWindow = new DBSettings(dbManager, this);
    dbWindow->show();
}

void MainWindow::on_departureCityLEdit_textEdited(const QString &departureCity)
{
    if (dbManager)
    {
        dbManager->performQuery(std::format("SELECT departureCity FROM flights WHERE departureCity LIKE '{}%' ORDER BY departureCity", departureCity.toStdString()).c_str());
        QStringList list;
        for (auto &i : dbManager->fields(0))
        {
            list << i.toString();
        }
        completer->setModel(new QStringListModel(list, completer));
    }
}

void MainWindow::on_arrivalCityLEdit_textEdited(const QString &arrivalCity)
{
    if (dbManager)
    {
        dbManager->performQuery(std::format("SELECT arrivalCity FROM flights WHERE arrivalCity LIKE '{}%' ORDER BY arrivalCity", arrivalCity.toStdString()).c_str());
        QStringList list;
        for (auto &i : dbManager->fields(0))
        {
            list << i.toString();
        }
        completer->setModel(new QStringListModel(list, completer));
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
    if (dbManager)
    {
        dbManager->performQuery(std::format("SELECT f.flightNumber, f.departureCity, f.arrivalCity, f.departureTime, f.arrivalTime, f.planeType, fc.class, c.price "
                                            "FROM flights AS f JOIN cost AS c ON f.flightNumber=c.flightNumber JOIN crew AS cr ON f.flightNumber=cr.flightNumber JOIN flightClasses AS fc ON c.classId=fc.classId "
                                            "WHERE f.departureCity='{}' AND f.arrivalCity='{}' AND f.departureTime LIKE '{}%' AND fc.class='{}' "
                                            "ORDER BY f.departureCity"
                                            , ui->departureCityLEdit->text().toStdString(), ui->arrivalCityLEdit->text().toStdString()
                                            , ui->departureDateDEdit->date().toString(Qt::DateFormat::ISODate).toStdString(), ui->flightClassCmb->currentText().toStdString()).c_str());
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
