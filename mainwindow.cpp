#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ftpclient.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_pFtp(NULL)
{
    ui->setupUi(this);
    setWindowTitle(QString::fromLocal8Bit("Ftp客户端"));

    ui->treeWidget->setRootIsDecorated(false);
    ui->treeWidget->header()->setSectionResizeMode(QHeaderView::Stretch);
    ui->treeWidget->setHeaderLabels(QStringList()<< QString::fromLocal8Bit("名称")<< QString::fromLocal8Bit("修改日期")<< QString::fromLocal8Bit("大小"));

    ui->le_ip->setText(QString::fromLocal8Bit("ftp://192.168.30.110"));
    ui->le_port->setText(QString::fromLocal8Bit("21"));
    connect(ui->btn_login, SIGNAL(clicked()), this, SLOT(slot_connectFtp()));
}

MainWindow::~MainWindow()
{
    delete ui;

    if(m_pFtp)
    {
        delete m_pFtp;
        m_pFtp = NULL;
    }
}

void MainWindow::slot_connectFtp()
{
    if(m_pFtp == NULL)
    {
        m_pFtp = new FtpClient;
    }
    ui->treeWidget->clear();

    QString url = ui->le_ip->text() + ":" + ui->le_port->text();
    m_pFtp->fileList(url, ui->le_username->text(), ui->le_password->text());
}

