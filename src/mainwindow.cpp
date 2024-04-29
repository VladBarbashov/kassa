#include "mainwindow.h"

#include <QStringListModel>

#include "dbsettings.h"
#include "ui_mainwindow.h"

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
    if (dbManager && (dbManager->connectionState() == db::DBState::OK))
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
    if (dbManager && (dbManager->connectionState() == db::DBState::OK))
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

}
