#include "dbsettings.h"

#include <QMessageBox>
#include <QString>

#include "ui_dbsettings.h"

#define MYSQL_INDEX 0
#define POSTGRES_INDEX 1
#define SQLITE_INDEX 2

DBSettings::DBSettings(db::DBManager *dbManager, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DBSettings)
    , dbManager(dbManager)
{
    ui->setupUi(this);

    QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    QRegularExpression ipRegex ("^" + ipRange +
                                "\\." + ipRange +
                                "\\." + ipRange +
                                "\\." + ipRange + "$");
    validator = new QRegularExpressionValidator(ipRegex, this);
    ui->ipLEdit->setValidator(validator);
}

DBSettings::~DBSettings()
{
    delete ui;
    delete validator;
}

void DBSettings::on_resultDBtnBox_accepted()
{
    db::DBDriver driver;
    switch (ui->driverCmb->currentIndex())
    {
    case MYSQL_INDEX:
        driver = db::DBDriver::MYSQL;
        break;
    case POSTGRES_INDEX:
        driver = db::DBDriver::POSTGRES;
        break;
    case SQLITE_INDEX:
        driver = db::DBDriver::SQLITE;
        break;
    default:
        QMessageBox::warning(this, "Ошибка", "Драйвер недоступен");
        return;
    }
    if (dbManager)
    {
        dbManager->removeConnection("main");
        dbManager->addNewConnection("main", ui->ipLEdit->text(), ui->userLEdit->text(), ui->passwdLEdit->text(), ui->dbNameLEdit->text(), driver);
    }
    else
    {
        QMessageBox::warning(this, "Ошибка", "Менеджер баз данных недоступен");
    }
    this->close();
}

void DBSettings::on_resultDBtnBox_rejected()
{
    this->close();
}
