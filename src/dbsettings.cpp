#include "dbsettings.h"

#include "ui_dbsettings.h"

DBSettings::DBSettings(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DBSettings)
{
    ui->setupUi(this);
}

DBSettings::~DBSettings()
{
    delete ui;
}

void DBSettings::on_resultDBtnBox_accepted()
{

}

void DBSettings::on_resultDBtnBox_rejected()
{

}
