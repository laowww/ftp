#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ftpclient.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(QString::fromLocal8Bit("Ftp客户端"));

    FtpClient ftp;
    ftp.fileList(QString::fromLocal8Bit("ftp://192.168.30.110:21"), "", "");
}

MainWindow::~MainWindow()
{
    delete ui;
}

