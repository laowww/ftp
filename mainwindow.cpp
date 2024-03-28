#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ftpclient.h"
#include "downloadfile.h"

#include <QDir>
#include <QDebug>
#include <qmath.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_pFtp(new FtpClient)
    , m_pDownload(new DownloadFile(this))
{
    ui->setupUi(this);
    setWindowTitle("Ftp客户端" );

    ui->treeWidget->setRootIsDecorated(false);
    ui->treeWidget->header()->setSectionResizeMode(QHeaderView::Stretch);
    ui->treeWidget->setHeaderLabels(QStringList()<< "名称" << "修改日期" << "大小" );
    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(slot_openFolder(QTreeWidgetItem *, int)));
    connect(ui->treeWidget, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(slot_treeMenu(const QPoint &)));

    ui->le_ip->setText("ftp://192.168.30.103:21");
    connect(ui->btn_login, SIGNAL(clicked()), this, SLOT(slot_connectFtp()));

    m_pDownload->show();
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

void MainWindow::connectFtp(const QString &path)
{
    ui->treeWidget->clear();

    QList<st_fileInfo> fileList;
    m_pFtp->fileList(fileList, path);
    foreach(const st_fileInfo &info, fileList)
    {
        QTreeWidgetItem *pItem = new QTreeWidgetItem;
        pItem->setText(0, info.name);
        pItem->setText(1, info.date);
        pItem->setText(2, QString::number(qCeil(info.size / 1024.0)) + "KB");
        pItem->setData(0, Qt::UserRole, info.bDir);
        pItem->setData(0, Qt::UserRole + 1, info.size);

        ui->treeWidget->addTopLevelItem(pItem);
    }

    ui->lb_dir->setText(path);
}

void MainWindow::slot_connectFtp()
{
    m_pFtp->setUser(ui->le_username->text(), ui->le_password->text());
    QString url = ui->le_ip->text();

    connectFtp(url);
}

void MainWindow::slot_download()
{
    QModelIndex index = ui->treeWidget->currentIndex();
    index = index.siblingAtColumn(0);
    if(index.isValid())
    {
        QString name = index.data(Qt::DisplayRole).toString();
        QString remote = QString("%1/%2").arg(ui->lb_dir->text()).arg(name);
        QString local = QString("%1/%2").arg(QApplication::applicationDirPath()).arg(name);

        m_pDownload->download(remote, local);
    }
}

void MainWindow::slot_openFolder()
{
    QModelIndex index = ui->treeWidget->currentIndex();
    index = index.siblingAtColumn(0);
    if(index.isValid())
    {
        QString name = index.data(Qt::DisplayRole).toString();
        QString remote = QString("%1/%2/").arg(ui->lb_dir->text()).arg(name);
        connectFtp(remote);
    }
}

void MainWindow::slot_treeMenu(const QPoint &pos)
{
    QModelIndex index = ui->treeWidget->indexAt(pos);
    index = index.siblingAtColumn(0);
    if(index.isValid())
    {
        QMenu menu(this);

        if(index.data(Qt::UserRole).toBool())
        {
            menu.addAction("打开", this, SLOT(slot_openFolder()));
        }
        else
        {
            menu.addAction("下载", this, SLOT(slot_download()));
        }

        menu.exec(QCursor().pos());
    }
}

void MainWindow::slot_openFolder(QTreeWidgetItem *pItem, int column)
{
    Q_UNUSED(column);
    if(pItem->data(0, Qt::UserRole).toBool())
    {
        slot_openFolder();
    }
}

