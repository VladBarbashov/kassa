#ifndef DBSETTINGS_H
#define DBSETTINGS_H

#include <QDialog>
#include <QRegularExpressionValidator>

#include "dbmanager.h"

namespace Ui
{
class DBSettings;
}

class DBSettings : public QDialog
{
    Q_OBJECT

public:
    explicit DBSettings(db::DBManager *dbManager, QWidget *parent = nullptr);
    ~DBSettings();

private slots:
    void on_resultDBtnBox_accepted();
    void on_resultDBtnBox_rejected();

private:
    Ui::DBSettings *ui;

    QRegularExpressionValidator *validator;

    db::DBManager *dbManager;
};

#endif // DBSETTINGS_H
