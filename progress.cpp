#include "progress.h"
#include "ui_progress.h"
#include "FtpClient.const.h"
#include "ftpclient.h"
#include "curl.h"

#include <QDebug>
#include <QFileInfo>

Progress::Progress(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Progress)
    , m_pFtp(new FtpClient)
    , m_remote("")
    , m_local("")
    , m_dltotal(0)
    , m_dlnow(0)
    , m_speed(0)
    , m_bPause(false)
{
    ui->setupUi(this);
    ui->progressBar->setRange(0, 100);
    ui->progressBar->setValue(0);
    ui->lb_progress->setText("0B/0B");

    connect(ui->btn_pause, SIGNAL(clicked()), this, SLOT(slot_pause()));
}

Progress::~Progress()
{
    delete ui;

    if(m_pFtp)
    {
        delete m_pFtp;
        m_pFtp = NULL;
    }
}

void Progress::downLoad(const QString &remote, const QString &local)
{
    m_remote = remote;
    m_local = checkName(local);
    ui->lb_name->setText(m_local.right(m_local.length() - m_local.lastIndexOf("/") - 1));

    m_watcher = QtConcurrent::run(this, &Progress::downloadThread, remote, m_local);
}

void Progress::setProgress(double dltotal, double dlnow)
{
    m_dltotal = dltotal;
    m_dlnow = dlnow;
}

void Progress::updateProgress()
{
    int progress = 100 * m_dlnow / m_dltotal;
    int speed = m_dlnow - m_speed;

    ui->progressBar->setValue(progress);
    ui->lb_progress->setText(byteConversion(m_dlnow) + "/" + byteConversion(m_dltotal));
    ui->lb_speed->setText(byteConversion(speed) + "/s");

    m_speed = m_dlnow;
}

bool Progress::isFinished()
{
    return (ui->progressBar->value() == 100);
}

QString Progress::checkName(const QString &name)
{
    QString newName = name;
    QFileInfo info(name);
    if(info.exists())
    {
        newName = info.absolutePath() + "/" + info.completeBaseName() + " - 副本." + info.suffix();

        int i = 2;
        while(QFile::exists(newName))
        {
            newName = info.absolutePath() + "/" + info.completeBaseName() + QString(" - 副本(%1).").arg(i) + info.suffix();
            i++;
        }
    }

    return newName;
}

void Progress::downloadThread(const QString &remote, const QString &local)
{
    m_pFtp->download(remote, local, this, &Progress::writeCallback, &Progress::progressCallback);
}

void Progress::slot_pause()
{
    m_bPause = !m_bPause;
    if(m_bPause)
    {
        m_watcher.cancel();
        ui->btn_pause->setText("继续");
    }
    else
    {
        ui->btn_pause->setText("暂停");
        m_watcher = QtConcurrent::run(this, &Progress::downloadThread, m_remote, m_local);
    }
}

size_t Progress::writeCallback(char *contents, size_t size, size_t nmemb, void *file)
{
    Progress *obj = static_cast<Progress *>(file);
    if(obj->m_bPause)
    {
        obj->m_file.close();
        return 0;
    }

    if(obj && obj->m_file.isOpen())
    {
        obj->m_file.write(contents, size * nmemb);
    }

    return (size * nmemb);
}

int Progress::progressCallback(void *obj, double dltotal, double dlnow, double ultotal, double ulnow)
{
    Progress *progress = static_cast<Progress *>(obj);
    if(progress)
    {
        progress->setProgress(dltotal, dlnow);
    }

    return 0;
}
