#ifndef FTPCLIENT_H
#define FTPCLIENT_H

#include <QObject>

#include "ftpclient.struct.h"

class FtpClientPrivate;
class FtpClient
{
public:
    FtpClient();
    ~FtpClient();

public:
    void fileList(QList<st_fileInfo> fileList, const QString &url, const QString &username = NULL, const QString &password = NULL);

    void setTimeOut(int time);

private:
    FtpClientPrivate *m_pd;
};

#endif // FTPCLIENT_H
