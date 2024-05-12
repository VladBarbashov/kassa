#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QCompleter>
#include <QMainWindow>

#include "dbmanager.h"
#include "dbsettings.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_dbSettingsAct_triggered();
    void enableWindow();

    void on_departureCityLEdit_textEdited(const QString &departureCity);
    void on_arrivalCityLEdit_textEdited(const QString &arrivalCity);

    void on_changeTBtn_clicked();
    void on_backFlightChk_stateChanged(int state);
    void on_searchPBtn_clicked();

private:
    Ui::MainWindow *ui;

    db::DBManager *dbManager;

    QCompleter *completer;

    DBSettings *dbWindow;
};

#endif // MAINWINDOW_H
