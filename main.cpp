#include "mainwindow.h"
#include "ftp_custom.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    Ftp_custom ftp("127.0.0.1",21,"elitakpk","123");
    return a.exec();
}
