#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class FtpClient;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void updateProgress(qint64 dltotal, qint64 dlnow, qint64 ultotal, qint64 ulnow);

private slots:
    void slot_connectFtp();

private:
    Ui::MainWindow *ui;

    FtpClient *m_pFtp;
};
#endif // MAINWINDOW_H
