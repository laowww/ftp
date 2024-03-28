#ifndef DOWNLOADFILE_H
#define DOWNLOADFILE_H

#include <QDialog>
#include <QFuture>
#include <QTimer>
#include <QtConcurrent/QtConcurrentRun>

namespace Ui {
class DownloadFile;
}

class FtpClient;
class Progress;
class DownloadFile : public QDialog
{
    Q_OBJECT

public:
    explicit DownloadFile(QWidget *parent = nullptr);
    ~DownloadFile();

public:
    void download(const QString &remote, const QString &local);

private:
    void downloadThread(const QString &remote, const QString &local);

private slots:
    void slot_updateProgress();

public:
    static int progressCallback(void *obj, double dltotal, double dlnow, double ultotal, double ulnow);

private:
    Ui::DownloadFile *ui;

    FtpClient *m_pClient;
    QList<Progress *> m_progressList;
    QList<QFuture<void>> m_watcherList;

    QTimer *m_pTimer;
};

#endif // DOWNLOADFILE_H
