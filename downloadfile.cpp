#include "downloadfile.h"
#include "ui_downloadfile.h"
#include "ftpclient.h"
#include "progress.h"

DownloadFile::DownloadFile(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DownloadFile)
    , m_pClient(new FtpClient)
    , m_pTimer(new QTimer(this))
{
    ui->setupUi(this);
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(slot_updateProgress()));
    m_pTimer->start(1000);
}

DownloadFile::~DownloadFile()
{
    delete ui;
}

void DownloadFile::download(const QString &remote, const QString &local)
{
    Progress *pWdt = new Progress(this);
    pWdt->setName(local.right(local.size() - local.lastIndexOf("/") - 1));

    m_progressList.append(pWdt);
    ui->verticalLayout->insertWidget(0, pWdt);

    QtConcurrent::run(this, &DownloadFile::downloadThread, remote, local);
}

int DownloadFile::progressCallback(void *obj, double dltotal, double dlnow, double ultotal, double ulnow)
{
    Progress *progress = static_cast<Progress *>(obj);
    if(progress)
    {
        progress->setDownLoad(dltotal, dlnow);
    }

    return 0;
}

void DownloadFile::downloadThread(const QString &remote, const QString &local)
{
    m_pClient->download(remote, local, m_progressList.last(), &DownloadFile::progressCallback);
}

void DownloadFile::slot_updateProgress()
{
    foreach(Progress *obj, m_progressList)
    {
        obj->updateProgress();
    }
}
