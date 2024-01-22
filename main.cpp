#include "mainwindow.h"

#include <QTextCodec>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("system"));

    MainWindow w;
    w.show();
    return a.exec();
}
