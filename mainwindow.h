#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class FtpClient;
class DownloadFile;
class QTreeWidgetItem;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void connectFtp(const QString &path);

private slots:
    void slot_connectFtp();

    void slot_download();
    void slot_openFolder();

    void slot_treeMenu(const QPoint &pos);
    void slot_openFolder(QTreeWidgetItem *pItem, int column);

private:
    Ui::MainWindow *ui;

    FtpClient *m_pFtp;
    DownloadFile *m_pDownload;
};
#endif // MAINWINDOW_H
