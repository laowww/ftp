#ifndef DOWNLOADFILE_H
#define DOWNLOADFILE_H

#include <QDialog>
#include <QTimer>

namespace Ui {
class DownloadFile;
}

class Progress;
class DownloadFile : public QDialog
{
    Q_OBJECT

public:
    explicit DownloadFile(QWidget *parent = nullptr);
    ~DownloadFile();

public:
    void download(const QString &remote, const QString &local);

private slots:
    void slot_updateProgress();

private:
    Ui::DownloadFile *ui;

    QList<Progress *> m_progressList;

    QTimer *m_pTimer;
};

#endif // DOWNLOADFILE_H
