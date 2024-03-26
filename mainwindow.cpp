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
    setWindowTitle("Ftp客户端" );

    ui->treeWidget->setRootIsDecorated(false);
    ui->treeWidget->header()->setSectionResizeMode(QHeaderView::Stretch);
    ui->treeWidget->setHeaderLabels(QStringList()<< "名称" << "修改日期" << "大小" );

    ui->le_ip->setText("ftp://192.168.30.103" );
    ui->le_port->setText("21" );
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

void MainWindow::updateProgress(qint64 dltotal, qint64 dlnow, qint64 ultotal, qint64 ulnow)
{

}

void MainWindow::slot_connectFtp()
{
    if(m_pFtp == NULL)
    {
        m_pFtp = new FtpClient;
    }
    ui->treeWidget->clear();

    m_pFtp->setUser(ui->le_username->text(), ui->le_password->text());
    QString url = ui->le_ip->text() + ":" + ui->le_port->text();
    QList<st_fileInfo> fileList;
    m_pFtp->fileList(fileList, url);

    foreach(const st_fileInfo &info, fileList)
    {
        QTreeWidgetItem *pItem = new QTreeWidgetItem;
        pItem->setText(0, info.name);
        pItem->setText(1, info.date);
        pItem->setText(2, info.size);

        ui->treeWidget->addTopLevelItem(pItem);
    }

    m_pFtp->download("ftp://192.168.30.103:21/1.exe" , QApplication::applicationDirPath()+"/安装包.exe" );
}

