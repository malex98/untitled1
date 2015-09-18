#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    QTextEdit *te = new QTextEdit(this);
    setCentralWidget(te);
}

MainWindow::~MainWindow()
{

}
