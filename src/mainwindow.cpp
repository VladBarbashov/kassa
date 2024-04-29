#include "mainwindow.h"

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

}

void MainWindow::on_arrivalCityLEdit_textEdited(const QString &arrivalCity)
{

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
