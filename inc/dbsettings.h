#ifndef DBSETTINGS_H
#define DBSETTINGS_H

#include <QDialog>

namespace Ui
{
class DBSettings;
}

class DBSettings : public QDialog
{
    Q_OBJECT

public:
    explicit DBSettings(QWidget *parent = nullptr);
    ~DBSettings();

private slots:
    void on_resultDBtnBox_accepted();
    void on_resultDBtnBox_rejected();

private:
    Ui::DBSettings *ui;
};

#endif // DBSETTINGS_H
