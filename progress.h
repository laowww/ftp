#ifndef RROGRESS_H
#define RROGRESS_H

#include <QMutex>
#include <QFile>
#include <QWidget>
#include <QFuture>
#include <QtConcurrent/QtConcurrentRun>

namespace Ui {
class Progress;
}

class FtpClient;
class Progress : public QWidget
{
    Q_OBJECT

public:
    explicit Progress(QWidget *parent = nullptr);
    ~Progress();

public:
    void downLoad(const QString &remote, const QString &local);

    void setProgress(double dltotal, double dlnow);

    void updateProgress();

    bool isFinished();

private:
    QString checkName(const QString &name);

    void downloadThread(const QString &remote, const QString &local);

private slots:
    void slot_pause();

public:
    static size_t writeCallback(char* contents, size_t size, size_t nmemb, void *file);
    static int progressCallback(void *obj, double dltotal, double dlnow, double ultotal, double ulnow);

private:
    Ui::Progress *ui;
    FtpClient *m_pFtp;

    QString m_remote;
    QString m_local;
    double m_dltotal;
    double m_dlnow;
    double m_speed;

    bool m_bPause;
    QFile m_file;
    QFuture<void> m_watcher;

    friend class FtpClient;
};

#endif // RROGRESS_H
