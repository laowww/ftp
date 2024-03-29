#include "downloadfile.h"
#include "ui_downloadfile.h"
#include "ftpclient.h"
#include "progress.h"

DownloadFile::DownloadFile(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DownloadFile)
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

    m_progressList.append(pWdt);
    ui->verticalLayout->insertWidget(0, pWdt);

    pWdt->downLoad(remote, local);
}

void DownloadFile::slot_updateProgress()
{
    QList<Progress *>::iterator iter(m_progressList.begin());
    while(iter != m_progressList.end())
    {
        if((*iter)->isFinished())
        {
            m_progressList.erase(iter);
            ui->verticalLayout->removeWidget(*iter);
            (*iter)->deleteLater();
        }
        else
        {
            (*iter)->updateProgress();
        }

        iter++;
    }
}
