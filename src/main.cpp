#include <QApplication>
#include <QMessageBox>

#include "mainwindow.h"

int main(int argc, char *argv[])
try
{
    QApplication a(argc, argv);
    MainWindow w;
    w.showMaximized();
    return a.exec();
}
catch (std::bad_alloc &exception)
{
    QMessageBox::critical(nullptr, "Ошибка выделения памяти", exception.what());
}
catch (...)
{
    QMessageBox::critical(nullptr, "Ошибка", "Произошла ошибка, пожалуйста перезапустите программу");
}
